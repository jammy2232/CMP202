#include "BattleScene.h"

BattleScene* BattleScene::instance_ = nullptr;

BattleScene::BattleScene()
{

	// Create instance
	instance_ = this;

	// Setup barrier objects and threads
	ProcessBarrier = new Barrier(1 + 1 + PROCESSINGTHEADS);
	RenderBarrier = new Barrier(1 + 1 + PROCESSINGTHEADS);
	FrameEndBarrier = new Barrier(1 + 1 + PROCESSINGTHEADS);


}


BattleScene::~BattleScene()
{
	CleanUp();
}


bool BattleScene::Init()
{

	// Create the world 
	world_ = new GameWorld(mapSize);

	// Start the pathfinder threads
	for (int threads = 0; threads < PATHFINDINGTHEADS; threads++)
	{
		pathfinder.push_back(new PathFinder(world_->GetStaticPathfinderMap(), world_->GetMapDimension()));
	}
	
	// Setup the view windows for main and minimaps
	SetUpViewWindows();

	// Get the map random map information (exluding extention - expects .xml and .png to have the same name)
	spriteRenderer_ = new SpriteRenderer("Textures/medievalRTS_spritesheet");

	// Setup the gameobject list
	units_ = new GameObjectManager(maxUnits, *world_);
	projectiles_ = new GameObjectManager(maxProjectiles, *world_);

	// variable for team selecetion
	Unit::TEAM teamSelection = Unit::TEAM::BLUE;

	// Spawn Units in random locations
	for (int i = 0; i < maxUnits; i++)
	{

		// Assign a team (evenly)
		if (teamSelection == Unit::TEAM::BLUE)
		{
			teamSelection = Unit::TEAM::RED;
		}
		else
		{
			teamSelection = Unit::TEAM::BLUE;
		}

		// Get a random tile
		sf::Vector2i tile = world_->GetRandomFreeTile();

		// Spawn the unit
		Unit* unit = new Unit(tile, teamSelection);

		// Setup the new unit
		unit->Init(*world_, new SearchAndDestoy(*unit));

		// Add the unit to the manager
		units_->AddObject(unit);

		// Create a new unit
		world_->SetUnitOnTile(unit, tile);

	}

	// Setup the worker threads
	for (int threads = 0; threads < PROCESSINGTHEADS; threads++)
	{
		workers_.push_back(new std::thread(&BattleScene::ProcessUnits, this));
		workers_[threads]->detach();

	}

	// return to show error free;
	return true;

}


void BattleScene::CleanUp()
{

	// Stop playing
	play = false;

	// Delete all pathfiding modules
	for (auto pathf : pathfinder)
	{
		pathf->required = false;

		while (pathf->active)
		{
			// wait
		}

		delete pathf;

	}

	delete world_;
	world_ = nullptr;

	delete spriteRenderer_;
	spriteRenderer_ = nullptr;

	delete units_;
	units_ = nullptr;

	// Clean up the worker threads
	for (auto worker : workers_)
	{
		delete worker;
	}

}


void BattleScene::HandleInput(float delta_time)
{
	// Scrollable keyboard controls 

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		main_view.move(-300.0f * delta_time * std::abs(moveMod), 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		main_view.move(300.0f * delta_time * std::abs(moveMod), 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		main_view.move(0.0f, -300.0f * delta_time * std::abs(moveMod));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		main_view.move(0.0f, 300.0f * delta_time * std::abs(moveMod));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		sf::Vector2f size = main_view.getSize();
		main_view.setSize(size + (size * zoom * delta_time));
		moveMod += 0.1f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		sf::Vector2f size = main_view.getSize();
		main_view.setSize(size - (size * zoom * delta_time));
		moveMod -= 0.1f;
	}

}


void BattleScene::Update(float dt)
{

	// Update the delta time
	delta_time = dt;

	// Store the frame rate counter
	DataLogger::LogValue("FPS", 1.0f / dt);

	// Create the work lists
	units_->CreateTaskList();
	projectiles_->CreateTaskList();

	// wait till all the threads have completed before flagging update complete

	// WAIT FOR DT TO BE SET
	ProcessBarrier->wait();

	// ALL UNIT/PROJECTILE PROCESSING COMPLETE
	RenderBarrier->wait();

	// END OF FRAME ALL RENDERING COMPLETE
	FrameEndBarrier->wait();

}


void BattleScene::Render(sf::RenderWindow & window)
{

	// Queue the world to be rendered
	world_->GenerateMap(*spriteRenderer_);

	// WAIT FOR DT TO BE SET
	ProcessBarrier->wait();

	// Main Render Function **********************
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		window.setView(main_view);
	}

	// ALL UNIT/PROJECTILE PROCESSING COMPLETE
	RenderBarrier->wait();

	// Render all the processed Projectiles
	spriteRenderer_->RenderFrame(window);

	// Frame end after UI

}


void BattleScene::RenderUI(sf::RenderWindow & window)
{

	// Main Render Function **********************
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		window.setView(miniMap_View);
	}

	// Queue the world to be rendered
	world_->GenerateMap(*spriteRenderer_);

	// Render all the processed Projectiles
	spriteRenderer_->RenderFrame(window);

	// Render the borders fro the minimap
	{

		std::unique_lock<std::mutex> lock(windowEditor_);
		// get the current desktop
		sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

		// This is the fixed tile size of the ground tiles
		const int tilesize = TILESIZE;
		int viewsize = tilesize * world_->GetMapDimension();

		// Create the current camera position outline
		sf::RectangleShape outline(main_view.getSize());
		outline.setOrigin(currentResolution.width / 2.0f, currentResolution.height / 2.0f);
		outline.setOutlineThickness(20.0f);
		outline.setFillColor(sf::Color::Transparent);
		outline.setOutlineColor(sf::Color::Black);
		outline.setPosition(main_view.getCenter());

		window.draw(outline);

		// Create the border around the minimap
		sf::RectangleShape map_outline(sf::Vector2f(viewsize, viewsize));
		map_outline.setOrigin(viewsize / 2.0f, viewsize / 2.0f);
		map_outline.setOutlineThickness(-128.0);
		map_outline.setFillColor(sf::Color::Transparent);
		map_outline.setOutlineColor(sf::Color::Black);
		map_outline.setPosition(viewsize / 2.0f, viewsize / 2.0f);

		window.draw(map_outline);

	}

	// END OF FRAME ALL RENDERING COMPLETE
	FrameEndBarrier->wait();

}


void BattleScene::ProcessUnits()
{

	while (play)
	{

		// WAIT FOR DT TO BE SET
		ProcessBarrier->wait();

		// Proecess the world, units and projectiles
		units_->Update(delta_time, *spriteRenderer_);
		projectiles_->Update(delta_time, *spriteRenderer_);

		// ALL PROCESSING COMPLETE
		RenderBarrier->wait();

		// RENDERING TO THE SCREEN

		// END OF FRAME ALL RENDERING COMPLETE
		FrameEndBarrier->wait();

	}

}


void BattleScene::SpawnAttack(Projectile* attack)
{

	instance_->projectiles_->AddObject(attack);

}


void BattleScene::SetUpViewWindows()
{

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// Main
	main_view.setSize(currentResolution.width, currentResolution.height);
	main_view.setCenter(TILESIZE * world_->GetMapDimension()/ 2.0f, TILESIZE * world_->GetMapDimension() / 2.0f);
	main_view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

	// MiniMap

	// This is the fixed tile size of the ground tiles
	const int tilesize = TILESIZE;
	const int mapSize_dimensions = world_->GetMapDimension();
	int viewsize = tilesize * (mapSize_dimensions + 2); // 2 accounts for the border

	// restict the view port to the bottom left of the screen with a border
	miniMap_View.setSize(viewsize, viewsize);
	miniMap_View.setCenter(viewsize / 2.0f, viewsize / 2.0f);
	miniMap_View.setViewport(sf::FloatRect(0.74, 0.74, 0.24, 0.24));

}
