#pragma once
#include <iostream>
#include <sstream>
#include <algorithm>
#include <boost/core/detail/sp_win32_sleep.hpp>
#include <string>
#include <map>
#include <mutex>

struct Key {
	std::string address;
	int port;

	// Оператор сравнения для использования в std::map
	bool operator<(const Key& other) const {
		return std::tie(address, port) < std::tie(other.address, other.port);
	}
};

using doubleKeyMap = std::map<Key, int>;

class TemporaryStorage
{

public:
	void run();
	void putInStorage(std::string adr, short port);
	void printStorage();

private:
	std::mutex g_lock;
	doubleKeyMap storage;
};