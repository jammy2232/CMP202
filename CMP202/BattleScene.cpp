#include "BattleScene.h"


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

	// Randomly Generate a map
	for (int i = 0; i < 10000; i++)
	{
		renderMap_.push_back(100 + rand()%2);
	}

	// Setup the view windows for main and minimaps
	SetUpViewWindows();

	// Flag that the initailsation has been complete
	loaded = true;

	// return to show error free;
	return true;

}

void BattleScene::CleanUp()
{

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

	// 


}


void BattleScene::Render(sf::RenderWindow & window)
{

	// Main Render Function **********************

	window.setView(main_view);

	// Render Tiles 

	// Render Units

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			sprites_[renderMap_[y * 100 + x]].setPosition(x * 64, y * 64);
			window.draw(sprites_[renderMap_[y * 100 + x]]);
		}
	}


	//Minimap render function ***********************

	window.setView(miniMap_View);

	// Render Map BackGround

	// test
	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			sprites_[renderMap_[y * 100 + x]].setPosition(x * 64, y * 64);
			window.draw(sprites_[renderMap_[y * 100 + x]]);
		}
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
	const int tilesize = 64.0f;
	const int mapSize_dimensions = 100;
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
	const int tilesize = 64.0f;
	const int mapSize_dimensions = 100;
	int viewsize = tilesize * (mapSize_dimensions + 2); // 2 accounts for the border

	// restict the view port to the bottom left of the screen with a border
	miniMap_View.setSize(viewsize, viewsize);
	miniMap_View.setCenter(viewsize / 2.0f, viewsize / 2.0f);
	miniMap_View.setViewport(sf::FloatRect(0.74, 0.74, 0.24, 0.24));

}