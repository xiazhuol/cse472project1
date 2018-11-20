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
	CGrTexture m_bodyTex;
	CGrTexture m_sphereTex;
	CGrCubeTexture m_cubeTex;
	//CMesh m_cat;
	CSphere m_sphere;
	//CSphere m_metallicSphere;
	CMesh m_skybox;
	//CTorus m_tori1;
	CMesh m_body;
	CSphere m_leftJoint;
	CSphere m_rightJoint;
	CMesh m_leftshoulder;
	CMesh m_rightshoulder;
	CGrCamera m_camera;

	//CMesh m_letter2;
	//CGrCubeTexture m_letterTex2;
	//CMesh m_letter3;
	//CGrCubeTexture m_letterTex3;

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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
