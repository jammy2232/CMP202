#include "PathFinder.h"

#include <assert.h>


// structures required for pathding

struct Cell
{

	enum INFO { BASIC, BLOCKED, OPEN, CLOSED };

	// This is the location of the cell on the world grid
	sf::Vector2i location;
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


struct Task
{
	Unit* unit = nullptr;
	sf::Vector2i start;
	sf::Vector2i destination;
};


// Private static delcarations
Semaphor PathFinder::requests_;
std::mutex PathFinder::queue;
std::queue<Task> PathFinder::taskQueue_;


PathFinder::PathFinder(const std::vector<bool> baseTileMap, int MapDimensions) : baseTileMap_(baseTileMap), mapDimension_(MapDimensions)
{

	// Initialise the internal map
	Reset();

	//Start
	required = true;

	// Initialise the thread
	worker_ = new std::thread(&PathFinder::Work, this);
	worker_->detach();

}


PathFinder::~PathFinder()
{

	// shutdown the worker thread
	required = false;

}


void PathFinder::RequestPath(Unit* unit, sf::Vector2i start, sf::Vector2i destination)
{

	// Lock the queue (as more than one pathfinding worker thread can exist)
	std::unique_lock<std::mutex> lock(queue);

	// create a new task 
	Task request;
	request.unit = unit;
	request.start = start;
	request.destination = destination;

	// Flag the Semaphor
	requests_.signal();

	// Add it to the queueu
	taskQueue_.push(request);

}


std::list<sf::Vector2i> PathFinder::findPath(sf::Vector2i Start, sf::Vector2i Finish)
{

	// create a reference to a number of x/y coordinates
	int xcoord, ycoord, new_g_cost;
	// Createing a pointer referfence for processing the neighbourghs
	Cell* neigh = nullptr;
	// reference for storing a copy of cell object information into the queue
	Cell queue_reference;


	// Create the data strcture to hold the open set
	std::priority_queue<Cell, std::vector<Cell>, cellCompare> open;

	// compute hgf values for the first node
	Cell* current = GetCell(Start);
	current->g_cost = 0;
	current->f_cost = 0 + CalculateHeuristic(Start, Finish);

	// Add the first to the open set
	current->state = Cell::OPEN;
	open.push(*current);

	// While there are Cells in the Open Set
	while (!open.empty())
	{

		// get the most promising node Update the open and remove it from the open set
		current = GetCell(open.top().location);
		queue_reference = open.top();
		open.pop();

		// IF the target has been found the path is complete
		if (current->location.x == Finish.x && current->location.y == Finish.y)
			break;

		// This is required to preserve the interal heap of the priority queue
		// Check if the node has already been removed from the Open Set or if it has been updated on the board
		while (current->state != Cell::OPEN && current->g_cost >= queue_reference.g_cost)
		{

			// Check the board isn't emptied by this proces
			if (open.empty())
			{
				break;
			}

			// Get the next cell from the open set
			current = GetCell(open.top().location);
			queue_reference = open.top();
			open.pop();

		}

		// Updated closed set i.e. remove the curent node from the open
		current->state = Cell::CLOSED;

		// Check each of the cells current neighbours
		for (auto& neighborough : neighboroughs)
		{

			// Find the neighboroughs location relative to the current node
			ycoord = neighborough.y + current->location.y;
			xcoord = neighborough.x + current->location.x;

			// Get a reference to the neighborough
			neigh = GetCell(sf::Vector2i(xcoord, ycoord));

			// check the node exists i.e. isn't off the map
			if (neigh == nullptr)
				continue;

			// This is wall and can't be accessed i.e. not a wall
			if (neigh->state == Cell::BLOCKED)
				continue;

			// calculate a new g value based on this nodes locaiton and the movement cost from the previous tile
			// Check if this is diagonal or perpendicular movement and adjust the movement cost weighting accordingly
			if (current->location.x == xcoord || current->location.y == ycoord)
			{
				new_g_cost = 10 + current->g_cost;
			}
			else
			{
				new_g_cost = 14 + current->g_cost;
			}


			// Check of the cell is in either for the sets 
			if (neigh->state == Cell::CLOSED)  //  if neighbor in CLOSED and cost less than g(neighbor):
			{
				if (new_g_cost < neigh->g_cost)
				{
					neigh->state = Cell::BASIC;
				}
				else
					continue;
			}
			else if (neigh->state == Cell::OPEN) // if neighbor in OPEN and cost less than g(neighbor):
			{
				if (new_g_cost > neigh->g_cost)
				{
					continue;
				}
			}

			// Make the changes to the neighborogh based on the current locaion 
			neigh->g_cost = new_g_cost;
			neigh->f_cost = new_g_cost + CalculateHeuristic(sf::Vector2i(xcoord, ycoord), Finish);
			neigh->parent = current;

			// Add a copy of this node to the Open set
			neigh->state = Cell::OPEN;
			open.push(*neigh);

		}

	}

	// Generate the path a new path to return 
	std::list<sf::Vector2i> path;

	// Start at the end Node to create the path object
	Cell* cell = GetCell(Finish);

	// If no path exists the end node will not have a parent
	if (cell->parent == nullptr)
	{
		path.clear();
		return path;
	}

	// Go throught the path build the coordinate route till the start is reached
	while (cell != GetCell(Start))
	{
		path.push_front(cell->location);
		cell = cell->parent;
	}

	// add the start to the path to complete it 
	// path.push_front(cell->location);

	// return the complete path object
	return path;

}


void PathFinder::Reset()
{

	cellMap_.clear();

	if (cellMap_.empty())
	{

		// Create a map of cells 
		for (int i = 0; i < baseTileMap_.size(); i++)
		{

			// Create a new cell
			Cell newCell;

			// This is the location of the cell on the world grid
			sf::Vector2i loc;
			loc.x = i % mapDimension_;
			loc.y = i / mapDimension_;
			newCell.location = loc;

			cellMap_.push_back(newCell);

		}

	}

	// Function to reset/Initialise all the data for calculatiung the next path
	for(int i = 0; i < baseTileMap_.size(); i++)
	{

		// Reset the tiles
		if (baseTileMap_[i])
			cellMap_[i].state = Cell::BASIC;
		else
			cellMap_[i].state = Cell::BLOCKED;

		cellMap_[i].parent = nullptr;
		cellMap_[i].g_cost = INT32_MAX;
		cellMap_[i].f_cost = INT32_MAX;

	}

}


void PathFinder::Work()
{

	// Work or wait on work
	while (required)
	{

		// Wait for requests
		requests_.wait();

		// Get a task
		Task task = GetTask();

		// Reset the map
		Reset();

		// Complete the task
		task.unit->SetPath(findPath(task.start, task.destination));

	}

}


Cell * PathFinder::GetCell(sf::Vector2i indexLocation)
{

	// If a place is request that doesn't exist return null
	if (indexLocation.y < 0 || indexLocation.y >= mapDimension_ || indexLocation.x < 0 || indexLocation.x >= mapDimension_)
		return nullptr;

	// Determine the row start position in the vector Return a pointer to the specific node requested
	return &cellMap_[indexLocation.y * mapDimension_ + indexLocation.x];

}


int PathFinder::CalculateHeuristic(sf::Vector2i Start, sf::Vector2i Finish)
{

	// Calculate the dx and dy for the heruristics
	int dx = abs(Start.x - Finish.x);
	int dy = abs(Start.y - Finish.y);

	// Using the Octile method
	return (10 * (dx + dy) - (6 * std::min(dx, dy)));

}


Task PathFinder::GetTask()
{

	std::unique_lock<std::mutex> lock(queue);
	Task newTask = taskQueue_.front();
	taskQueue_.pop();
	return newTask;

}


