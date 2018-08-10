// Project 2 Cache Simulation
//Author: Rattikarn Dudley

#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include "Ram.h"
#include <vector>


class Cache :public Ram
{
private:
	std::vector<short> validFlag;
	short slots[16];
	std::vector <short> tag;
	std::vector<std::vector<short>> data;
	short offSet;
	std::vector<short> dirtyBit;
	const int SIZE = 16;
	std::vector <short> address;
public:

	Cache(); //constructor defalut 

	//Function to display cache
	void displayCache(std::ofstream &outFile);

	//Function to get an offset 
	short getOffset(short address);

	//Function to get a slot number
	short getSlot(short address);

	//Function to assign a tag 
	std::vector<short> updateTag(short address);

	//Function to get data from the Ram (get data from a Ram class)
	std::vector<std::vector<short>> updateData(short address);

	//Function to get a valid flag from private members(cache)
	std::vector<short> getValid();

	//Function to get a data from private members(cache)
	std::vector<std::vector<short>> getData();

	//Function to get a tag from private members(cache)
	std::vector<short> getTag();

	//funtion to get a dirty bit from private member(cache)
	std::vector<short> getDirtyBit();

	// Function to check valid flag
	std::vector<short> updateValid(short address);

	//function to assign dirty bit when a write operation is called
	std::vector <short> updateDirty(short addr);

	//Function to get value
	short getValue(short addr);

	//Function to read cache
	void readCache(short addr, std::ofstream& outFile);

	//Function to write cache
	void writeCache(short addr, short newData, std::ofstream& outFile);

	//Function to get an address back to a main memory
	std::vector<short> getAdress(short newAdress);

};

#endif