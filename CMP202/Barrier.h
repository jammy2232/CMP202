#pragma once

#include <condition_variable>
#include <mutex>

class Barrier
{

public:

	Barrier(int threads) : threads_(threads), thread_counter_(threads) {}
	~Barrier() {}

	void wait()
	{

		// Lock the barrier
		std::unique_lock<std::mutex> lock(barrier_lock_);

		// Update the thread counter
		thread_counter_--;

		// Check if the barrier needs unlocked
		if (thread_counter_ == 0)
		{
			cv.notify_all();
			thread_counter_ = threads_;
		}
		else
		{
			// Otherwise threads need to wait
			cv.wait(lock);
		}

	}

private:

	// Variables 
	std::condition_variable cv;
	std::mutex barrier_lock_;
	int thread_counter_;
	int threads_;

};

