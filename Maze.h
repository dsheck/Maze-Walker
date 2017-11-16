#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <iterator>

using namespace std;

class CMaze
{
friend ostream& operator<<(ostream&, CMaze&);

private:
	int m_nWidth;
	int m_nHeight;
	int* m_Maze;
	bool m_bSolved;
	list<int> m_PrevMoves;
	list<int> m_visited;

public:
	CMaze(void);
	CMaze(char* szFilename);
	CMaze(int nHeight, int nWidth);
	~CMaze(void);
	
	bool load(char* filename);
	int elementAt(int nRow, int nCol);
	bool solve(void);
	bool canMove(int nRow, int nCol);
	bool isPreviousMove(int nMove);
	bool stepSolve(void);
	bool wasSolved(void);
	void reset(void);
	bool breadthSolve(void);
	bool breadthStep(void);
	int getWidth(void);
	int getHeight(void);
private:
	int m_nCurrPosX;
	int m_nCurrPosY;
public:
	bool setWidth(int nCols);
	bool setHeight(int nRows);
	bool initMaze(void);
	void toggle(int nX, int nY);
	bool setElement(int nRow, int nCol, int nVal);
	int getPosition(void);
};
