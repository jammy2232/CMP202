#pragma once

// SFML includes
#include "SFML\Graphics.hpp"

// std includes
#include <string>
#include <mutex>

// XML Parsing
#include "rapidxml.hpp"
#include <fstream>


// Struct for storing only information regarding rendering
struct RenderObject
{

	RenderObject(sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), int spriteId_ = -1): screen_position(position), id(spriteId_) {}

	// objects location
	sf::Vector2f screen_position;

	// sprite Id (-1 is the default to draw nothing)
	int id = -1;

};


class RenderManager
{

public:

	RenderManager(unsigned int maxEntities, std::string filename);
	~RenderManager();

	// ThreadSafeAccess
	int AddEntity(RenderObject object);
	void UpdateEntity(int index, RenderObject& object);
	int getNumberOfEntities();

	// Sprite Render
	const sf::Sprite& RenderSprite(int index);

private:

	// Load and create sprite atlas
	bool CreateSpriteSheet(std::string filename);

	// Sprite/Texture atlas for rendering all ingame entities
	sf::Texture spriteSheet_;
	std::vector<sf::Sprite> sprites_;

	// Controlled list for interfacing with game objects and rendering
	std::vector<RenderObject> entities_;

	// Thread safe access control
	std::mutex entityAccessor;
	std::mutex sizeCheck;

	// intenal control for max numberof entities
	unsigned int maxEntities_;
	unsigned int totalEntities_;

};

