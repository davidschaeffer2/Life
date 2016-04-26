/**************************************************************************************************
Author:		David Schaeffer
Date: 		April 26, 2016
Filename:	life.cpp
Description:	Small program that replicates Conway's Game of Life:
		https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
**************************************************************************************************/

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>      // atoi() and rand()
#include <time.h>       // for intitial rand() seed

// compile with: g++ -o life life.cpp -std=gnu++0x

using namespace std;

void createLiveBoard(vector<vector<char> > &liveBoard, int boardSize);
void seedRandomly(vector<vector<char> > &liveBoard, int boardSize);
void seedFromFile(vector<vector<char> > &liveBoard, ifstream &inf, int boardSize);
void copyLiveBoard(vector<vector<char> > &liveBoard, vector<vector<char> > &snapshotBoard, int boardSize);
void copySnapshotBoard(vector<vector<char> > &liveBoard, vector<vector<char> > &snapshotBoard, int boardSize);
void tick(vector<vector<char> > &liveBoard, vector<vector<char> > &snapshotBoard, int boardSize);
void printLiveBoard(vector<vector<char> > &liveBoard, int boardSize);
bool stability(vector<vector<char> > &priorSnapshotBoard, vector<vector<char> > &snapshotBoard, int boardSize);

int main(int argc, char *argv[])
{
	if(argc == 2)
	{
		int boardSize = atoi(argv[1]);
		vector<vector<char> > liveBoard(boardSize, vector<char>(boardSize-1));
		vector<vector<char> > snapshotBoard(boardSize, vector<char>(boardSize-1));
		vector<vector<char> > priorSnapshotBoard(boardSize, vector<char>(boardSize-1));
		createLiveBoard(liveBoard, boardSize);
		seedRandomly(liveBoard, boardSize);
		copyLiveBoard(liveBoard, snapshotBoard, boardSize);
		int iteration = 0;
		do
		{
			iteration++;
			tick(liveBoard, snapshotBoard, boardSize);
			copySnapshotBoard(liveBoard, snapshotBoard, boardSize);
			cout << "Board after " << iteration << " iterations:" << endl;
			printLiveBoard(liveBoard, boardSize);
			cin.ignore();
			system("clear");
		} while(iteration < 10000);
	}
	else if(argc == 3)
	{
		ifstream inf;
		inf.open(argv[1]);
		if(!inf)
		{
			cout << "File failed to open." << endl;
			return(0);
		}
		int boardSize = atoi(argv[2]);
		vector<vector<char> > liveBoard(boardSize, vector<char>(boardSize-1));
		vector<vector<char> > snapshotBoard(boardSize, vector<char>(boardSize-1));
		seedFromFile(liveBoard, inf, boardSize);
		copyLiveBoard(liveBoard, snapshotBoard, boardSize);
		int iteration = 0;
		do
		{
			iteration++;
			tick(liveBoard, snapshotBoard, boardSize);
			copySnapshotBoard(liveBoard, snapshotBoard, boardSize);
			cout << "Board after " << iteration << " iterations:" << endl;
			printLiveBoard(liveBoard, boardSize);
			cin.ignore();
			system("clear");
		} while(iteration < 10000);
	}
	else
	{
		cout << "Please enter a file name and size of the board "
			<< "as command line argument. Example: " 
			<< "./a.out fileName.txt 5" << endl;
	}
	return(0);
}

void createLiveBoard(vector<vector<char> > &liveBoard, int boardSize)
{
	// top border
	for(int row = 0;;)
	{
		for(int col = 0; col < boardSize; col++)
		{
			liveBoard[row][col] = 'l';
		}
		break;
	}
	// left border
	for(int row = 0; row < boardSize; row++)
	{
		for(int col = 0;;)
		{
			liveBoard[row][col] = 'l';
			break;
		}
	}
	// bottom border
	for(int row = boardSize-1;;)
	{
		for(int col = 0; col < boardSize; col++)
		{
			liveBoard[row][col] = 'l';
		}
		break;
	}
	// right border
	for(int row = 0; row < boardSize; row++)
	{
		for(int col = boardSize-1;;)
		{
			liveBoard[row][col] = 'l';
			break;
		}
	}
}

void seedRandomly(vector<vector<char> > &liveBoard, int boardSize)
{
	int randNum;
	srand(time(NULL));
	for(int row = 1; row < boardSize-1; row++)
	{
		for(int col = 1; col < boardSize-1; col++)
		{
			randNum = rand() % 2; // 0-1
			if(randNum == 0)
			{
				liveBoard[row][col] = '.';
			}
			else liveBoard[row][col] = 'O';
		}
	}
}

void seedFromFile(vector<vector<char> > &liveBoard, ifstream &inf, int boardSize)
{
	for(int row = 0; row < boardSize; row++)
	{
		for(int col = 0; col < boardSize; col++)
		{
			inf >> liveBoard[row][col];
		}
	}
}

void copyLiveBoard(vector<vector<char> > &liveBoard, vector<vector<char> > &snapshotBoard, int boardSize)
{
	for(int row = 0; row < boardSize; row++)
	{
		for(int col = 0; col < boardSize; col++)
		{
			snapshotBoard[row][col] = liveBoard[row][col];
		}
	}
}

void copySnapshotBoard(vector<vector<char> > &liveBoard, vector<vector<char> > &snapshotBoard, int boardSize)
{
	for(int row = 0; row < boardSize; row++)
	{
		for(int col = 0; col < boardSize; col++)
		{
			liveBoard[row][col] = snapshotBoard[row][col];
		}
	}
}

void tick(vector<vector<char> > &liveBoard, vector<vector<char> > &snapshotBoard, int boardSize)
{	
	for(int row = 1; row < boardSize-1; row++)
	{
		for(int col = 1; col < boardSize-1; col++)
		{
			int numLivingNeighbors = 0;
			if(liveBoard[row][col] == 'O')
			{
				if(liveBoard[row-1][col-1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row][col-1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row+1][col-1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row+1][col] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row+1][col+1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row][col+1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row-1][col+1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row-1][col] == 'O'){numLivingNeighbors++;}
				// if less than 2 living neighbors, die
				if(numLivingNeighbors < 2)
				{
					snapshotBoard[row][col] = '.';
				}
				// if 2-3 living neighbors, survive
				if(numLivingNeighbors == 2 || numLivingNeighbors == 3)
				{
					snapshotBoard[row][col] = 'O';
				}
				// if more than 3 living neighbors, die
				if(numLivingNeighbors > 3)
				{
					snapshotBoard[row][col] = '.';
				}
			}
			else if(liveBoard[row][col] == '.')
			{
				if(liveBoard[row-1][col-1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row][col-1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row+1][col-1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row+1][col] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row+1][col+1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row][col+1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row-1][col+1] == 'O'){numLivingNeighbors++;}
				if(liveBoard[row-1][col] == 'O'){numLivingNeighbors++;}
				// if more than 3 living neighbors, come back to life
				if(numLivingNeighbors == 3)
				{
					snapshotBoard[row][col] = 'O';
				}
			}
		}
	}
	
}

void printLiveBoard(vector<vector<char> > &liveBoard, int boardSize)
{
	for(int row = 0; row < boardSize; row++)
	{
		for(int col = 0; col < boardSize; col++)
		{
			cout << liveBoard[row][col] << " ";
		}
		cout << endl;
	}
}

bool stability(vector<vector<char> > &priorSnapshotBoard, vector<vector<char> > &snapshotBoard, int boardSize)
{
	for(int row = 1; row < boardSize-1; row++)
	{
		for(int col = 1; col < boardSize-1; col++)
		{
			if(priorSnapshotBoard[row][col] == snapshotBoard[row][col])
			{
				return true;
			}
		}
	}
	return false;
}
