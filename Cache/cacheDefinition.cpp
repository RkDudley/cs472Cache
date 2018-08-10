// Project 2 Cache Simulation
//Author: Rattikarn Dudley

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "cache.h"
#include "Ram.h"


Cache::Cache()
{
	//initialize all members to 0
	offSet = 0;
	validFlag = std::vector<short>(16, 0);
	tag = std::vector<short>(16, 0);
	dirtyBit = std::vector<short>(16, 0);
	data = std::vector<std::vector<short> >(16, std::vector<short>(16, 0));
	//initialize slots from 1-16
	for (int i = 0 ; i < SIZE; i++)
	{
		slots[i] = i;
	}
	address = std::vector<short>(16, 0);
	
}

std::vector<short> Cache::getValid()
{
	std::vector<short> valid = validFlag;
	return valid;
}

std::vector<std::vector<short>> Cache::getData()
{
	std::vector<std::vector<short>> vecData = data;
	return vecData;
}

std::vector<short> Cache::getTag()
{
	std::vector<short> vecTag = tag;
	return vecTag;
}

std::vector <short> Cache::getDirtyBit()
{
	std::vector<short> vecDirty = dirtyBit;
	return vecDirty;
}

short Cache::getOffset(short address)
{
	offSet = address & 0x0000000F;
	return offSet;
}

short Cache::getSlot(short address)
{
	short tempSlot = (address & 0x000000F0) >> 4;
	
	return tempSlot;
}
std::vector<short> Cache::updateTag(short address)
{
	short tempTag = address >> 8;
	short slot = getSlot(address);
	for (size_t i = 0; i < tag.size(); i++)
	{
			if (i == slot)
			{
				tag[i] = tempTag;
			}
	}

	return tag;
}

std::vector <short> Cache::updateValid(short addr)
{
	for (size_t i = 0; i < validFlag.size(); i++)
	{
		if (i == getSlot(addr))
		{
			validFlag[i] = 1;
		}
	}

	return validFlag;
}

std::vector <short> Cache::getAdress(short addr)
{
	for (size_t i = 0; i < address.size(); i++)
	{
		if (i == getSlot(addr))
		{
			address[i] = addr;
		}
	}

	return address;
}

std::vector <short> Cache::updateDirty(short addr)
{
	for (size_t i = 0; i < dirtyBit.size(); i++)
	{
		if (i == getSlot(addr))
		{
			dirtyBit[i] = 1;
		}
	}

	return dirtyBit;
}

std::vector<std::vector<short>> Cache::updateData(short addr) 
{
	std::vector<std::vector <short>> tempData = getData();
	short slot = getSlot(addr);

	std::vector<short> block = getBlock(addr);

	for (size_t i = 0; i < tempData.size(); i++)
	{
		for (size_t j = 0; j < tempData[i].size(); j++)
		{
			if (i == slot)
			{
				//get the block from Ram class 
				tempData[i][j] = block[j];
			}
		}
	}

	//put a memory to data cache
	data = tempData;

	return tempData;	
}
short Cache::getValue(short addr)
{
	std::vector <std::vector<short>> vecData = getData();
	short slot = getSlot(addr);
	short imm = getOffset(addr);
	short value = 0;

	for (size_t i = 0; i < vecData.size(); i++)
	{
		for (size_t j = 0; j < vecData[i].size(); j++)
		{
			if (i == slot)
			{
				value = vecData[i][imm];
			}
		}
	}
	return value;
}

void Cache::readCache(short addr, std::ofstream& outFile)
{
	short slot = getSlot(addr);
	std::vector<std::vector<short>> vecData = getData();
	std::vector<short> valid = getValid();
	std::vector<short> vecTag = getTag();
	std::vector<short> tempTag = updateTag(addr);

	for (size_t i = 0; vecData.size(); i++ )
	{	
		for (size_t j = 0; j < vecData[i].size(); j++)
		{
			if (i == slot)
			{   // determine if the data has been read by valid bit and a tag
				if (valid[i] == 1 &&  vecTag[i] == tempTag[i])
				{
					outFile << "At address " << addr << ", there is a value " << std::hex << std::uppercase << getValue(addr)
						<< ". (Cache Hit)" << std::endl;
					return;		
				}
				else
				{
					outFile << "There is no value " << getRamVal(addr)<< " (Cache Miss)"
						<< std::endl;
					//check a dirty bit if an write operation is occured
					if (dirtyBit[i] == 1 )
					{
						//get a correct offset 
						short imm = getOffset(address[i]);
						//update memory by calling function from ram class
						updateRam(address[i],data[i][imm]);
						//reverse a dirty bit back to 0 after update a value back to a main memory
						dirtyBit[i] = 0;
					}
					vecData = updateData(addr);
					valid = updateValid(addr);	
					vecTag = updateTag(addr);	
				return;
				}
			}//end if				
		}//end for
	}//end for
}


void Cache::writeCache(short addr, short newData, std::ofstream& outFile)
{
	std::vector <short> vecTag = getTag();
	std::vector <short> temptag = updateTag(addr);
	std::vector <std::vector <short>> vecData = getData();
	std::vector <short> vecValid = getValid();
	std::vector <short> vecDirtyBit = getDirtyBit();
	short numSlot = getSlot(addr);
	short immi = getOffset(addr);
	//keep address for write back to main memory
	getAdress(addr);

	for (size_t i = 0; i < vecData.size(); i++)
	{
		for (size_t j = 0; j < vecData[i].size(); j++)
		{
			if (i == numSlot)
			{	//determine cache miss by checking a valid bit
				if (vecValid[i] != 1)
				{
					outFile << "Cache Miss !" << std::endl;
					vecData = updateData(addr);
					vecValid = updateValid(addr);
					vecTag = updateTag(addr);
					vecDirtyBit = updateDirty(addr);
					//assign a new data according to an offset in a particular block 
					data[i][immi] = newData;

					outFile << "Value " << std::hex << std::uppercase << newData << " has been written to address " 
						<< std::hex << std::uppercase << addr << "." << std::endl;
					return;		
				}
				//determine a right offset and a right tag for prepare to put a value back to ram
				else if (vecValid[i] == 1 && j == immi && vecTag[i] == temptag[i])
				{
					outFile << "Cache Hit " << std::endl;
					vecDirtyBit = updateDirty(addr);
					data[i][immi] = newData;
					outFile << "Value " << std::hex << std::uppercase << newData << " has been written to address " 
						<< std::hex << std::uppercase << addr << "." << std::endl;
				}
			}//end if
		}//end for 
	}//end for 
	
}//end function


void Cache::displayCache(std::ofstream& outFile)
{
	std::vector<std::vector<short>> temp = getData();
	std::vector<short> tempTag = getTag();
	std::vector<short> valid = getValid();
	std::vector<short> dirty = getDirtyBit();
	
	std::cout << "Slot" << std::setw(10) << "Valid" << std::setw(9) << "Tag" <<std::setw(13)<< "Dirty Bit" << std::setw(11) << "Data" << std::endl;
	outFile << "Slot" << std::setw(10) << "Valid"  << std::setw(9) << "Tag" << std::setw(13) << "Dirty Bit" << std::setw(11) << "Data" << std::endl;
	for (int i = 0; i < SIZE; i++)
	{
		std::cout << std::hex << std::uppercase << std::setw(2) << slots[i] << std::setw(10) << valid[i]
			<<std::setw(10)  << std::setw(10) << tempTag[i] << std::setw(10) << dirtyBit[i]<<"         ";

		outFile << std::hex << std::uppercase << std::setw(2) << slots[i] << std::setw(10) << valid[i]
			<< std::setw(10) << std::setw(10) << tempTag[i]<<std::setw(10) << dirtyBit[i] << "         ";

		for (size_t j = 0; j < temp[i].size(); j++)
		{
			std::cout << std::hex << std::uppercase << std::setw(3) << temp[i][j] << " ";
			outFile << std::hex << std::uppercase << std::setw(3) << temp[i][j] << " ";
		}

		std::cout << std::endl;
		outFile << std::endl;
	}
}