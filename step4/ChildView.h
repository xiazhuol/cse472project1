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
	UINT_PTR m_cameraTimer;
	// Construction
public:
	CChildView();

	// Attributes
public:
	int camNum = 1;
	CSphere m_sphere;  //head
	CGrTexture m_sphereTex;

	CMesh m_skybox;
	CGrCubeTexture m_cubeTex;

	CMesh m_body;
	CGrTexture m_bodyTex;

	CSphere m_leftJoint;
	CSphere m_rightJoint;
	CGrTexture m_leftJ;
	CGrTexture m_rightJ;
	
	CMesh m_leftshoulder;
	CMesh m_rightshoulder;
	
	CSphere m_lefteye;
	CSphere m_righteye;
	CGrTexture m_leftTex;
	CGrTexture m_rightTex;


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

	afx_msg void OnFreecamera();
	afx_msg void OnStaticcam();
	afx_msg void OnProject1Rotation2();
};
