// Maze WalkerDoc.h : interface of the CMazeWalkerDoc class
//


#pragma once
#include "maze.h"

class CMazeWalkerDoc : public CDocument
{
protected: // create from serialization only
	CMazeWalkerDoc();
	DECLARE_DYNCREATE(CMazeWalkerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMazeWalkerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
private:
	CMaze *m_Maze;
public:
	int getMazeWidth(void);
	int getMazeHeight(void);
	int valueAt(int nX, int nY);
	bool doDepthStep(void);
	void reset(void);
	bool doBreadthStep(void);
	void toggle(int nX, int nY);
	bool solved(void);
	bool depthSolvable(void);
	bool breadthSolvable(void);
	int getPosition(void);
private:
	bool m_bFirstRun;
public:
	bool setWidth(int nWidth);
	bool setHeight(int nHeight);
};


