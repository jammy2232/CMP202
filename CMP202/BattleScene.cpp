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

	// Setup the view windows for main and minimaps
	SetUpViewWindows();

	// Get the map random map information (exluding extention - expects .xml and .png to have the same name)
	tileMapRenderer = newMap.CreateRenderSystem("Textures/medievalRTS_spritesheet");

	// create a render manager for the units
	unitRenderer = new RenderManager(maxUnits, "Textures/medievalRTS_spritesheet");

	// Populate the unit map
	units_.assign(mapDimension*mapDimension, nullptr);

	// bool for team selecetion
	bool team = false;

	// Spawn Units in random locations
	for (int i = 0; i < maxUnits; i++)
	{

		// Create a new unit
		Unit* newUnit = new Unit(units_, mapDimension);

		// set the initials state
		newUnit->SetInitialState(new SearchAndDestoy(newUnit));

		// Find a valid spawn location for the unity
		sf::Vector2i coordinates;

		// randomly select a starting value
		coordinates = sf::Vector2i(rand() % (mapDimension - 1), rand() % (mapDimension - 1));

		bool test1;
		bool test2;

		while (!(newMap.GetStaticMapData()[coordinates.y * mapDimension + coordinates.x] && units_[coordinates.y * mapDimension + coordinates.x] == nullptr))
		{
			coordinates = sf::Vector2i(rand() % (mapDimension - 1), rand() % (mapDimension - 1));

			// randomly select a starting value
			test1 = units_[coordinates.y * mapDimension + coordinates.x] == nullptr;
			test2 = newMap.GetStaticMapData()[coordinates.y * mapDimension + coordinates.x] == true;

		} 

		bool test3 = units_[coordinates.y * mapDimension + coordinates.x] == nullptr;

		// Apply the details to the new unit
		newUnit->currentTile = coordinates;

		if (team)
		{
			newUnit->team = Unit::TEAM::BLUE;
			team = false;
		}
		else
		{
			newUnit->team = Unit::TEAM::RED;
			team = true;
		}
		
		newUnit->spriteInfo.id = (int)newUnit->team;
		newUnit->spriteInfo.x = (float)newUnit->currentTile.x * TILESIZE;
		newUnit->spriteInfo.y = (float)newUnit->currentTile.y * TILESIZE;
		
		// Place the unit on the map 
		assert(units_[coordinates.y * mapDimension + coordinates.x] == nullptr);
		units_[newUnit->currentTile.y * mapDimension + newUnit->currentTile.x] = newUnit;

		// initial sync with the renderer 
		newUnit->entityId = unitRenderer->AddEntity(newUnit->spriteInfo);

	}

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

		// check there is a valid unit to process
		if (unit)
		{

			unit->UpdateState(delta_time);

			//if the units has moved on the map update the render map
			if (unit->posDirty_)
			{
				unitRenderer->UpdateEntity(unit->entityId, unit->spriteInfo);
				unit->posDirty_ = false;
			}

		}

	}

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
