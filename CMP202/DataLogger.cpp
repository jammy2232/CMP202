#include "DataLogger.h"
#include "GameSettings.h"

// Simplify code
using std::chrono::duration_cast;
using std::chrono::milliseconds;

// reference to the data logger
DataLogger* DataLogger::instance_ = nullptr;


DataLogger::DataLogger(std::string filename) : name(filename)
{

	// Check there is only one instance and assign it to the singleton pattern
	assert(instance_ == nullptr);
	instance_ = this;

	// Get the time stamp for the start
	start = std::chrono::steady_clock::now();

}


DataLogger::~DataLogger()
{

	// Once instanceated create/open the desired file
	file.open(name);

	// Check the file has opened successfully and is ready to be written to 
	assert(file.good() == true);

	// Output all the data to the file 
	OutputDataToFile();

	// Once instanceated create/open the desired file
	file.close();

}


void DataLogger::LogValue(std::string Type, float value)
{

	// if no datalogger exists just return
	if (!instance_)
		return;

	// Get the time stamp
	auto now = std::chrono::steady_clock::now();

	// Cast it to milliseconds
	auto timeTaken = duration_cast<milliseconds>(now - instance_->start).count();

	// check if the entry exists and setup a new list
	if(instance_->data_.count(Type) == 0)
	{ 
		instance_->data_[Type + " TimeStamp"].clear();
		instance_->data_[Type].clear();
	}

	// Add a entry to the map for handling time stamps
	instance_->data_.at(Type + " TimeStamp").push_back(timeTaken);
	
	// Write the data to the relecant list within the map structure
	instance_->data_.at(Type).push_back(value);

}


void DataLogger::OutputDataToFile()
{

	// Log the critcal information corresponding to the applicaiton
	file << "MAXUNITS" << MAXUNITS << '\n';
	file << "MAXPROJECTILES" << MAPSIZE*2 << '\n';
	file << "MAPSIZE" << MAPSIZE << '\n';
	file << "PATHFINDINGTHEADS" << PATHFINDINGTHEADS * 2 << '\n';
	file << "PROCESSINGTHEADS" << PROCESSINGTHEADS << '\n';

	// For each of the entries in the data logger output the informaiton
	for (auto data : data_)
	{

		// Write out what the data is 
		file << data.first << ",";

		for (auto item : data.second)
		{
			file << item << ",";
		}

		// Create a new line
		file << '\n';

	}

}
