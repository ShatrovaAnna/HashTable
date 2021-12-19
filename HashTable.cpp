#include "pch.h"
#include "HashTable.h"
#include <cmath>
#include <iostream>
#include <list>

using namespace std;

HashTable::HashTable() {
	table = new ListOfTable[bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		table[i] = new list<std::pair<Key, Value>>;
	}
}

HashTable::~HashTable() {
	freeMemory();
}

HashTable::HashTable(const HashTable& old) {
	bufferSize = old.bufferSize;
	amountOfElements = old.amountOfElements;
	table = new ListOfTable[bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		table[i] = new list<std::pair<Key, Value>>;
		for (auto& it : *old.table[i]) { //звездочка
			table[i]->push_back(it);
		}
	}
}

HashTable::HashTable(HashTable&& old) {
	bufferSize = old.bufferSize;
	amountOfElements = old.amountOfElements;
	table = old.table;
	old.table = nullptr;
	old.bufferSize = 0;
}

HashTable& HashTable::operator=(const HashTable& old) {
	HashTable temp(old);
	swap(temp);
	return *this; //old
}

HashTable& HashTable::operator=(HashTable&& old) {
	HashTable temp(old);
	swap(temp);
	old.table = nullptr;
	old.bufferSize = 0;
	return *this;
}

void HashTable::clear() {
	for (int i = 0; i < bufferSize; i++) {
		table[i]->clear();
	}
	amountOfElements = 0;
}

void HashTable::swap(HashTable& other) {
	std::swap(bufferSize, other.bufferSize);
	std::swap(amountOfElements, other.amountOfElements);
	std::swap(table, other.table);
}

bool HashTable::erase(const Key& key) {
	try {
		int hash = calculate_hash(key, bufferSize);
		ListOfTable list = table[hash];
		for (auto it = list->begin(); it != list->end(); ++it) {
			if (it->first == key) {
				list->erase(it);
				break;
			}
		}
		amountOfElements--;
		return true;
	}
	catch (const invalid_argument& e) {
		return false; 
	}
}

bool HashTable::insert(const Key& key, const Value& value) {
	if (contains(key))
	{
		return false;
	}
	double newFactor = amountOfElements / (double)bufferSize;
	if (newFactor > loadFactor) {
		resize();
	}
	int hash = calculate_hash(key, bufferSize);
	pair <Key, Value> p1(key, value);
	table[hash]->push_back(p1);
	amountOfElements++;
	return true;
}

bool HashTable::contains(const Key& k) const {
	try {
		this->at(k);
		return true;
	}
	catch (const invalid_argument& e) {
		return false;
	}
}

Value& HashTable::operator[](const Key& key) {
	if (contains(key)) {
		int hash = calculate_hash(key, bufferSize);
		ListOfTable list = table[hash];
		for (auto& it : *list) {
			if (key == it.first) {
				return (Value&)(it);
			}
		}
	}
	else {
		pair <Key, Value> p1(key, Value());
		this->insert(p1.first, p1.second);
		int hash = calculate_hash(key, bufferSize);
		ListOfTable list = table[hash];
		for (auto& it : *list) {
			if (key == it.first) {
				return (Value&)(it);
			}
		}
	}
}

Value& HashTable::at(const Key& key) {
	int hash = calculate_hash(key, bufferSize);
	ListOfTable list = table[hash];
	for (auto it = list->begin(); it != list->end(); ++it) {
		if (key == (*it).first) {
			return (Value&)((*it).second);
		}
	}
	throw invalid_argument("No value for this key");
}

const Value& HashTable::at(const Key& key) const {
	int hash = calculate_hash(key, bufferSize);
	ListOfTable list = table[hash];
	for (auto& it : *list) {
		if (key == it.first) {
			return it.second;
		}
	}
	throw invalid_argument("No value for this key");
}

bool operator==(const HashTable& a, const HashTable& b) {
	if (a.bufferSize != b.bufferSize && a.amountOfElements != b.amountOfElements) {
		return false;
	}

	for (int i = 0; i < a.bufferSize; i++) {
		ListOfTable listA = a.table[i];
		ListOfTable listB = b.table[i];
		if (listA->size() == listB->size()) {
			for (auto itA = listA->begin(); itA != listA->end(); ++itA) {
				for (auto itB = listB->begin(); itB != listB->end(); ++itB) {
					Value valueA = (*itA).second;
					Value valueB = (*itB).second;
					if (valueA.age != valueB.age || valueA.weight != valueB.weight || (*itA).first != (*itB).first) {
						return false;
					}
				}
			}
		}
		else {
			return false;
		}
	}
	return true;
}

bool operator!=(const HashTable& a, const HashTable& b) {
	return !(a == b);
}

bool HashTable::empty() const {
	return (amountOfElements == 0);
}

size_t HashTable::size() const {
	return amountOfElements;
}

void HashTable::resize() {
	size_t newBufferSize = bufferSize * 2;
	Table newTable = new ListOfTable[newBufferSize];
	for (int i = 0; i < newBufferSize; i++) {
		newTable[i] = new list<std::pair<Key, Value>>;
	}
	for (int i = 0; i < bufferSize; i++) {
		ListOfTable list = table[i];
		for (auto it = list->begin(); it != list->end(); ++it) {//
			//Value value = (*it);
			int hash = calculate_hash((*it).first, newBufferSize);
			newTable[hash]->push_back(*it);
		}
		delete table[i];
	}
	delete[]table;
	table = newTable;
	bufferSize = newBufferSize;
}

void HashTable::freeMemory() {
	if (table == nullptr)
	{
		return;
	}
	for (int i = 0; i < bufferSize; i++) {
		delete table[i];
	}
	delete[]table;
}

int HashTable::calculate_hash(const Key& key, int size) const {
	size_t leng = key.length();
	size_t hash = 0;
	for (int i = 0; i < leng; i++) {
		int symb = (int)key[i];
		hash += (symb % 3) * (int)pow(3, i);
	}
	return (int)(hash % size);
}