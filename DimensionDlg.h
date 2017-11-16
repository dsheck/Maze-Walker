#pragma once

#include "Maze WalkerDoc.h"
// CDimensionDlg dialog

class CDimensionDlg : public CDialog
{
	DECLARE_DYNAMIC(CDimensionDlg)

public:
	CDimensionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDimensionDlg();

// Dialog Data
	enum { IDD = IDD_DIMENSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int m_nHeight;
	int m_nWidth;
public:
	afx_msg void OnBnClickedOk();
	virtual INT_PTR DoModal(CMazeWalkerDoc&);
private:
	CMazeWalkerDoc *theDoc;
};
