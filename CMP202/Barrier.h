#pragma once

#include <condition_variable>
#include <atomic>
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

			thread_counter_ = threads_;
			cv.notify_all();

		}
		else
		{ 
			// Otherwise threads need to wait
			cv.wait(lock, [=] { return thread_counter_ == threads_; });
		}

	}

private:

	// Variables 
	std::condition_variable cv;
	std::mutex barrier_lock_;
	std::atomic<int> thread_counter_;
	int threads_;

};

