// Project 2 Cache Simulation
//Author: Rattikarn Dudley

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "cache.h"
#include "Ram.h"

int main()
{
	char choice = ' ';
	Cache testCache;
	short requestAddr = 0;
	std::ifstream inFile("instruction.txt");
	std::ofstream outFile("output.txt");

	while (inFile >> choice)
	{	
			outFile << "(R)ead, (W)rite or (D)isplay Cache ?" << std::endl;
			outFile << choice << std::endl;
			
			switch (choice)
			{
			case 'R':
				outFile << "What address would you like to read? " << std::endl;
				inFile >> std::hex >> requestAddr;
				outFile << requestAddr << std::endl;
				testCache.readCache(requestAddr,outFile);
				break;
			case 'W':
				outFile << "What address would you like to write? " << std::endl;
				inFile >> std::hex >> requestAddr;
				outFile << requestAddr << std::endl;
				outFile << "What data would you like to write at the address?" << std::endl;
				short data;
				inFile >> std::hex >> data;
				outFile << data << std::endl;
				testCache.writeCache(requestAddr, data,outFile);
				break;
			case 'D':
				testCache.displayCache(outFile);
				break;
			default:
				outFile << "an error occurs " << std::endl;
				break;
			}
	}

	inFile.close();
	outFile.close();

	return 0;
}





