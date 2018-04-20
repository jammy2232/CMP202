#include "Projectile.h"

std::list<Projectile*> Projectile::projectiles;
std::mutex Projectile::project;
UnitWorld* Projectile::world_ = nullptr;
RenderManager* Projectile::render = new RenderManager(500, "Textures/medievalRTS_spritesheet");

Projectile::Projectile(sf::Vector2f Start, sf::Vector2f End, sf::Vector2i TargetTile)
{

	// Set the starting location
	currentInfo.screen_position = Start;

	// Calculate the direction to be applied 
	direction = (End - Start);

	// Normalise it
	direction = direction/(std::sqrtf(std::powf(direction.x, 2.0f) + std::powf(direction.y, 2.0f)));

	// set the destination
	destination = End;
	destinationTile = TargetTile;

	// set the sprite 
	currentInfo.id = 9;

	// add this to the renderer
	id = render->AddEntity(currentInfo);

}


Projectile::~Projectile()
{
}


void Projectile::SpawnProjectile(sf::Vector2f Start, sf::Vector2f End, sf::Vector2i TargetTile)
{

	// Create a new projectile 
	Projectile* proj = new Projectile(Start, End, TargetTile);

	// Project access to the list
	std::unique_lock<std::mutex> lock(project);

	// Add this to the list 
	projectiles.push_back(proj);

}


void Projectile::Render(sf::RenderWindow & window)
{

	for (int arrow = 0; arrow < render->getNumberOfEntities(); arrow++)
	{
		window.draw(render->RenderSprite(arrow));
	}

}


void Projectile::Update(float dt)
{


	// Project access to the list
	std::unique_lock<std::mutex> lock(project);

	// update all the units
	for (auto arrows : projectiles)
	{

		// Update the state
		if (arrows->active)
		{
			arrows->UpdateArrow(dt);

			// update hte renderer
			render->UpdateEntity(arrows->getId(), arrows->GetSpriteInfo());
		}

		if (!arrows->active)
		{
			// projectiles.remove(arrows);
			// delete arrows;

		}

	}

}


int Projectile::getId()
{
	return id;
}


void Projectile::UpdateArrow(float dt)
{

	// update the position
	std::unique_lock<std::mutex> lock(info);

	// Move the unit in the direction of travel
	currentInfo.screen_position = currentInfo.screen_position + direction * PROJECTILESPEED * dt;

	// whats the current vector direction
	sf::Vector2f currentDirection = destination - currentInfo.screen_position;

	// Check if thedot product to see the unit is passed the point 
	if ((direction.x * currentDirection.x + direction.y * currentDirection.y) < 0.0f)
	{

		// the unit is pasted the intended position so therefore set the position
	//	Unit* tileInfo = world_GetUnitInfo(destinationTile);

		if (tileInfo)
		{
			tileInfo->Damage(20.0f);
		}

		active = false;

	}

}

RenderObject Projectile::GetSpriteInfo()
{

	// update the position
	std::unique_lock<std::mutex> lock(info);
	return currentInfo;

}
