#include "Semaphor.h"


Semaphor::Semaphor(unsigned int counter)
{
	m_count = counter;
}


Semaphor::~Semaphor()
{
}


void Semaphor::signal()
{

	std::unique_lock<std::mutex> lock(m_count_lock);
	m_count++;
	m_count_cv.notify_one();

}


void Semaphor::wait()
{

	std::unique_lock<std::mutex> lock(m_count_lock);

	// This will catch any spurious wakeup while maintaining wait
	while (m_count == 0)
	{
		m_count_cv.wait(lock);
	}

	m_count--;

}
