#pragma once

// std
#include "stdint.h"
#include <queue>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <atomic>

// Game inclides
#include "SFML\System\Vector2.hpp"
#include "Unit.h"
#include "Semaphor.h"
#include "GameSettings.h"

// Forward declaration
struct Cell;
struct Task;
class Unit;

class PathFinder
{
public:

	PathFinder(const std::vector<bool> baseTileMap, int MapDimensions);
	~PathFinder();

	static void RequestPath(Unit* unit, sf::Vector2i start, sf::Vector2i destination);
	std::atomic<bool> required = false;
	std::atomic<bool> active = true;

private:

	// Pathfiding 
	std::list<sf::Vector2i> findPath(sf::Vector2i Start, sf::Vector2i Finish);
	int CalculateHeuristic(sf::Vector2i Start, sf::Vector2i Finish);
	void Reset();
	Cell* GetCell(sf::Vector2i indexLocation);
	const std::vector<bool> baseTileMap_;
	std::vector<Cell> cellMap_;
	int mapDimension_;

	sf::Vector2i neighboroughs[8] =
	{
		sf::Vector2i(-1,-1),
		sf::Vector2i(0,-1),
		sf::Vector2i(1,-1),
		sf::Vector2i(-1, 0),
		sf::Vector2i(1, 0),
		sf::Vector2i(-1, 1),
		sf::Vector2i(0, 1),
		sf::Vector2i(1, 1)
	};

	// Task Management
	static std::queue<Task> taskQueue_;
	Task GetTask();

	// Thread and Access Management 
	void Work();
	std::thread* worker_;
	static Semaphor requests_;
	static std::mutex queue;

};


