// ChildView.h : interface of the CChildView class
//


#pragma once
#include "ShaderWnd\ShaderWnd.h"
#include "Mesh.h"
#include "Sphere.h"
#include "graphics\GrTexture.h"
#include "graphics\GrCubeTexture.h"
#include "graphics\GrCamera.h"

//#include "Torus.h"
// CChildView window

class CChildView : public CShaderWnd
{
private:
	float  m_spinAngle;


	UINT_PTR m_spinTimer;
	// Construction
public:
	CChildView();

	// Attributes
public:
	int camNum = 1;
	CSphere m_sun;
	CGrTexture m_drawSun;

	CSphere m_earth;
	CGrTexture m_drawEarth;

	CSphere m_mercury;
	CGrTexture m_drawMer;

	CSphere m_venus;
	CGrTexture m_drawVenus;

	CSphere m_mars;
	CGrTexture m_drawMars;

	CSphere m_juiter;
	CGrTexture m_drawJupiter;

	CSphere m_saturn;
	CGrTexture m_drawSa;

	CSphere m_uranus;
	CGrTexture m_drawU;

	CSphere m_neptune;
	CGrTexture m_drawNep;


// Operations
public:

	// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void RenderGL();
	virtual void InitGL();
	virtual void CleanGL();

	// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	
	afx_msg void OnProject1Rotation();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnStaticcam();
	afx_msg void OnProject1Rotation2();
};
