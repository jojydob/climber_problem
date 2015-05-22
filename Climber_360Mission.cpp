 /*
	 file: Climber_360Mission.cpp : 定义控制台应用程序的入口点。
	 mission: solve Climber_Problem advised by 360	
	 author: Yan Gong
	 date: 2015/5/22
 */
#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <vector>
using namespace std;

#define H_ARRAYSIZE(a) \
    ((sizeof(a) / sizeof(*(a))) / \
    static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#define MAX(a,b) ((a)>(b))?(a):(b)
#define MIN(a,b) ((a)<(b))?(a):(b)
#define DEBUG false

// func:
int resolve(const char* input); // get distance from input
int value(const char* mainStr, const char* subStr); // get val between comma and '\n'
int check(); // check result from 'resolve'
int getMounNum(const char* mainStr, const char* LFPos); // get Mountain num
void getMounData(const int mounCount, int* mounData, const char* subStr); // get Mountain list data 
void getMounEdge(const int mounCount, const int* mounData, int& leftPos, int &rightPos); // get Mountain Edge
void getMounArea(const int mounCount, const int* mounData, int* mounArea, const int rightPos); // get Mountain Area
int getDistance(const int* mounArea, int leftPos, int rightPos); // get distance

int main()
{
	check();
	return 0;
}

int resolve(const char* input)
{	
	int mounCount = 0; // count input mountain num
	int* npMounData; // store mountain data
	int leftPos, rightPos; // left and right pos of mountain
	int *mounArea; // mountain area height info

	// get Mountain Count
	mounCount = getMounNum(input, strchr(input, '\n'));

	// get Mountain data	
	npMounData = new int[3 * mounCount]; // store mountain data
	getMounData(mounCount, npMounData, strchr(input, '\n') + 1); // Mountain data start behind first '\n'

	// left and right pos in Mountain Array		
	getMounEdge(mounCount, npMounData, leftPos, rightPos);

	// get mountain area
	mounArea = new int[rightPos + 1]; // mountain area height info
	getMounArea(mounCount, npMounData, mounArea, rightPos);

	// return distance
	return getDistance(mounArea, leftPos, rightPos);
}

int value(const char* mainStr, const char* subStr){
	int res = 0;
	const char* pMainStr = mainStr;
	const char* pSubStr = subStr;
	// in case mainStr is null
	if (mainStr == NULL)
		return res;
	// in case subStr is null
	if (subStr == NULL)
		pSubStr = pMainStr + strlen(pMainStr);
			
	while (pMainStr < pSubStr){
		res += 10 * res + *(pMainStr++) - '0';
	}
	return res;
}

int getMounNum(const char* mainStr, const char* LFPos) // get Mountain num
{
	return value(mainStr, LFPos);
}

int check()
{
	const char* input[] = {
		"3\n1,3,2\n2,4,4\n6,7,5\n", //The giving example
		"1\n1,2,1\n",
		"2\n1,2,1\n2,3,2",
		"3\n1,2,1\n2,3,2\n3,6,1",
		"4\n1,2,1\n2,3,2\n3,6,1\n5,8,2",
		"5\n1,2,1\n2,3,2\n3,6,1\n5,8,2\n7,9,1",
		"1\n0,1,1",
		"2\n0,1,1\n2,4,3",
		"3\n0,1,1\n2,4,3\n3,5,1",
		"4\n0,1,1\n2,4,3\n3,5,1\n5,6,1",
		"5\n0,1,1\n2,4,3\n3,5,1\n5,6,1\n6,8,3",
		//TODO please add more test case here
	};
	//int expectedSteps[] = { 25, 4, 7, 10, 14, 15, 3, 12, 13, 14, 20 };  // goverment's result contain error.
	int expectedSteps[] = { 25, 4, 7, 10, 14, 15, 3, 10, 11, 12, 18 };
	
	for (size_t i = 0; i < H_ARRAYSIZE(input); ++i)
	{
		assert(resolve(input[i]) == expectedSteps[i]);
	}
	
	// show distance value
	if (DEBUG){
		for (size_t i = 0; i < H_ARRAYSIZE(input); ++i)
		{
			cout << resolve(input[i]) << endl;
		}
	}

	return 0;
}

void getMounData(const int mounCount, int* mounData, const char* subStr) // get Mountain list data
{
	/* divide mounData (3 * mounCount) into 3 section
	 1: left position
	 2: right position
	 3: mountain height*/
	int* npMounData = mounData;
	int* npMounLeftPosArray = npMounData;
	int* npMounRightPosArray = npMounData + mounCount;
	int* npMounHighArray = npMounData + 2 * mounCount;

	// set several str point to separate vals
	const char* cpSubStr = subStr; // sub string 
	const char* cpLFPos; // character '\n' position, use to separate value
	const char* cpCommaPos; // character ',' position, use to separate value

	for (int i = 0; i < mounCount; i++){		
		cpLFPos = strchr(cpSubStr, '\n'); // Next '\n' pos		
		cpCommaPos = strchr(cpSubStr, ',');
		*(npMounLeftPosArray++) = value(cpSubStr, cpCommaPos); // get left pos info
		cpSubStr = cpCommaPos + 1;
		cpCommaPos = strchr(cpSubStr, ',');
		*(npMounRightPosArray++) = value(cpSubStr, cpCommaPos);// get right pos info
		cpSubStr = cpCommaPos + 1;
		// in case no '\n' at last, has solved in value() func
		*(npMounHighArray++) = value(cpSubStr, cpLFPos); // get mountain height info
		cpSubStr = cpLFPos + 1;// update cpSubStr
	}
}

void getMounEdge(const int mounCount, const int* mounData, int& leftPos, int &rightPos)
{
	// mountain edge
	leftPos = *mounData,
	rightPos = 0;

	const int* npMounLeftPosArray = mounData;
	const int* npMounRightPosArray = mounData + mounCount;

	for (int i = 0; i < mounCount; i++)
		leftPos = MIN(*(npMounLeftPosArray + i), leftPos);

	for (int i = 0; i < mounCount; i++)
		rightPos = MAX(*(npMounRightPosArray + i), rightPos);
}

void getMounArea(const int mounCount, const int* mounData, int* mounArea, const int rightPos)
{
	// initial mounArea
	memset(mounArea, 0, sizeof(int)*(rightPos + 1));

	// get mountain surface	
	/*
		MounLeftPos: mounData; MounRightPos: mounData + mounCount; MounHighPos: mounData + 2 * mounCount;
		get mounArea's MAX hight by check each mountain height(leftPos -> rightPos)
	*/
	for (int i = 0; i < mounCount; i++){
		for (int j = *(mounData + i); j <= *(mounData + mounCount + i); j++)
			*(mounArea + j) = MAX(*(mounArea + j), *(mounData + 2 * mounCount + i));
	}
}

int getDistance(const int* mounArea, int leftPos, int rightPos)
{
	// get Maxima Point Position
	// Maxmima Pos character: left side +, while right side -
	vector<int> MaxPos;
	for (int i = leftPos; i < rightPos; i++){
		// needn't care whether rightPos is Maxima Pos, because it must be
		// in case Pos 0 is a mountain
		if (i == 0 && mounArea[0] > mounArea[1]){
			MaxPos.push_back(i);
			continue;
		}
				
		if (mounArea[i] > mounArea[i + 1])
			MaxPos.push_back(i);
	}

	// get distance
	// because vertical distance Sum: 2 * mountain height - 2 * needn't climb height 
	int dis = rightPos + 2 * mounArea[rightPos]; // add horizon distance + last mountain height * 2
	for (vector<int>::iterator iter = MaxPos.begin(); iter != MaxPos.end(); iter++){ // add vertical distance
		dis += 2 * (*(mounArea + *iter) - *(mounArea + *iter + 1));
	}
	
	// show distance
	if (DEBUG)
		cout << "dis: " << dis << endl;

	return dis;
}