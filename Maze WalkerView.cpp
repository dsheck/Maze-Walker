// Maze WalkerView.cpp : implementation of the CMazeWalkerView class
//

#include "stdafx.h"
#include "Maze Walker.h"

#include "Maze WalkerDoc.h"
#include "Maze WalkerView.h"
#include ".\maze walkerview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMazeWalkerView

IMPLEMENT_DYNCREATE(CMazeWalkerView, CFormView)

BEGIN_MESSAGE_MAP(CMazeWalkerView, CFormView)
	ON_BN_CLICKED(IDC_MOVE, OnBnClickedMove)
	ON_BN_CLICKED(IDC_RESET, OnBnClickedReset)
	ON_BN_CLICKED(IDC_DEPTH, OnBnClickedDepth)
	ON_BN_CLICKED(IDC_BREADTH, OnBnClickedBreadth)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_VIEW_EDITMODE, OnViewEditmode)
	ON_COMMAND(ID_VIEW_SOLVEMODE, OnViewSolvemode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITMODE, OnUpdateViewEditmode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SOLVEMODE, OnUpdateViewSolvemode)
	ON_BN_CLICKED(IDC_SOLVABLE, OnBnClickedSolvable)
END_MESSAGE_MAP()

// CMazeWalkerView construction/destruction

CMazeWalkerView::CMazeWalkerView()
	: CFormView(CMazeWalkerView::IDD)
	, m_bDepthFirst(true)
	, m_bEditMode(false)
{
	// TODO: add construction code here

}

CMazeWalkerView::~CMazeWalkerView()
{
}

void CMazeWalkerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CMazeWalkerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CMazeWalkerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_bEditMode = false;
	CButton *button;
	if( m_bDepthFirst )
	{
		button = reinterpret_cast<CButton *>(GetDlgItem(IDC_DEPTH));
		button->SetCheck(1);
		button = reinterpret_cast<CButton *>(GetDlgItem(IDC_BREADTH));
		button->SetCheck(0);
	}
	else
	{
		button = reinterpret_cast<CButton *>(GetDlgItem(IDC_BREADTH));
		button->SetCheck(1);
		button = reinterpret_cast<CButton *>(GetDlgItem(IDC_DEPTH));
		button->SetCheck(0);
	}
}


// CMazeWalkerView diagnostics

#ifdef _DEBUG
void CMazeWalkerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMazeWalkerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMazeWalkerDoc* CMazeWalkerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMazeWalkerDoc)));
	return (CMazeWalkerDoc*)m_pDocument;
}
#endif //_DEBUG


// CMazeWalkerView message handlers

void CMazeWalkerView::OnDraw(CDC* pDC)
{
	// Draw the Maze Outline
	pDC->FillSolidRect(START_X,START_Y, WIDTH, WIDTH, RGB(255,255,255));
	pDC->MoveTo(START_X,START_Y);
	pDC->LineTo(START_X,START_Y+WIDTH);
	pDC->LineTo(START_X+WIDTH, START_Y+WIDTH);
	pDC->LineTo(START_X+WIDTH, START_Y);
	pDC->LineTo(START_X,START_Y);

	// Draw the Maze Contents
	drawGrid();
	refreshMaze();
	drawCharacter();
}

/********************************************************************
 *			drawGrid Function										*
 *																	*
 *  Description: Draws the maze grid based on the size of the maze	*
 *																	*
 *	IN: pDC is the device context to draw to						*
 *	OUT: N/A														*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/21/2003	Don Sheck		Original specification				*
 ********************************************************************/
void CMazeWalkerView::drawGrid()
{
	CDC *pDC = GetDC();
	CMazeWalkerDoc *pDoc = GetDocument();
	int nWidth = pDoc->getMazeWidth();
	int nHeight = pDoc->getMazeHeight();

	pDC->MoveTo(START_X, START_Y);	// move to the upper corner
	for(int i = 0; i < nHeight; i++)
	{
		pDC->MoveTo(START_X, START_Y+((i+1)*WIDTH/nHeight));
		pDC->LineTo(START_X+WIDTH, START_Y+((i+1)*WIDTH/nHeight));
		for(int j = 0; j < nWidth; j++)
		{
			pDC->MoveTo(START_X+((j+1)*WIDTH/nWidth), START_Y);
			pDC->LineTo(START_X+((j+1)*WIDTH/nWidth), START_Y+WIDTH);
		}
	}
}

/********************************************************************
 *			refreshMaze Function									*
 *																	*
 *  Description: Refreshes the contents of the maze					*
 *																	*
 *	IN: N/A															*
 *	OUT: N/A														*
 *																	*
 *	History:														*
 *	Date		Author			Notes								*
 *	--------------------------------------------------------------	*
 *	11/21/2003	Don Sheck		Original specification				*
 ********************************************************************/
void CMazeWalkerView::refreshMaze()
{
	CDC *pDC = GetDC();
	CMazeWalkerDoc *pDoc = GetDocument();
	int nWidth = pDoc->getMazeWidth();
	int nHeight = pDoc->getMazeHeight();

	// Redraw start and finish spots with special color
	pDC->FillSolidRect(START_X+2, START_Y+2, (WIDTH/nWidth)-3, (WIDTH/nHeight)-3, RGB(255,0,0));
	pDC->FillSolidRect(START_X+2+((nHeight-1)*(WIDTH/nHeight)), START_Y+2+((nWidth-1)*WIDTH/nWidth), (WIDTH/nWidth)-3, (WIDTH/nHeight)-3, RGB(0,255,0));

	for(int i = 0; i < nHeight; i++)
	{
		for(int j = 0; j < nWidth; j++)
		{
			int upperX = START_X + (j * WIDTH/nWidth);
			int upperY = START_Y + (i * WIDTH/nHeight);
			
			if( pDoc->valueAt(i,j) == 1 )
			{
				pDC->FillSolidRect(upperX, upperY, WIDTH/nWidth, WIDTH/nHeight, RGB(0,0,0));
			}
			else if( pDoc->valueAt(i,j) == 8 )
			{
				pDC->FillSolidRect(upperX+((WIDTH/nWidth)/2)-3,upperY+((WIDTH/nHeight)/2)-3,3,3,RGB(0,0,255));
			}
			else
			{
				// Empty square
				pDC->FillSolidRect(upperX+((WIDTH/nWidth)/2)-3,upperY+((WIDTH/nHeight)/2)-3, 4, 4, RGB(255,255,255));
			}
		}
	}
}

void CMazeWalkerView::OnBnClickedMove()
{
	CMazeWalkerDoc *pDoc = GetDocument();
	int nChecked = GetCheckedRadioButton(IDC_DEPTH, IDC_BREADTH);
	if(nChecked == IDC_DEPTH)
	{
		if(pDoc->doDepthStep())
		{
			if(pDoc->solved())
				AfxMessageBox("Maze was solved!");
			else
				AfxMessageBox("No Solution!");
			GetDlgItem(IDC_MOVE)->EnableWindow(false);
		}
	}
	else
	{
		if(pDoc->doBreadthStep())
		{
			if(pDoc->solved())
				AfxMessageBox("Maze was solved!");
			else
				AfxMessageBox("No Solution!");
			GetDlgItem(IDC_MOVE)->EnableWindow(false);
		}
	}
	Invalidate();
}

void CMazeWalkerView::OnBnClickedReset()
{
	CMazeWalkerDoc *pDoc = GetDocument();
	pDoc->reset();
	GetDlgItem(IDC_MOVE)->EnableWindow(true);
	Invalidate();
}

void CMazeWalkerView::OnBnClickedDepth()
{
	CMazeWalkerDoc *pDoc = GetDocument();
	pDoc->reset();
	Invalidate();
}

void CMazeWalkerView::OnBnClickedBreadth()
{
	CMazeWalkerDoc *pDoc = GetDocument();
	pDoc->reset();
	Invalidate();
}

void CMazeWalkerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Check to see if we are in Edit mode
	if( !m_bEditMode )
		return;

	CMazeWalkerDoc *pDoc = GetDocument();
	
	if( (point.x >= START_X && point.x <= START_X+WIDTH) &&
		(point.y >= START_Y && point.y <= START_Y+WIDTH) )
	{
		// We are within the grid area.. now we have to figure out which square we are
		// actually pointing at.
		int sizeOfX = WIDTH / pDoc->getMazeWidth();
		int sizeOfY = WIDTH / pDoc->getMazeHeight();
		int locX = (point.x / sizeOfX);
		int locY = (point.y / sizeOfY);
		if( locX == 0 && locY == 0 )
			AfxMessageBox("You can't block the starting position");
		else
			pDoc->toggle(locX, locY);
		Invalidate();
	}
}

void CMazeWalkerView::OnViewEditmode()
{
	if( m_bEditMode )
		return;	// Edit Mode already enabled, leave function

	CMazeWalkerDoc *pDoc = GetDocument();
	m_bEditMode = true;

	pDoc->reset();
	// disable solving controls here
	GetDlgItem(IDC_DEPTH)->EnableWindow(false);
	GetDlgItem(IDC_BREADTH)->EnableWindow(false);
	GetDlgItem(IDC_MOVE)->EnableWindow(false);
	GetDlgItem(IDC_RESET)->EnableWindow(false);
}

void CMazeWalkerView::OnViewSolvemode()
{
	if( !m_bEditMode )
		return;	// Edit Mode already enabled, leave function

	CMazeWalkerDoc *pDoc = GetDocument();
	m_bEditMode = false;

	pDoc->reset();
	// Enable solving controls here
	GetDlgItem(IDC_DEPTH)->EnableWindow(true);
	GetDlgItem(IDC_BREADTH)->EnableWindow(true);
	GetDlgItem(IDC_MOVE)->EnableWindow(true);
	GetDlgItem(IDC_RESET)->EnableWindow(true);
}

void CMazeWalkerView::OnUpdateViewEditmode(CCmdUI *pCmdUI)
{
	// Update the UI based on whether edit mode is on, or not
	if( m_bEditMode )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMazeWalkerView::OnUpdateViewSolvemode(CCmdUI *pCmdUI)
{
	// Update the UI based on whether edit mode is on, or not
	if( !m_bEditMode )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMazeWalkerView::OnBnClickedSolvable()
{
	// Check to see if the maze is solvable in either algorithm
	CMazeWalkerDoc *pDoc = GetDocument();
	int nChecked = GetCheckedRadioButton(IDC_DEPTH, IDC_BREADTH);
	if(nChecked == IDC_DEPTH)
	{
		if( pDoc->depthSolvable() )
			AfxMessageBox("Depth First Search method is Solvable");
		else
			AfxMessageBox("Depth First Search method can find no solution");
	}
	else
	{
		if( pDoc->breadthSolvable() )
			AfxMessageBox("Breadth First Search method is Solvable");
		else
			AfxMessageBox("Breadth First Search method can find no solution");
	}
	pDoc->reset();
}

void CMazeWalkerView::drawCharacter(void)
{
	CDC *pDC = GetDC();
	CMazeWalkerDoc *pDoc = GetDocument();
	int nHeight = pDoc->getMazeHeight(), nWidth = pDoc->getMazeWidth();
	int pos = pDoc->getPosition();
	int nY = pos / 10000;
	pos -= nY * 10000;
	
	int upperX = START_X + (pos * WIDTH/nWidth);
	int upperY = START_Y + (nY * WIDTH/nHeight);
	
	CBitmap walker;
	walker.LoadBitmap(IDB_WALKER);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(walker);
	pDC->TransparentBlt(upperX+((WIDTH/nWidth)/2)-5,upperY+((WIDTH/nWidth)/2)-5,8,8, &memDC, 0, 0,8,8, 0x00ff00ff);
	//pDC->TransparentBlt(START_Y+pos*(WIDTH/nHeight),START_X+nX*(WIDTH/nWidth),10,10, &memDC, 0,0,10,10,RGB(255,0,255));
	//pDC->FillSolidRect(START_Y+pos*(WIDTH/nHeight),START_X+nX*(WIDTH/nWidth),10,6,RGB(255,0,0));
}
