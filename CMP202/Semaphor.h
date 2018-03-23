#pragma once

#include <mutex>
#include <condition_variable>

class Semaphor
{

public:

	Semaphor(unsigned int counter = 0);
	~Semaphor();

	void signal();
	void wait();

private:

	unsigned m_count; 
	std::condition_variable m_count_cv;
	std::mutex m_count_lock;

};

