#pragma once

// std
#include "stdint.h"
#include <queue>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include "Semaphor.h"
#include "GameSettings.h"

#include "Coordsi.h"
#include "Unit.h"


struct Cell
{

	enum INFO { BASIC, BLOCKED, OPEN, CLOSED };

	// This is the location of the cell on the world grid
	Coordsi location;
	// This is to determine the movement cost based on type
	INFO state = BASIC;

	// Values for calucluating the A* Algorithm
	int f_cost = INT32_MAX;
	int g_cost = INT32_MAX;
	Cell* parent = nullptr;

};


class cellCompare
{
public:

	bool operator() (const Cell& left, const Cell& right) const
	{
		return left.f_cost > right.f_cost;
	}

};


// Forward declaration
class Unit;

struct Task
{
	Unit* unit = nullptr;
	Coordsi start;
	Coordsi destination;
};


class PathFinder
{
public:

	PathFinder(const std::vector<bool> baseTileMap);
	~PathFinder();

	static void RequestPath(Unit* unit, Coordsi start, Coordsi destination);

private:

	std::list<Coordsi> findPath(Coordsi Start, Coordsi Finish);
	void Reset();
	void Work();

	static std::queue<Task> taskQueue_;
	const std::vector<bool> baseTileMap_;
	Cell* GetCell(Coordsi indexLocation);
	int CalculateHeuristic(Coordsi Start, Coordsi Finish);
	std::vector<Cell> cellMap_;
	Task GetTask();

	std::thread* worker_;
	static Semaphor requests_;
	static std::mutex queue;

	bool required = false;

	Coordsi neighboroughs[8] =
	{
		Coordsi(-1,-1),
		Coordsi(0,-1),
		Coordsi(1,-1),
		Coordsi(-1, 0),
		Coordsi(1, 0),
		Coordsi(-1, 1),
		Coordsi(0, 1),
		Coordsi(1, 1)
	};

};


