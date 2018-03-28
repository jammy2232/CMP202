#include "BattleScene.h"

#include "Moving.h"

BattleScene::BattleScene()
{

	// This is not for loading 
	loading = false;
	unloaded = false;

}

BattleScene::~BattleScene()
{
}

bool BattleScene::Init()
{

	// Load the texture data ***************
	// Load the Sprite sheet and symbols for generating the games graphics
	if (!spriteSheet_.loadFromFile("Textures/medievalRTS_spritesheet.png"))
	{
		return false;
	}

	// Load the sprite data ***************
	if (!LoadSpriteSheet())
	{
		return false;
	}

	// Create a map to display
	TileMapGenerator();

	// Setup the view windows for main and minimaps
	SetUpViewWindows();

	// Start the pathfinder
	pathfinder = new PathFinder(tileMapStatus_);

	// Spawn Units
	for (int i = 0; i < UNITS; i++)
	{

		Unit* newUnit = new Unit();
		Coordsi coordinates;

		do
		{
			coordinates = Coordsi(rand() % (MAPDIMENSIONS - 1), rand() % (MAPDIMENSIONS - 1));
			newUnit->currentTile = coordinates;
			newUnit->currentPosition = Coordsf(newUnit->currentTile.x * TILESIZE, newUnit->currentTile.y * TILESIZE);
			newUnit->forwardDirection = Coordsf(0.0f, 0.0f);
		} while (!tileMapStatus_[coordinates.y * MAPDIMENSIONS + coordinates.x]);

		SetTileOccupancy(newUnit->currentTile, true);
		units_.push_back(newUnit);

	}
	
	// Calculate the paths
	for (auto unit : units_)
	{
	// 	int randx = rand() % (MAPDIMENSIONS - 1);
	//	int randy = rand() % (MAPDIMENSIONS - 1);
		PathFinder::RequestPath(unit, unit->currentTile , Coordsi(MAPDIMENSIONS - 1, MAPDIMENSIONS - 1));
		unit->ChangeState(Moving::stateInstance);
	}

	// Flag that the initailsation has been complete
	loaded = true;

	// return to show error free;
	return true;

}

void BattleScene::CleanUp()
{

	delete pathfinder;
	pathfinder = nullptr;

	unloaded = true;

}

Scene * BattleScene::transition()
{
	return nullptr;
}

void BattleScene::HandleInput(float delta_time)
{
	// Scrollable keyboard controls 

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		main_view.move(-300.0f * delta_time, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		main_view.move(300.0f * delta_time, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		main_view.move(0.0f, -300.0f * delta_time);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		main_view.move(0.0f, 300.0f * delta_time);
	}

	// Scrollable mouse controls

}

void BattleScene::Update(float delta_time)
{

	// update all the units
	for (auto unit : units_)
	{
	 	unit->Update(delta_time);
	}

}


void BattleScene::Render(sf::RenderWindow & window)
{

	// Main Render Function **********************

	window.setView(main_view);

	// Render Tiles 

	// Render Units

	for (int y = 0; y < MAPDIMENSIONS; y++)
	{
		for (int x = 0; x < MAPDIMENSIONS; x++)
		{
			sprites_[renderMap_[y * MAPDIMENSIONS + x]].setPosition(x * TILESIZE, y * TILESIZE);
			window.draw(sprites_[renderMap_[y * MAPDIMENSIONS + x]]);
		}
	}

	for (auto unit : units_)
	{		
		sprites_[unit->spriteId_].setPosition(unit->currentPosition.x, unit->currentPosition.y);
		window.draw(sprites_[unit->spriteId_]);
	}


	//Minimap render function ***********************

	window.setView(miniMap_View);

	// Render Map BackGround

	// test
	for (int y = 0; y < MAPDIMENSIONS; y++)
	{
		for (int x = 0; x < MAPDIMENSIONS; x++)
		{
			sprites_[renderMap_[y * MAPDIMENSIONS + x]].setPosition(x * TILESIZE, y * TILESIZE);
			window.draw(sprites_[renderMap_[y * MAPDIMENSIONS + x]]);
		}
	}

	for (auto unit : units_)
	{
		sprites_[unit->spriteId_].setPosition(unit->currentPosition.x, unit->currentPosition.y);
		window.draw(sprites_[unit->spriteId_]);
	}

}


void BattleScene::RenderUI(sf::RenderWindow & window)
{

	// Render Geometry Symbols

	// Render the Map Unity Symbols

	// Render Current View Box

	// get the current desktop
	sf::VideoMode currentResolution = sf::VideoMode::getDesktopMode();

	// This is the fixed tile size of the ground tiles
	const int tilesize = TILESIZE;
	const int mapSize_dimensions = MAPDIMENSIONS;
	int viewsize = tilesize * mapSize_dimensions;

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


bool BattleScene::LoadSpriteSheet()
{

	// hold the raw xml text
	std::vector<char> xmlDocText;
	std::ifstream xmlFile;

	// open the file and check for success
	xmlFile.open("Textures/medievalRTS_spritesheet.xml", std::fstream::in);

	// error check
	if (xmlFile.fail())
	{
		return false;
	}

	// read in all the information from the document for parsing
	char c;
	while (xmlFile.get(c))
	{
		xmlDocText.push_back(c);
	}
	xmlDocText.push_back(NULL); // null terminate the data

	// Load in the XML that will indicate each positon of the sprites
	rapidxml::xml_document<> spiteSheetSettings;
	spiteSheetSettings.parse<0>(xmlDocText.data());

	// For each of the textures in the sprite sheet create a sprite to be accessed via an index
	rapidxml::xml_node<>* atlas = spiteSheetSettings.first_node();
	rapidxml::xml_node<>* sprite = atlas->first_node();

	// Go thourgh each and extract the information
	while (sprite != nullptr)
	{

		// create a new sprite object
		sf::Sprite newSprite;

		// set the texture
		newSprite.setTexture(spriteSheet_);

		// Set the texture to be a subset of this
		sf::Rect<int> textureRect;

		// extract the information from the sprite sheet info (unrolled) 
		// x, y, width and height are specific for this xml
		rapidxml::xml_attribute<>* attrib = sprite->first_attribute()->next_attribute();
		textureRect.left = std::stoi(attrib->value());			// x
		attrib = attrib->next_attribute();
		textureRect.top = std::stoi(attrib->value());			// y
		attrib = attrib->next_attribute();
		textureRect.width = std::stoi(attrib->value());			// width
		attrib = attrib->next_attribute();
		textureRect.height = std::stoi(attrib->value());		// height

		// set the texture rect
		newSprite.setTextureRect(textureRect);

		// All the tiles will be based on drawing from the centre
		newSprite.setOrigin(textureRect.width / 2, textureRect.height / 2);

		// store the sprite
		sprites_.push_back(newSprite);

		// move to the next node
		sprite = sprite->next_sibling();

	}

	// success
	return true;

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
	const int mapSize_dimensions = MAPDIMENSIONS;
	int viewsize = tilesize * (mapSize_dimensions + 2); // 2 accounts for the border

	// restict the view port to the bottom left of the screen with a border
	miniMap_View.setSize(viewsize, viewsize);
	miniMap_View.setCenter(viewsize / 2.0f, viewsize / 2.0f);
	miniMap_View.setViewport(sf::FloatRect(0.74, 0.74, 0.24, 0.24));

}


void BattleScene::TileMapGenerator()
{

	// Create a tile occupancy map
	for (int i = 0; i < (MAPDIMENSIONS * MAPDIMENSIONS); i++)
	{
		currentMapStatus_.push_back(false);
	}

	// Create a square map with Random Grass textures
	for (int i = 0; i < (MAPDIMENSIONS * MAPDIMENSIONS); i++)
	{

		// Get a random texture (2 possible 100 or 101)
		int randomGrassTextureId = 100 + rand() % 2;
		// Store this in the map location 
		renderMap_.push_back(randomGrassTextureId);
		// Assume all tiles are walkable
		tileMapStatus_.push_back(true);

	}

	// Add random trees and foliage on 10% of the map
	for (int i = 0; i < (MAPDIMENSIONS * MAPDIMENSIONS * 0.3f); i++)
	{

		// Get a Random Tile ID
		int randomTileId_x = rand() % MAPDIMENSIONS;
		int randomTileId_y = rand() % MAPDIMENSIONS;

		// Get a random Tree tile 85 to 92 on the sprite sheet
		int randomTreeTile = 85 + rand() % 7;

		// Store this in the map location 
		renderMap_[randomTileId_y * MAPDIMENSIONS + randomTileId_x] = randomTreeTile;
		// Areas are not walkable
		tileMapStatus_[randomTileId_y * MAPDIMENSIONS + randomTileId_x] = false;

	}

	// Add a river down the middle with a crossing point
	for (int y = 0; y < (MAPDIMENSIONS); y++)
	{

		for (int x = 0; x < (MAPDIMENSIONS); x++)
		{

			// Check if you create Sand
			if (x == MAPDIMENSIONS * 9 / 20 || x == MAPDIMENSIONS * 11 / 20)
			{
				if (y > MAPDIMENSIONS * 9 / 20 && y < MAPDIMENSIONS * 11 / 20)
				{
					// Place Bridge
					renderMap_[y * MAPDIMENSIONS + x] = 57;
					// Areas are not walkable
					tileMapStatus_[y * MAPDIMENSIONS + x] = true;
				}
				else
				{
					// Place Beach
					renderMap_[y * MAPDIMENSIONS + x] = 45;
					// Areas are not walkable
					tileMapStatus_[y * MAPDIMENSIONS + x] = true;
				}
			}

			// Check if you create Water
			if (x > MAPDIMENSIONS * 9 / 20 && x < MAPDIMENSIONS * 11 / 20)
			{
				if (y > MAPDIMENSIONS * 9 / 20 && y < MAPDIMENSIONS * 11 / 20)
				{
					// Place Bridge
					renderMap_[y * MAPDIMENSIONS + x] = 57;
					// Areas are not walkable
					tileMapStatus_[y * MAPDIMENSIONS + x] = true;
				}
				else
				{
					// Place Water
					renderMap_[y * MAPDIMENSIONS + x] = 71;
					// Areas are not walkable
					tileMapStatus_[y * MAPDIMENSIONS + x] = false;
				}

			}

		}

	}

}


bool BattleScene::CheckIfTileIsOccupied(Coordsi request)
{

	std::unique_lock<std::mutex> lock(currentMapStatusLock_);
	return currentMapStatus_[request.y * MAPDIMENSIONS + request.x];

}


void BattleScene::SetTileOccupancy(Coordsi request, bool occupancy)
{

	std::unique_lock<std::mutex> lock(currentMapStatusLock_);
	currentMapStatus_[request.y * MAPDIMENSIONS + request.x] = occupancy;

}


Unit * BattleScene::CheckForUnit(Coordsi location, int range, TEAM team)
{
	/*
	float minDistance;

	// Check in the radius around the unit looking
	for (int y = (location.y - range); y < (location.y + range); y++)
	{
		for (int x = (location.x - range); x < (location.x + range); x++)
		{




		}
	}


	*/

	// No units exist
	return nullptr;

}
