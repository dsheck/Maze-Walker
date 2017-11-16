#include "StdAfx.h"
#include ".\maze.h"

/********************************************************************
 *			Default CMaze Constructor								*
 *																	*
 *  Description: Constructs a CMaze object with a default size of 	*
 *	10 by 10 squares.												*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/06/2003	Don Sheck		Original specification				*
 ********************************************************************/
CMaze::CMaze(void)
: m_bSolved(false),
m_nHeight(10),
m_nWidth(10)
, m_nCurrPosX(0)
, m_nCurrPosY(0)
{
	// Default Constructor
	m_Maze = new int[m_nHeight * m_nWidth];

	// Initialize all elements
	for(int i = 0; i < m_nHeight; i++)
		for(int j = 0; j < m_nWidth; j++)
			m_Maze[i*m_nWidth + j] = 0;
}

/********************************************************************
 *			Overloaded CMaze Constructor							*
 *																	*
 *  Description: Constructs a CMaze object using the given filename	*
 *																	*
 *	IN: szFilename is the filename that must be loaded				*
 *	OUT: N/A														*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/07/2003	Don Sheck		Original specification				*
 ********************************************************************/
CMaze::CMaze(char* szFilename)
{
	m_bSolved = false;
	
	// Init memory for m_Maze
	fstream in(szFilename);
	in >> m_nHeight;
	in.ignore(2);
	in >> m_nWidth;
	m_Maze = new int[m_nHeight * m_nWidth];
	in.close();

	// Load Maze
	load(szFilename);
}

/********************************************************************
 *			Overloaded CMaze Constructor							*
 *																	*
 *  Description: Constructs a CMaze object using the given height 	*
 *	and width.														*
 *																	*
 *	IN: nHeight and nWidth are the dimensions of the CMaze			*
 *	OUT: N/A														*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/23/2003	Don Sheck		Original specification				*
 ********************************************************************/
CMaze::CMaze(int nHeight, int nWidth)
{
	m_bSolved = false;
	m_nCurrPosX = 0;
	m_nCurrPosY = 0;
	m_nHeight = nHeight;
	m_nWidth = nWidth;
	initMaze();
}

CMaze::~CMaze(void)
{
	delete [] m_Maze;
}

/********************************************************************
 *			Overloaded Ostream Operator <<							*
 *																	*
 *  Description: Allows the maze to be output to a console app or 	*
 *	other ostream device											*
 *																	*
 *	IN: os is the ostream object and maze is the CMaze to output	*
 *	OUT: returns the ostream object to allow: cout << maze1 << maze2*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/06/2003	Don Sheck		Original specification				*
 ********************************************************************/
ostream& operator<<(ostream &os, CMaze &maze)
{
	for(int i = 0; i < maze.m_nHeight; i++)
	{
		for(int j = 0; j < maze.m_nWidth; j++)
			os << maze.elementAt(i, j) << " ";
		os << endl;
	}
	os << endl;
	return os;
}

/********************************************************************
 *			load Function											*
 *																	*
 *  Description: Loads a maze from a file							*
 *																	*
 *	IN: filename is the name of the file to open					*
 *	OUT: Returns true if the file loaded successfully				*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/06/2003	Don Sheck		Original specification				*
 ********************************************************************/
bool CMaze::load(char* filename)
{
	// Open a file for reading
	ifstream inputFile(filename, ios::in);
	if( !inputFile )
		return false; // error, file didn't open

	inputFile >> m_nHeight;
	inputFile.ignore(2);// ignore ', '
	inputFile >> m_nWidth;
	
	for(int i = 0; i < m_nHeight; i++)
	{
		for(int j = 0; j < m_nWidth; j++)
			inputFile >> m_Maze[i*m_nWidth + j];
		inputFile.ignore(); // ignore newline
	}

	inputFile.close();

	return true;
}

/********************************************************************
 *			elementAt Function										*
 *																	*
 *  Description: Returns the value of the element (or square) at 	*
 *	nRow and nCol in the maze.										*
 *																	*
 *	IN: nRow, nCol is the row and column we want to look at			*
 *	OUT: Returns the value of the maze at nRow and nCol				*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/06/2003	Don Sheck		Original specification				*
 ********************************************************************/
int CMaze::elementAt(int nRow, int nCol)
{
	if( nRow < m_nHeight && nCol < m_nWidth )
		return m_Maze[nRow*m_nWidth + nCol];
	else
		return -1;
}

/********************************************************************
 *			solve Function											*
 *																	*
 *  Description: Solves the maze and stores the final path in the	*
 *	m_PrevMoves list.												*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns true if a solution is found						*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/06/2003	Don Sheck		Original specification				*
 *	11/07/2003	Don Sheck		Tidied up the code, made visited	*
 *								list a member var for stepSolve.	*
 *	11/24/2003	Don Sheck		Optimized the algorithm by removing	*
 *								usage of isPreviousMove()			*
 ********************************************************************/
bool CMaze::solve(void)
{
	reset();
	m_visited.push_back(0);

	while( !m_visited.empty() && !((m_nCurrPosY == m_nHeight-1) && (m_nCurrPosX == m_nWidth-1)) )
	{
		int temp = m_visited.back();
		m_visited.pop_back();
		m_nCurrPosY = temp / 10000;
		m_nCurrPosX = temp - (m_nCurrPosY * 10000);

		if( canMove( m_nCurrPosY-1, m_nCurrPosX ) ) // north
		{
			m_visited.push_back((m_nCurrPosY-1)*10000+m_nCurrPosX);
			m_Maze[(m_nCurrPosY-1)*m_nWidth+m_nCurrPosX] = -1;
		}
		if( canMove( m_nCurrPosY, m_nCurrPosX-1 ) ) // west
		{
			m_visited.push_back(m_nCurrPosY*10000+m_nCurrPosX-1);
			m_Maze[m_nCurrPosY*m_nWidth+(m_nCurrPosX-1)] = -1;
		}
		if( canMove( m_nCurrPosY+1, m_nCurrPosX ) ) // south
		{
			m_visited.push_back((m_nCurrPosY+1)*10000+m_nCurrPosX);
			m_Maze[(m_nCurrPosY+1)*m_nWidth+m_nCurrPosX] = -1;
		}
		if( canMove( m_nCurrPosY, m_nCurrPosX+1 ) ) // east
		{
			m_visited.push_back(m_nCurrPosY*10000+m_nCurrPosX+1);
			m_Maze[m_nCurrPosY*m_nWidth+(m_nCurrPosX+1)] = -1;
		}
		
	}

	return ((m_nCurrPosY == m_nHeight-1) && (m_nCurrPosX == m_nWidth-1));
}

/********************************************************************
 *			canMove Function										*
 *																	*
 *  Description: Returns true if the given location is movable		*
 *																	*
 *	IN: nRow and nCol is the location to test						*
 *	OUT: Returns true if location is a valid move					*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/06/2003	Don Sheck		Original specification				*
 ********************************************************************/
bool CMaze::canMove(int nRow, int nCol)
{
	if( nRow >= m_nHeight || nCol >= m_nWidth )
		return false;
	else if( nRow < 0 || nCol < 0 )
		return false;
	else
	{
		if( m_Maze[nRow*m_nWidth + nCol] == 0 )
			return true;
		else
			return false;
	}
}

/********************************************************************
 *			isPreviousMove Function									*
 *																	*
 *  Description: Returns true if the given move was a previous move	*
 *																	*
 *	IN: nMove is the move to test									*
 *	OUT: Returns true if location is a previous move				*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/06/2003	Don Sheck		Original specification				*
 ********************************************************************/
bool CMaze::isPreviousMove(int nMove)
{
	list<int>::iterator listIterator = m_PrevMoves.begin();
	while( listIterator != m_PrevMoves.end() )
	{
		if( *listIterator == nMove )
			return true;
		listIterator++;
	}
	return false;
}

/********************************************************************
 *			stepSolve Function										*
 *																	*
 *  Description: Advances through the steps to solve the maze one	*
 *	step at a time.													*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns true if the maze is solved at the end of this step	*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/07/2003	Don Sheck		Original specification				*
 *	11/24/2003	Don Sheck		Optimized the algorithm by removing	*
 *								usage of isPreviousMove()			*
 ********************************************************************/
bool CMaze::stepSolve(void)
{
	int temp;

	if( m_visited.empty() )	// First time through list
		m_visited.push_back(0);

	temp = m_visited.back();
	m_visited.pop_back();
	m_nCurrPosY = temp / 10000;
	m_nCurrPosX = temp - (m_nCurrPosY * 10000);

	if( canMove( m_nCurrPosY-1, m_nCurrPosX ) ) // north
	{
		m_visited.push_back((m_nCurrPosY-1)*10000+m_nCurrPosX);
		m_Maze[(m_nCurrPosY-1)*m_nWidth+m_nCurrPosX] = -1;
	}
	if( canMove( m_nCurrPosY, m_nCurrPosX-1 ) ) // west
	{
		m_visited.push_back(m_nCurrPosY*10000+m_nCurrPosX-1);
		m_Maze[m_nCurrPosY*m_nWidth+(m_nCurrPosX-1)] = -1;
	}
	if( canMove( m_nCurrPosY+1, m_nCurrPosX ) ) // south
	{
		m_visited.push_back((m_nCurrPosY+1)*10000+m_nCurrPosX);
		m_Maze[(m_nCurrPosY+1)*m_nWidth+m_nCurrPosX] = -1;
	}
	if( canMove( m_nCurrPosY, m_nCurrPosX+1 ) ) // east
	{
		m_visited.push_back(m_nCurrPosY*10000+m_nCurrPosX+1);
		m_Maze[m_nCurrPosY*m_nWidth+(m_nCurrPosX+1)] = -1;
	}

	if( (m_nCurrPosY == m_nHeight-1) && (m_nCurrPosX == m_nWidth-1) )
	{
		m_bSolved = true;
	}

	m_Maze[m_nCurrPosY*m_nWidth+m_nCurrPosX] = 8;

	return ( ((m_nCurrPosY == m_nHeight-1) && (m_nCurrPosX == m_nWidth-1)) || m_visited.empty() );
}

/********************************************************************
 *			wasSolved Function										*
 *																	*
 *  Description: Returns the value of the m_bSolved Variable. That	*
 *	is, if the maze was solved, m_bSolved = true.					*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns true if the maze was solved.						*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/07/2003	Don Sheck		Original specification				*
 ********************************************************************/
bool CMaze::wasSolved(void)
{
	return m_bSolved;
}

/********************************************************************
 *			reset Function											*
 *																	*
 *  Description: Resets the lists to empty							*
 *																	*
 *	IN: N/A															*
 *	OUT: N/A														*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/07/2003	Don Sheck		Original specification				*
 ********************************************************************/
void CMaze::reset(void)
{
	m_PrevMoves.clear();
	m_visited.clear();
	m_bSolved = false;

	m_nCurrPosX = 0;
	m_nCurrPosY = 0;

	for(int i = 0; i < m_nHeight; i++)
		for(int j = 0; j < m_nWidth; j++)
			if(m_Maze[i*m_nWidth+j] == 8 || m_Maze[i*m_nWidth+j] == -1)
				m_Maze[i*m_nWidth+j] = 0;
}

/********************************************************************
 *			breadthSolve Function									*
 *																	*
 *  Description: Solves the maze using a breadth-first search		*
 *	approach														*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns true if the maze was solvable						*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/07/2003	Don Sheck		Original specification				*
 *	11/24/2003	Don Sheck		Optimized the algorithm by removing	*
 *								usage of isPreviousMove()			*
 ********************************************************************/
bool CMaze::breadthSolve(void)
{
	reset();
	m_visited.push_back(0);

	while( !m_visited.empty() && !((m_nCurrPosY == m_nHeight-1) && (m_nCurrPosX == m_nWidth-1)) )
	{
		// Pull from the front, since this is breadth first
		int temp = m_visited.front();
		m_visited.pop_front();
		m_nCurrPosY = temp / 10000;
		m_nCurrPosX = temp - (m_nCurrPosY * 10000);

		if( canMove( m_nCurrPosY, m_nCurrPosX+1 ) ) // east
		{
			m_visited.push_back(m_nCurrPosY*10000+m_nCurrPosX+1);
			m_Maze[m_nCurrPosY*m_nWidth+(m_nCurrPosX+1)] = -1;
		}
		if( canMove( m_nCurrPosY+1, m_nCurrPosX ) ) // south
		{
			m_visited.push_back((m_nCurrPosY+1)*10000+m_nCurrPosX);
			m_Maze[(m_nCurrPosY+1)*m_nWidth+m_nCurrPosX] = -1;
		}
		if( canMove( m_nCurrPosY, m_nCurrPosX-1 ) ) // west
		{
			m_visited.push_back(m_nCurrPosY*10000+m_nCurrPosX-1);
			m_Maze[m_nCurrPosY*m_nWidth+(m_nCurrPosX-1)] = -1;
		}
		if( canMove( m_nCurrPosY-1, m_nCurrPosX ) ) // north
		{
			m_visited.push_back((m_nCurrPosY-1)*10000+m_nCurrPosX);
			m_Maze[(m_nCurrPosY-1)*m_nWidth+m_nCurrPosX] = -1;
		}
	}

	return ((m_nCurrPosY == m_nHeight-1) && (m_nCurrPosX == m_nWidth-1));
}

/********************************************************************
 *			breadthStep Function									*
 *																	*
 *  Description: Advances through the steps of a Breath First Search*
 *	approach to solving the maze.									*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns true if the maze was solvable						*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/07/2003	Don Sheck		Original specification				*
 *	11/20/2003	Don Sheck		Reordered the push sequence onto the*
 *								queue for better performance		*
 *	11/24/2003	Don Sheck		Optimized the algorithm by removing	*
 *								usage of isPreviousMove()			*
 ********************************************************************/
bool CMaze::breadthStep(void)
{
	int temp;

	if( m_visited.empty() )	// First time through list
		m_visited.push_back(0);

	temp = m_visited.front();
	m_visited.pop_front();
	m_nCurrPosY = temp / 10000;
	m_nCurrPosX = temp - (m_nCurrPosY * 10000);
	//m_PrevMoves.push_back(m_nCurrPosY*10000+m_nCurrPosX);
	
	if( canMove( m_nCurrPosY, m_nCurrPosX+1 ) )//&& !isPreviousMove((m_nCurrPosY*10000)+m_nCurrPosX+1) ) // east
	{
		m_visited.push_back(m_nCurrPosY*10000+m_nCurrPosX+1);
		m_Maze[m_nCurrPosY*m_nWidth+(m_nCurrPosX+1)] = -1;
	}
	if( canMove( m_nCurrPosY+1, m_nCurrPosX ) )//&& !isPreviousMove((m_nCurrPosY+1)*10000+m_nCurrPosX) ) // south
	{
		m_visited.push_back((m_nCurrPosY+1)*10000+m_nCurrPosX);
		m_Maze[(m_nCurrPosY+1)*m_nWidth+m_nCurrPosX] = -1;
	}
	if( canMove( m_nCurrPosY, m_nCurrPosX-1 ) )//&& !isPreviousMove((m_nCurrPosY*10000)+m_nCurrPosX-1) ) // west
	{
		m_visited.push_back(m_nCurrPosY*10000+m_nCurrPosX-1);
		m_Maze[m_nCurrPosY*m_nWidth+(m_nCurrPosX-1)] = -1;
	}
	if( canMove( m_nCurrPosY-1, m_nCurrPosX ) )//&& !isPreviousMove((m_nCurrPosY-1)*10000+m_nCurrPosX) ) // north
	{
		m_visited.push_back((m_nCurrPosY-1)*10000+m_nCurrPosX);
		m_Maze[(m_nCurrPosY-1)*m_nWidth+m_nCurrPosX] = -1;
	}
	
	if( (m_nCurrPosY == m_nHeight-1) && (m_nCurrPosX == m_nWidth-1) )
	{
		m_bSolved = true;
	}

	m_Maze[m_nCurrPosY*m_nWidth+m_nCurrPosX] = 8;

	return ( ((m_nCurrPosY == m_nHeight-1) && (m_nCurrPosX == m_nWidth-1)) || m_visited.empty() );
}

/********************************************************************
 *			getWidth Function										*
 *																	*
 *  Description: Returns the width of the maze						*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns the width of the maze								*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/17/2003	Don Sheck		Original specification				*
 ********************************************************************/
int CMaze::getWidth(void)
{
	return m_nWidth;
}

/********************************************************************
 *			getHeight Function										*
 *																	*
 *  Description: Returns the height of the maze						*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns the height of the maze								*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/17/2003	Don Sheck		Original specification				*
 ********************************************************************/
int CMaze::getHeight(void)
{
	return m_nHeight;
}

/********************************************************************
 *			setWidth Function										*
 *																	*
 *  Description: Sets the width of the maze.						*
 *																	*
 *	IN: nCols is the number of columns to set the maze to			*
 *	OUT: Returns true if the maze was set to a valid value			*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/21/2003	Don Sheck		Original specification				*
 *	11/23/2003	Don Sheck		Fixed a logic error and changed the *
 *								max # of columns to 50				*
 ********************************************************************/
bool CMaze::setWidth(int nCols)
{
	if(nCols > 30 || nCols < 4 )
		return false;
	else
	{
		m_nWidth = nCols;
		return true;
	}
}

/********************************************************************
 *			setHeight Function										*
 *																	*
 *  Description: Sets the height of the maze.						*
 *																	*
 *	IN: nRows is the number of rows to set the maze to				*
 *	OUT: Returns true if the maze was set to a valid value			*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/21/2003	Don Sheck		Original specification				*
 *	11/23/2003	Don Sheck		Changed max rows to 50				*
 ********************************************************************/
bool CMaze::setHeight(int nRows)
{
	if( nRows > 30 || nRows < 4 )
		return false;
	else
	{
		m_nHeight = nRows;
		return true;
	}
}

/********************************************************************
 *			initMaze Function										*
 *																	*
 *  Description: Reinitializes the maze to all 0s. Should be used 	*
 *	after the height or width of the maze is changed				*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns true if the maze was initialized properly			*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/21/2003	Don Sheck		Original specification				*
 ********************************************************************/
bool CMaze::initMaze(void)
{
	// delete the current maze
	delete [] m_Maze;
	m_Maze = new int[m_nHeight * m_nWidth];
	for(int i = 0; i < m_nHeight; i++)
		for(int j = 0; j < m_nWidth; j++)
			m_Maze[i*m_nWidth+j] = 0;
	return true;
}

/********************************************************************
 *			toggle Function											*
 *																	*
 *  Description: Toggles a wall into an open space in the maze and	*
 *	vice versa.														*
 *																	*
 *	IN: nX and nY is the location in the grid to toggle.			*
 *	OUT: N/A														*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/23/2003	Don Sheck		Original specification				*
 ********************************************************************/
void CMaze::toggle(int nX, int nY)
{
	if( m_Maze[nY*m_nWidth+nX] == 0 && !(nX == 0 && nY == 0) )
		m_Maze[nY*m_nWidth+nX] = 1;
	else
		m_Maze[nY*m_nWidth+nX] = 0;
}

/********************************************************************
 *			setElement Function										*
 *																	*
 *  Description: Changes the value of a position in the maze array	*
 *																	*
 *	IN: nRow and nCol are the coords and nVal is the value to change*
 *	the position to.												*
 *	OUT: N/A														*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/23/2003	Don Sheck		Original specification				*
 ********************************************************************/
bool CMaze::setElement(int nRow, int nCol, int nVal)
{
	// if submitted row and col are outside of bounds, return false
	if( !(nRow >= 0 && nRow < m_nHeight) || !(nCol >= 0 && nCol < m_nWidth) ) 
		return false;

	// Make sure they can only set it to a 1 or 0
	if( nVal != 0 && nVal != 1 )
		nVal = 0;

	m_Maze[nRow*m_nWidth+nCol] = nVal;
	return true;
}

/********************************************************************
 *			getPosition Function									*
 *																	*
 *  Description: Returns the current position of the maze			*
 *																	*
 *	IN: N/A															*
 *	OUT: Returns the current position as a single integer.			*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/23/2003	Don Sheck		Original specification				*
 ********************************************************************/
int CMaze::getPosition(void)
{
	return m_nCurrPosY*10000+m_nCurrPosX;
}
