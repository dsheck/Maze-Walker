// Maze WalkerView.h : interface of the CMazeWalkerView class
//


#pragma once

// Definitions used to draw map
#define START_X	10
#define START_Y	10
#define WIDTH	300

class CMazeWalkerView : public CFormView
{
protected: // create from serialization only
	CMazeWalkerView();
	DECLARE_DYNCREATE(CMazeWalkerView)

public:
	enum{ IDD = IDD_MAZEWALKER_FORM };

// Attributes
public:
	CMazeWalkerDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMazeWalkerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
public:
	void drawGrid();
	void refreshMaze();
private:
	bool m_bDepthFirst;
public:
	afx_msg void OnBnClickedMove();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedDepth();
	afx_msg void OnBnClickedBreadth();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewEditmode();
private:
	bool m_bEditMode;
public:
	afx_msg void OnViewSolvemode();
	afx_msg void OnUpdateViewEditmode(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewSolvemode(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedSolvable();
	void drawCharacter(void);
};

#ifndef _DEBUG  // debug version in Maze WalkerView.cpp
inline CMazeWalkerDoc* CMazeWalkerView::GetDocument() const
   { return reinterpret_cast<CMazeWalkerDoc*>(m_pDocument); }
#endif

