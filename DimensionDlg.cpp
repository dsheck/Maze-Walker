// DimensionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Maze Walker.h"
#include "DimensionDlg.h"
#include ".\dimensiondlg.h"


// CDimensionDlg dialog

IMPLEMENT_DYNAMIC(CDimensionDlg, CDialog)
CDimensionDlg::CDimensionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDimensionDlg::IDD, pParent)
	, m_nHeight(0)
	, m_nWidth(0)
	, theDoc(NULL)
{
}

CDimensionDlg::~CDimensionDlg()
{
}

void CDimensionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
}


BEGIN_MESSAGE_MAP(CDimensionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDimensionDlg message handlers

void CDimensionDlg::OnBnClickedOk()
{
	UpdateData(true);
	if( (m_nHeight > 3 && m_nHeight <= 30) && (m_nWidth > 3 && m_nWidth <= 30) )
	{
		// valid
		if( theDoc->setHeight(m_nHeight) && theDoc->setWidth(m_nWidth) )
			AfxMessageBox("Dimensions set correctly!");
		OnOK();
	}
	else
	{
		AfxMessageBox("Incorrect input values for the width or height.");
	}
}

INT_PTR CDimensionDlg::DoModal(CMazeWalkerDoc &doc)
{
	theDoc = &doc;

	return CDialog::DoModal();
}
