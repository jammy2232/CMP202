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

	instance_ = nullptr;

}


void DataLogger::LogValue(std::string Type, float value)
{

	// Mutex lock the data logger
	std::unique_lock<std::mutex> lock(instance_->lock_);

	// if no datalogger exists just return
	if (!instance_)
		return;

	// Get the time stamp
	auto now = std::chrono::steady_clock::now();

	// Cast it to milliseconds
	auto timeTaken = duration_cast<milliseconds>(now - instance_->start).count();

	// Add a entry to the map for handling time stamps
	instance_->TimeStamps_[Type + "_TimeStamp"].push_back(timeTaken);
	
	// Write the data to the relecant list within the map structure
	instance_->data_f_[Type].push_back(value);

}


void DataLogger::LogValue(std::string Type, long long value)
{

	// Mutex lock the data logger
	std::unique_lock<std::mutex> lock(instance_->lock_);

	// if no datalogger exists just return
	if (!instance_)
		return;

	// Get the time stamp
	auto now = std::chrono::steady_clock::now();

	// Cast it to milliseconds
	auto timeTaken = duration_cast<milliseconds>(now - instance_->start).count();

	// Add a entry to the map for handling time stamps
	instance_->TimeStamps_[Type + "_TimeStamp"].push_back(timeTaken);

	// Write the data to the relecant list within the map structure
	instance_->data_ll_[Type].push_back(value);

}


void DataLogger::LogValue(std::string Type, Unit * value)
{

	// Mutex lock the data logger
	std::unique_lock<std::mutex> lock(instance_->lock_);

	// if no datalogger exists just return
	if (!instance_)
		return;

	// Get the time stamp
	auto now = std::chrono::steady_clock::now();

	// Cast it to milliseconds
	auto timeTaken = duration_cast<milliseconds>(now - instance_->start).count();

	// Add a entry to the map for handling time stamps
	instance_->TimeStamps_[Type + "_TimeStamp"].push_back(timeTaken);

	// Write the data to the relecant list within the map structure
	instance_->data_unit_[Type].push_back(value);

}


void DataLogger::WriteValues()
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


void DataLogger::OutputDataToFile()
{

	// Log the critcal information corresponding to the applicaiton
	file << "MAXUNITS, " << MAXUNITS << '\n';
	file << "MAPSIZE, " << MAPSIZE << '\n';
	file << "PATHFINDINGTHEADS, " << PATHFINDINGTHEADS << '\n';
	file << "PROCESSINGTHEADS, " << PROCESSINGTHEADS << '\n';

	// For each of the entries in the data logger output the informaiton
	for (auto timestamp : TimeStamps_)
	{

		// Write out what the data is 
		file << timestamp.first << ",";

		for (auto item : timestamp.second)
		{
			file << item << ",";
		}

		// Create a new line
		file << '\n';

	}

	// For each of the entries in the data logger output the informaiton
	for (auto data : data_f_)
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

	// For each of the entries in the data logger output the informaiton
	for (auto data : data_ll_)
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

	// For each of the entries in the data logger output the informaiton
	for (auto data : data_unit_)
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
