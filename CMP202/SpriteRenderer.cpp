#include "SpriteRenderer.h"



SpriteRenderer::SpriteRenderer(std::string filename)
{

	// Load the Sprite sheet and symbols for generating the games graphics
	if (!spriteSheet_.loadFromFile(filename + ".png"))
	{
		assert(false);
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
		assert(false);
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

}


SpriteRenderer::~SpriteRenderer()
{
}


// Adds a object to be rendered in the frame
void SpriteRenderer::DrawSprite(SpriteObject& sprite)
{

	// Lock the queue for thread safety
	std::unique_lock<std::mutex> lock(renderQueueLock_);

	// 
	assert(sprite.id > -1 && sprite.id < 200);

	// Add the object to be rendered
	renderQueue_.push(sprite);

}


// Renders all the objects in the queue
void SpriteRenderer::RenderFrame(sf::RenderWindow & window)
{

	// Lock the queue for thread safety
	std::unique_lock<std::mutex> lock(renderQueueLock_);

	// Draw Objects to the render target until the queue is empty
	while (!renderQueue_.empty())
	{

		// get the front of the queue
		SpriteObject current = renderQueue_.front();

		// Update the sprite 
		sprites_[current.id].setPosition(current.screen_position);

		// Render this to the window
		window.draw(sprites_[current.id]);

		// Remove the drawn sprite from the queue
		renderQueue_.pop();

	}

}
