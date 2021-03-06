
// step4.h : main header file for the step4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cstep4App:
// See step4.cpp for the implementation of this class
//

class Cstep4App : public CWinApp
{
public:
	Cstep4App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cstep4App theApp;
