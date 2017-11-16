// Maze Walker.h : main header file for the Maze Walker application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CMazeWalkerApp:
// See Maze Walker.cpp for the implementation of this class
//

class CMazeWalkerApp : public CWinApp
{
public:
	CMazeWalkerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnUpdateViewEditmode(CCmdUI *pCmdUI);
};

extern CMazeWalkerApp theApp;