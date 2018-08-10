#ifndef RAM_H
#define RAM_H

#include <vector>

class Ram
{
private:
	short mainMemory[2048];
	std::vector <short> blockData;
	short blockBegin;
	short blockEnd;
	const int SIZE = 16;
	const int MEMSIZE = 2048;
public:
	Ram();
	//Function to get a value from main memory
	short getRamVal(short addr);
	//Function to get block base 16 bytes
	std::vector<short> getBlock(short addr);
	//Function to get block begin
	short getBlockBegin(short addr);
	//Function to update value to main memory when there is a write back operation occur 
	void updateRam(short address, short data);
};


#endif
