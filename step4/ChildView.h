
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "ShaderWnd\ShaderWnd.h"
#include "Mesh.h"
#include "Sphere.h"
#include "graphics\GrTexture.h"
#include "graphics\GrCubeTexture.h"
//#include "Torus.h"
// CChildView window

class CChildView : public CShaderWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	CGrTexture m_catTex;
	CGrTexture m_sphereTex;
	CGrCubeTexture m_cubeTex;
	//CMesh m_cat;
	CSphere m_sphere;
	//CSphere m_metallicSphere;
	CMesh m_skybox;
	CGrTexture m_marble1;
	//CTorus m_tori1;
	//CMesh m_letter;
	//CGrCubeTexture m_letterTex;
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
};

