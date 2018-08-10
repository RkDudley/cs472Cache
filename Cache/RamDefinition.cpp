#include "Ram.h"
#include "cache.h"
#include <string>
#include <sstream>
Ram::Ram()
{
	//initialize main memory 0-255
	short int location = 0;
	int i = 0;
	//assign address to each memories
	while (i <= 0x7FF)
	{
		if (location <= 0xFF)
		{
			mainMemory[i] = location;
			location++;
		}
		else
		{
			// assign address back to 0 when reach count to 255
			location = 0;
			mainMemory[i] = location;
			location++;
		}
		i++;
	}
}
short Ram::getBlockBegin(short addr)
{
	short begin = addr & 0xFFFFFFF0;
	return begin;
}

short Ram::getRamVal(short addr)
{
	short ramVal;
	for (int i = 0; i < MEMSIZE; i++)
	{
		ramVal = mainMemory[addr];
	}
	return ramVal;
}

void Ram::updateRam(short address, short data)
{
	//put a new data according to an old address
	mainMemory[address] = data;
}

std::vector <short> Ram::getBlock(short addr)
{
	blockData.resize(0);
	// get an block begin to find a starting data
	short begin = getBlockBegin(addr);
	for (int i = 0; i < MEMSIZE; i++)
	{
		if (i == begin) // if the memory find a block begin
		{
			for (int j = 0; j < 16; j++)
			{
				//get block begin
				blockData.push_back(mainMemory[i+j]);
			}
			return blockData;
		}
	}
	return blockData;
	
}

