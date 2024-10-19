#include "TemporaryStorage.hpp"


void TemporaryStorage::run() {
	while (true) {
		boost::core::detail::Sleep(5000);
		g_lock.lock();
		bool isPrintable = false;
		for (auto it = storage.begin(); it != storage.end(); ) {
			it->second -= 5;
			if (it->second < 0) {
				it = storage.erase(it); // Удаляем элемент и обновляем итератор
				isPrintable = true;
			}
			else {
				++it; // Переходим к следующему элементу
			}
		}
		g_lock.unlock();
		if (isPrintable == true) {
			printStorage();
		}
	}
}

void TemporaryStorage::printStorage() {
	std::cout << "------------------------------------------------" << std::endl;
	for (const auto& pair : storage) {
		std::cout << pair.first.address << " " << pair.first.port << std::endl;
	}
	std::cout << "------------------------------------------------" << std::endl;
}

void TemporaryStorage::putInStorage(std::string adr, short port){
	g_lock.lock();
	if (storage.size() == 0) {
		storage.insert(std::pair<Key, int>({ adr,port }, 15));
		printStorage();
	}
	else if (storage.count({ adr, port }) == 1) {
		storage[{adr, port}] = 15;
	}
	else if (storage.count({ adr, port }) == 0) {
		storage.insert(std::pair<Key, int>({ adr,port }, 15));
		printStorage();
	}
	g_lock.unlock();
}
