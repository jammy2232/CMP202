#pragma once

#include <list>
#include <mutex>
#include "SFML\System\Vector2.hpp"
#include "RenderManager.h"
#include "UnitWorld.h"
#include "Unit.h"
#include <atomic>
// SFML
#include "SFML\Graphics.hpp"

class UnitWorld;

class Projectile
{

public:

	Projectile(sf::Vector2f Start, sf::Vector2f End, sf::Vector2i TargetTile);
	~Projectile();

	static void SpawnProjectile(sf::Vector2f Start, sf::Vector2f End, sf::Vector2i TargetTile);
	static void SetWorld(UnitWorld* world) { world_ = world; }
	static void Render(sf::RenderWindow& window);
	static void Update(float dt);

	void UpdateArrow(float dt);
	int getId();
	RenderObject GetSpriteInfo();


private:

	static std::list<Projectile*> projectiles;
	static std::mutex project;
	static UnitWorld* world_;
	static RenderManager* render;

	// Start and end locations 
	RenderObject currentInfo;
	std::mutex info;
	sf::Vector2f direction;
	sf::Vector2f destination;
	sf::Vector2i destinationTile;
	std::atomic<int> id = 0;

	// bools for control
	std::atomic<bool> active = true;

};

