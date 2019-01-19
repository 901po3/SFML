#pragma once
#include <mutex>
#include <iostream>
#include <string> 
#include <fstream>
#include <stdexcept>

void write_to_file(const std::string & message) {
	// mutex to protect file access (shared across threads)
	static std::mutex mutex;

	// lock mutex before accessing file
	std::lock_guard<std::mutex> lock(mutex);

	// try to open file
	std::ofstream file("example.txt");
	if (!file.is_open())
		throw std::runtime_error("unable to open file");

	// write message to file
	file << message << std::endl;

	// file will be closed 1st when leaving scope (regardless of exception)
	// mutex will be unlocked 2nd (from lock destructor) when leaving
	// scope (regardless of exception)
}

void bad()
{
	std::mutex m;
	m.lock();                    // acquire the mutex
	write_to_file("bad way of writing to file");   // if function throws an exception, the mutex is never released
	if (1) return; // early return, the mutex is never released
	m.unlock();                  // if bad() reaches this statement, the mutex is released
}

void good()
{
	std::mutex m;
	std::lock_guard<std::mutex> lk(m); // RAII class: mutex acquisition is initialization
	write_to_file("good way of writing to file");  // if function throws an exception, the mutex is released
	if (1) return;       // early return, the mutex is released
}

