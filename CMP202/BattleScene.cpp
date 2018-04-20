#include "BattleScene.h"


BattleScene::BattleScene()
{}

BattleScene::~BattleScene()
{
}

bool BattleScene::Init()
{

	// Create the world 
	world_ = new GameWorld(100);

	// Start the pathfinder thread
	pathfinder = new PathFinder(world_->GetStaticPathfinderMap(), world_->GetMapDimension());

	// Setup the view windows for main and minimaps
	// SetUpViewWindows();

	// Get the map random map information (exluding extention - expects .xml and .png to have the same name)
	spriteRenderer_ = new SpriteRenderer("Textures/medievalRTS_spritesheet");

	// Setup the gameobject list
	units_ = new GameObjectManager(200, *world_);
	projectiles_ = new GameObjectManager(200, *world_);

	// Set the unit world
	// Projectile::SetWorld(unitsWorld_);

	// variable for team selecetion
	Unit::TEAM teamSelection = Unit::TEAM::BLUE;

	// Spawn Units in random locations
	for (int i = 0; i < 200; i++)
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
		Unit* unit = (Unit*)units_->SpawnObject(Unit(tile, teamSelection));

		// Set the initial state
		unit->ChangeState(*world_, new SearchAndDestoy(*unit));

		// Create a new unit
		world_->SetUnitOnTile(unit, tile);

	}

	// return to show error free;
	return true;

}


void BattleScene::CleanUp()
{

	delete world_;
	world_ = nullptr;

	delete spriteRenderer_;
	spriteRenderer_ = nullptr;

	delete units_;
	units_ = nullptr;

}


void BattleScene::HandleInput(float delta_time)
{
	// Scrollable keyboard controls 

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		main_view.move(-300.0f * delta_time, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		main_view.move(300.0f * delta_time, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		main_view.move(0.0f, -300.0f * delta_time);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		main_view.move(0.0f, 300.0f * delta_time);
	}

}


void BattleScene::Update(float delta_time)
{

	// Prepare the unit update list
	units_->PreProcessing();

	// Update the units
	units_->Update(delta_time, *spriteRenderer_);

	// Barrier here 

	// Prepare the unit update list
	projectiles_->PreProcessing();

	// Update the Projectiles
	projectiles_->Update(delta_time, *spriteRenderer_);

}


void BattleScene::Render(sf::RenderWindow & window)
{

	// Main Render Function **********************
	//{
	//	std::unique_lock<std::mutex> lock(windowEditor_);
	//	window.setView(main_view);
	//}

	// Render World
	spriteRenderer_->RenderFrame(window);

	// Render Units
	// spriteRenderer_->RenderFrame(window);

	// Render Arrows
	// spriteRenderer_->RenderFrame(window);


}


void BattleScene::RenderUI(sf::RenderWindow & window)
{

	/*

	//Minimap render function ***********************
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		window.setView(miniMap_View);
	}

	// Render Tiles 
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		for (int tile = 0; tile < tileMapRenderer->getNumberOfEntities(); tile++)
		{
			window.draw(tileMapRenderer->RenderSprite(tile));
		}
	}

	// Render Units
	/*
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		for (int unit = 0; unit < unitRenderer->getNumberOfEntities(); unit++)
		{
			window.draw(unitRenderer->RenderSprite(unit));
		}
	}
	

	// Render the borders fro the minimap
	{

		std::unique_lock<std::mutex> lock(windowEditor_);
		// get the current desktop
		sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

		// This is the fixed tile size of the ground tiles
		const int tilesize = TILESIZE;
		int viewsize = tilesize * mapDimension;

		// Create the current camera position outline
		sf::RectangleShape outline(sf::Vector2f(currentResolution.width, currentResolution.height));
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

	*/

}


void BattleScene::SetUpViewWindows()
{

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// Main
	main_view.setSize(currentResolution.width, currentResolution.height);
	main_view.setCenter(currentResolution.width / 2.0f, currentResolution.height / 2.0f);
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
