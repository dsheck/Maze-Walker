// Maze WalkerDoc.cpp : implementation of the CMazeWalkerDoc class
//

#include "stdafx.h"
#include "Maze Walker.h"

#include "Maze WalkerDoc.h"
#include ".\maze walkerdoc.h"
#include "DimensionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMazeWalkerDoc

IMPLEMENT_DYNCREATE(CMazeWalkerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMazeWalkerDoc, CDocument)
END_MESSAGE_MAP()


// CMazeWalkerDoc construction/destruction

CMazeWalkerDoc::CMazeWalkerDoc()
: m_bFirstRun(true)
{
	// TODO: add one-time construction code here
	m_Maze = new CMaze();
}

CMazeWalkerDoc::~CMazeWalkerDoc()
{
}

BOOL CMazeWalkerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	delete m_Maze;
	m_Maze = new CMaze();
	if( !m_bFirstRun )
	{
		CDimensionDlg dlg;
		dlg.DoModal(*this);
		m_Maze->initMaze();

	}
	m_Maze->reset();

	// Set m_bFirstRun to false, as it the doc is opened
	// this is to avoid the dimension dialog from appearing on first run
	m_bFirstRun = false;

	return TRUE;
}




// CMazeWalkerDoc serialization

void CMazeWalkerDoc::Serialize(CArchive& ar)
{
	int nHeight, nWidth;
	if (ar.IsStoring())
	{
		
		nHeight = m_Maze->getHeight();
		nWidth = m_Maze->getWidth();
		// Write out the dimensions first
		ar.Write(&nHeight, sizeof(int));
		ar.Write(&nWidth, sizeof(int));
		
		// go through the maze and write each block out
		for(int i = 0; i < nHeight; i++)
			for(int j = 0; j < nWidth; j++)
			{
				int nVal = m_Maze->elementAt(i,j);
				if( nVal == 8 || nVal == -1 )
					nVal = 0;
				ar.Write(&nVal, sizeof(int));
			}
	}
	else
	{
		ar.Read(&nHeight, sizeof(int));
		ar.Read(&nWidth, sizeof(int));

		m_Maze->setHeight(nHeight);
		m_Maze->setWidth(nWidth);
		m_Maze->initMaze();

		for(int i = 0; i < nHeight; i++)
			for(int j = 0; j < nWidth; j++)
			{
				int nVal;
				ar.Read(&nVal, sizeof(int));
				if( !m_Maze->setElement(i,j, nVal) )
				{
					CString temp;
					temp.Format("Error Loading Variables: %d,%d",i,j);
					AfxMessageBox(temp);
				}
			}
		
	}
}


// CMazeWalkerDoc diagnostics

#ifdef _DEBUG
void CMazeWalkerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMazeWalkerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMazeWalkerDoc commands

int CMazeWalkerDoc::getMazeWidth(void)
{
	return m_Maze->getWidth();
}

int CMazeWalkerDoc::getMazeHeight(void)
{
	return m_Maze->getHeight();
}

int CMazeWalkerDoc::valueAt(int nX, int nY)
{
	return m_Maze->elementAt(nX,nY);
}

bool CMazeWalkerDoc::doDepthStep(void)
{
	return m_Maze->stepSolve();
}

void CMazeWalkerDoc::reset(void)
{
	m_Maze->reset();
}

bool CMazeWalkerDoc::doBreadthStep(void)
{
	return m_Maze->breadthStep();
}

void CMazeWalkerDoc::toggle(int nX, int nY)
{
	m_Maze->toggle(nX, nY);
}

bool CMazeWalkerDoc::solved(void)
{
	return m_Maze->wasSolved();
}

bool CMazeWalkerDoc::depthSolvable(void)
{
	return m_Maze->solve();
}

bool CMazeWalkerDoc::breadthSolvable(void)
{
	return m_Maze->breadthSolve();
}

int CMazeWalkerDoc::getPosition(void)
{
	return m_Maze->getPosition();
}

bool CMazeWalkerDoc::setWidth(int nWidth)
{
	return m_Maze->setWidth(nWidth);
}

bool CMazeWalkerDoc::setHeight(int nHeight)
{
	return m_Maze->setHeight(nHeight);
}
