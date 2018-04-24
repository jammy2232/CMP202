#pragma once

#include <string>
#include <map>
#include <list>
#include <assert.h>
#include <fstream>
#include <chrono>

class DataLogger
{
public:

	DataLogger(std::string filename);
	~DataLogger();

	static void LogValue(std::string Type, float value);

private:

	// reference to the data logger
	static DataLogger* instance_;

	// Storage system for the datalogger
	std::map<std::string, std::list<float>> data_;

	// Reference to the output file 
	std::ofstream file;
	std::string name;

	// Functio to write out data
	void OutputDataToFile();

	// Timestamp info
	std::chrono::steady_clock::time_point start;
	
};

