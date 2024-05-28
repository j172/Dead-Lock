// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex1;
std::mutex mutex2;

//Dead Lock
//void Thread1() {
//	std::lock_guard<std::mutex> lock1(mutex1);
//	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
//
//	std::cout << "Thread 1 trying to lock mutex2" << std::endl;
//	std::lock_guard<std::mutex> lock2(mutex2);
//	std::cout << "Thread 1 acquired both mutex1 and mutex2" << std::endl;
//}
//
//void Thread2() {
//	std::lock_guard<std::mutex> lock2(mutex2);
//	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
//
//	std::cout << "Thread 2 trying to lock mutex1" << std::endl;
//	std::lock_guard<std::mutex> lock1(mutex1);
//	std::cout << "Thread 2 acquired both mutex1 and mutex2" << std::endl;
//}

//同時鎖定兩個互斥鎖
//void Thread1() {
//	// Lock both mutexes using std::lock to avoid deadlock
//	std::lock(mutex1, mutex2);
//
//	// Adopt the locks into lock guards to ensure they are properly unlocked
//	std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
//	std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
//
//	std::cout << "Thread 1 acquired both mutex1 and mutex2" << std::endl;
//}
//
//void Thread2() {
//	// Lock both mutexes using std::lock to avoid deadlock
//	std::lock(mutex1, mutex2);
//
//	// Adopt the locks into lock guards to ensure they are properly unlocked
//	std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
//	std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
//
//	std::cout << "Thread 2 acquired both mutex1 and mutex2" << std::endl;
//}

//一致的鎖定順序
//void Thread1() {
//	std::lock_guard<std::mutex> lock1(mutex1);
//	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模擬工作
//
//	std::lock_guard<std::mutex> lock2(mutex2);
//	std::cout << "Thread 1 獲取了 mutex1 和 mutex2" << std::endl;
//}
//
//void Thread2() {
//	std::lock_guard<std::mutex> lock1(mutex1); // 注意順序與 Thread1 一致
//	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模擬工作
//
//	std::lock_guard<std::mutex> lock2(mutex2);
//	std::cout << "Thread 2 獲取了 mutex1 和 mutex2" << std::endl;
//}

//使用 std::unique_lock 和 std::try_lock
void Thread1() {
	while (true) {
		std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
		std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);

		if (std::try_lock(lock1, lock2) == -1) {
			// 成功獲取兩個鎖
			std::cout << "Thread 1 獲取了 mutex1 和 mutex2" << std::endl;
			break;
		}
		else {
			// 鎖定失敗，重試
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

void Thread2() {
	while (true) {
		std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
		std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);

		if (std::try_lock(lock1, lock2) == -1) {
			// 成功獲取兩個鎖
			std::cout << "Thread 2 獲取了 mutex1 和 mutex2" << std::endl;
			break;
		}
		else {
			// 鎖定失敗，重試
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::thread t1(Thread1);
	std::thread t2(Thread2);

	t1.join();
	t2.join();

	return 0;
}

