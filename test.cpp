#include "pch.h"
#include "HashTable.h"


TEST(Test1, Insert) {

	Value value1(10, 20);
	Value value2(30, 40);
	Value value3(50, 60);
	Value value4(70, 80);
	pair <Key, Value> pair1("key1", value1);
	pair <Key, Value> pair2("key2", value2);
	pair <Key, Value> pair3("key3", value3);
	pair <Key, Value> pair4("key3", value4);
	HashTable* table = new HashTable();

	EXPECT_EQ(true, table->insert(pair1.first, pair1.second));
	EXPECT_EQ(true, table->insert(pair2.first, pair2.second));
	EXPECT_EQ(true, table->insert(pair3.first, pair3.second));

	EXPECT_EQ(false, table->insert(pair1.first, pair1.second));

	table->erase(pair3.first);

	EXPECT_EQ(true, table->insert(pair4.first, pair4.second));

	delete table;
}

TEST(Test2, Contain) {

	Value value1(10, 20);
	pair <Key, Value> pair1("key1", value1);
	HashTable* table = new HashTable();

	EXPECT_EQ(false, table->contains(pair1.first));

	table->insert(pair1.first, pair1.second);

	EXPECT_EQ(true, table->contains(pair1.first));

	table->erase(pair1.first);

	EXPECT_EQ(false, table->contains(pair1.first));

	delete table;
}

TEST(Test3, Erase) {
	Value value1(10, 20);
	pair <Key, Value> pair1("key1", value1);
	HashTable* table = new HashTable();

	EXPECT_EQ(false, table->erase(pair1.first));

	table->insert(pair1.first, pair1.second);

	EXPECT_EQ(true, table->erase(pair1.first));

	delete table;
}

TEST(Test4, At) {

	Value value1(10, 20);
	pair <Key, Value> pair1("key1", value1);
	HashTable* table = new HashTable();
	try {
		table->at(pair1.first);
	}
	catch (std::invalid_argument& e) {
		table->insert(pair1.first, pair1.second); //поправить с value
		Value& value = table->at("key1");
		EXPECT_EQ(10, value.age);
		EXPECT_EQ(20, value.weight);
		EXPECT_EQ("key1", pair1.first);

	}

	delete table;
}

TEST(Test5, Operator) {

	Value value1(10, 20);
	pair <Key, Value> pair1("key1", value1);
	HashTable* table = new HashTable();
	try {
		(*table)["key1"];
	}
	catch (std::invalid_argument& e) { //value
		table->insert(pair1.first, pair1.second);
		Value& value = (*table)["key1"];
		EXPECT_EQ(10, value.age);
		EXPECT_EQ(20, value.weight);
		EXPECT_EQ("key1", pair1.first);
	}
	delete table;
}