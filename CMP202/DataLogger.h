#pragma once

#include <string>
#include <map>
#include <list>
#include <assert.h>
#include <fstream>
#include <chrono>

class Unit;

class DataLogger
{
public:

	DataLogger(std::string filename);
	~DataLogger();

	static void LogValue(std::string Type, float value);
	static void LogValue(std::string Type, long long value);
	static void LogValue(std::string Type, Unit* value);
	void WriteValues();

private:

	// reference to the data logger
	static DataLogger* instance_;

	// Storage system for the datalogger
	std::map<std::string, std::list<Unit*>> data_unit_;
	std::map<std::string, std::list<float>> data_f_;
	std::map<std::string, std::list<long long>> data_ll_;
	std::map<std::string, std::list<long long>> TimeStamps_;

	// Reference to the output file 
	std::ofstream file;
	std::string name;

	// Functio to write out data
	void OutputDataToFile();

	// Timestamp info
	std::chrono::steady_clock::time_point start;
	
};

