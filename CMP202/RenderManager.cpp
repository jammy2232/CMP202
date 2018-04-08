#include "RenderManager.h"


RenderManager::RenderManager(unsigned int maxEntities, std::string filename) : maxEntities_(maxEntities), totalEntities_(0)
{

	// Check the max entities is valid
	assert(maxEntities > 0);

	// Preallocate space for the entities
	entities_.assign(maxEntities, RenderObject());

	// Load the Spritesheet
	assert(CreateSpriteSheet(filename));

}


RenderManager::~RenderManager()
{

	// Clean up via destruction
	sprites_.clear();
	entities_.clear();

}


bool RenderManager::CreateSpriteSheet(std::string filename)
{
	
	// Load the Sprite sheet and symbols for generating the games graphics
	if (!spriteSheet_.loadFromFile(filename + ".png"))
	{
		return false;
	}

	 // From the XML sprite atlas get all the required information and generate the sprites for rendering

	// Setup the raw XML docuement
	std::vector<char> xmlDocText;
	std::ifstream xmlFile;

	// open the file and check for success
	xmlFile.open(filename + ".xml", std::fstream::in);

	// if this failed flag an error
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


int RenderManager::AddEntity(RenderObject object)
{

	// Lock access
	std::unique_lock<std::mutex> size(sizeCheck);


	// Check if another entity can be added
	if (totalEntities_ == maxEntities_)
	{
		// not allowed max limit breached
		return -1;
	}

	// Lock access
	std::unique_lock<std::mutex> entity(entityAccessor);

	// Add the entity 
	entities_[totalEntities_] = object;

	// increment the totalEntities
	totalEntities_++;

	// return the index 
	return totalEntities_ - 1;

}


int RenderManager::getNumberOfEntities()
{	

	// Lock access
	std::unique_lock<std::mutex> size(sizeCheck);
	std::unique_lock<std::mutex> entity(entityAccessor);

	// return if the max size is reached
	return totalEntities_;

}


void RenderManager::UpdateEntity(int index, RenderObject& object)
{

	// Lock access
	std::unique_lock<std::mutex> entity(entityAccessor);

	// Get the renderobject information
	entities_[index] = object;

}


const sf::Sprite& RenderManager::RenderSprite(int index)
{

	// Lock access
	std::unique_lock<std::mutex> entity(entityAccessor);

	// Update the sprite 
	sprites_[entities_[index].id].setPosition(entities_[index].x, entities_[index].y);

	// return access for the sprite to be drawn
	return sprites_[entities_[index].id];

}
