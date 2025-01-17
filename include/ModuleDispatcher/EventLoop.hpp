#pragma once
#include <functional>
#include <iostream>
#include <unistd.h>
#include <condition_variable>
#include <thread>
#include <atomic>

enum ECallPolicy {
	ECP_ENDLESS,
	ECP_DONE
};

template <typename KeyType>
struct EventId {
public:
	KeyType key;
	ECallPolicy callPolicy;
};

class EventLoop final
{
public:
	using function_type = std::function<void()>;
	using EventType = std::pair<EventId<std::string>, std::function<void()>>;
	EventLoop() : mThread(&EventLoop::start, this) {	}

	EventLoop(const EventLoop&) = delete;
	EventLoop(EventLoop&&) noexcept = delete;

	~EventLoop() {
		stop();
		mThread.join();
	};

	std::vector<function_type>	mFunctions;
	std::vector<function_type>	mFunctionsRegular;
	std::thread					mThread;
	std::mutex					mMutex;

	void stop() {
		mWorking = false;
	}
	template <int T = ECP_DONE>
	void addFunction(const function_type& func) {
		std::lock_guard<std::mutex> lock(mMutex);
		(T == ECP_DONE) ? mFunctions.push_back(func) : mFunctionsRegular.push_back(func);
	}

	void join() {
		mThread.join();
	}

	void setSleepTimeMS(unsigned long long ms) {
		mSleepTimeMS = ms;
	}
	
private:
	bool mWorking = true;
	unsigned long long mSleepTimeMS = 50;

	std::vector<function_type> local_deq;
	void start() {
		while (mWorking) {
			{
				std::lock_guard<std::mutex> lock(mMutex);
				local_deq.insert(local_deq.end(), mFunctionsRegular.begin(), mFunctionsRegular.end());
				if (!mFunctions.empty()) {
					local_deq.insert(local_deq.end(), mFunctions.begin(), mFunctions.end());
					mFunctions.clear();
				}
			}
			for (auto &f: local_deq) {
				f();
			}
			local_deq.clear();
			usleep(mSleepTimeMS * 1000);
		}
	}
};