#include "BattleScene.h"


BattleScene::BattleScene()
{}

BattleScene::~BattleScene()
{
}

bool BattleScene::Init()
{

	// Create a map to display for the battle
	MapGenerator newMap(mapDimension);

	// Start the pathfinder thread
	pathfinder = new PathFinder(newMap.GetStaticMapData(), mapDimension);
	pathfinder1 = new PathFinder(newMap.GetStaticMapData(), mapDimension);

	// Setup the view windows for main and minimaps
	SetUpViewWindows();

	// Get the map random map information (exluding extention - expects .xml and .png to have the same name)
	tileMapRenderer = newMap.CreateRenderSystem("Textures/medievalRTS_spritesheet");

	// create a render manager for the units
	unitRenderer = new RenderManager(maxUnits, "Textures/medievalRTS_spritesheet");

	// Spawn units on the map 
	unitsWorld_ = new UnitWorld(mapDimension, maxUnits, newMap.GetStaticMapData(), *unitRenderer);

	// Get a reference to all the created units
	units_ = unitsWorld_->GetUnitList();

	// Set the unit world
	Projectile::SetWorld(unitsWorld_);

	// return to show error free;
	return true;

}


void BattleScene::CleanUp()
{

	delete pathfinder;
	pathfinder = nullptr;

	delete tileMapRenderer;
	tileMapRenderer = nullptr;

	delete unitRenderer;
	unitRenderer = nullptr;

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

	// update all the units
	for (auto unit : units_)
	{

		// Update the state
		unit->UpdateState(delta_time);

		// check there is a valid unit to process
		if (unit)
		{

			//if the units has moved on the map update the render map
			if (unit->posDirty_)
			{
				unitRenderer->UpdateEntity(unit->GetEntityId(), unit->GetSpriteInfo());
				unit->posDirty_ = false;
			}

		}

	}


	Projectile::Update(delta_time);

}


void BattleScene::Render(sf::RenderWindow & window)
{

	// Main Render Function **********************
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		window.setView(main_view);
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
	{
		std::unique_lock<std::mutex> lock(windowEditor_);
		for (int unit = 0; unit < unitRenderer->getNumberOfEntities(); unit++)
		{
			window.draw(unitRenderer->RenderSprite(unit));
		}
	}

	// Render Arrows
	 {
	 	std::unique_lock<std::mutex> lock(windowEditor_);
		Projectile::Render(window);
	}


}


void BattleScene::RenderUI(sf::RenderWindow & window)
{

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
	*/

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
	const int mapSize_dimensions = mapDimension;
	int viewsize = tilesize * (mapSize_dimensions + 2); // 2 accounts for the border

	// restict the view port to the bottom left of the screen with a border
	miniMap_View.setSize(viewsize, viewsize);
	miniMap_View.setCenter(viewsize / 2.0f, viewsize / 2.0f);
	miniMap_View.setViewport(sf::FloatRect(0.74, 0.74, 0.24, 0.24));

}
