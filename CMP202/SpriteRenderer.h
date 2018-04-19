#pragma once

// specific SFML Includes 
#include "SFML\Graphics.hpp"
#include "SFML\System\Vector2.hpp"

// Sprite Sheet reading via XML Parsing
#include "rapidxml.hpp"
#include <fstream>

// std includes
#include <string>
#include <mutex>
#include <queue>
#include <vector>

// Struct for storing only information regarding rendering
struct SpriteObject
{

	SpriteObject(sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), int spriteId_ = -1) : screen_position(position), id(spriteId_) {}

	// objects location
	sf::Vector2f screen_position;

	// sprite Id (-1 is the default to draw nothing)
	int id = -1;

};


// This class will queue items to be drawn from a sprite sheet during the render phase
class SpriteRenderer
{

public:

	SpriteRenderer(std::string filename = "");
	~SpriteRenderer();

	// Thread safe function to add something to be drawn in the frame
	void DrawSprite(SpriteObject& object);

	// Thread safe function to render all the objects in the queue
	void RenderFrame(sf::RenderWindow& window);

private:

	// Sprite/Texture atlas for rendering all ingame entities
	sf::Texture spriteSheet_;
	std::vector<sf::Sprite> sprites_;

	// Queue to store all the sprites to be drawn in the frame
	std::queue<SpriteObject> renderQueue_;
	// Mutex to protect access to the render queueu
	std::mutex renderQueueLock_;

};

