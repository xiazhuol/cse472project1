// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "step4.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()

{
	m_spinAngle = 0;
	m_spinTimer = 0; 

	m_vEye = vec3(30, 20, 50);
	m_vLookat = vec3(0, 0, 0);
	m_vUp = vec3(0, 1, 0);

	
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CShaderWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_PROJECT1_ROTATION, &CChildView::OnProject1Rotation)
	ON_WM_TIMER()
	ON_COMMAND(ID_PROJECT1_FREECAMERA, &CChildView::OnFreecamera)
	ON_COMMAND(ID_PROJECT1_STATICCAM, &CChildView::OnStaticcam)
	ON_COMMAND(ID_PROJECT1_ROTATION2, &CChildView::OnProject1Rotation2)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CShaderWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

void CChildView::InitGL()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	/*Bump Map a Sphere*/
	m_sphereTex.LoadFile(L"textures/marble10.bmp");
	m_sphere.SetRadius(3);
	m_sphere.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere2.glsl");
	m_sphere.InitGL();
	/*Sky Box*/
	m_cubeTex.LoadFiles(L"textures/right.jpg", L"textures/left.jpg", L"textures/top.jpg",
		L"textures/bottom.jpg", L"textures/front.jpg", L"textures/back.jpg");

	m_skybox.CreateCube();
	m_skybox.m_program = LoadShaders("ShaderWnd/vertexSky.glsl", "ShaderWnd/fragmentSky.glsl");
	m_skybox.InitGL();
	//test that I can update 
	/*body*/
	m_bodyTex.LoadFile(L"textures/marble10.bmp");
	m_body.Createbody();
	m_body.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_body.InitGL();

	/*left joint*/
	m_leftJ.LoadFile(L"textures/bumpmap.jpg");
	m_leftJoint.SetRadius(1.5);
	m_leftJoint.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere2.glsl");
	m_leftJoint.InitGL();

	/*right joint*/
	m_rightJ.LoadFile(L"textures/bumpmap.jpg");
	m_rightJoint.SetRadius(1.5);
	m_rightJoint.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere2.glsl");
	m_rightJoint.InitGL();

	/*leftshoulder*/
	m_leftshoulder.CreateLeftshoulder();
	m_leftshoulder.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_leftshoulder.InitGL();

	/*rightshoulder*/
	m_rightshoulder.CreateRightshoulder();
	m_rightshoulder.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_rightshoulder.InitGL();
	
	/*lefteye*/

	m_leftTex.LoadFile(L"textures/haha2.jpg");
	m_lefteye.SetRadius(1.);
	m_lefteye.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_lefteye.InitGL();

	/*righteye*/
	m_rightTex.LoadFile(L"textures/haha2.jpg");
	m_righteye.SetRadius(1.);
	m_righteye.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_righteye.InitGL();

}

void CChildView::RenderGL()
{
	
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//
// Set up the camera
//

	int width, height;
	RECT rect;
	GetWindowRect(&rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;







	// below: for texture
	glm::vec4 light_position(0.f, 0.f, 0.f, 0.f);
	glm::vec4 light_ambient(0.2f, 0.2f, 0.2f, 1.f);
	glm::vec4 light_diffuse(1.f, 1.f, 1.f, 1.f);
	glm::vec4 light_specular(1.f, 1.f, 1.f, 1.f);

	glm::vec4 material_ambient(1.f, 1.f, 1.f, 1.f);
	glm::vec4 material_diffuse(1.f, 1.f, 1.f, 1.f);
	glm::vec4 material_specular(1.f, 1.f, 1.f, 1.f);
	float material_shininess = 100.0f;

	glm::vec4 ambient_product = light_ambient * material_ambient;
	glm::vec4 diffuse_product = light_diffuse * material_diffuse;
	glm::vec4 specular_product = light_specular * material_specular;

	/*Bump Map a Sphere*/
	m_program = m_sphere.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M = translate(mat4(1.f), vec3(0., 5., 0.))
		* rotate(mat4(1.f), m_spinAngle, vec3(1., 0., 0.));
	glm::mat4 VM = m_mVM * M;
	glm::mat4 PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sphereTex.TexName());

	m_sphere.RenderGL();
	/*Sky Box*/
	m_program = m_skybox.m_program;
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "skybox"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mP");
	m_nVM = glGetUniformLocation(m_program, "mV");
	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(m_mProjection));
	glm::mat4 view = glm::mat4(glm::mat3(m_mVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(view));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTex.TexName());
	glDepthFunc(GL_LEQUAL);
	m_skybox.RenderGL();


	/*body*/
	m_program = m_body.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");


	M = translate(mat4(1.f), vec3(-2.5, -18, -2.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_bodyTex.TexName());

	m_body.RenderGL();

	/*left Joint*/

	m_program = m_leftJoint.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");
	glm::mat4 M1;
	if (camNum == 2) {
		M1 = translate(mat4(1.f), vec3(-4, 0., 0))
			* rotate(mat4(1.f), m_spinAngle, vec3(1., 0., 0.5));

	}
	else {
		M1 = translate(mat4(1.f), vec3(-4, 0., 0))
			* rotate(mat4(1.f), m_spinAngle, vec3(1., 0., 0.));
	}
	
	glm::mat4 VM1 = m_mVM * M1;
	glm::mat4 PVM1 = m_mPVM * M1;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM1));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM1));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_leftJ.TexName());

	m_leftJoint.RenderGL();

	/*right Joint*/

	m_program = m_rightJoint.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M2; 
	if (camNum == 2) {
		M2 = translate(mat4(1.f), vec3(4, 0., 0))
			* rotate(mat4(1.f), m_spinAngle, vec3(-1., 0., 0.5));

	}
	else {
		M2 = translate(mat4(1.f), vec3(4, 0., 0))
			* rotate(mat4(1.f), m_spinAngle, vec3(1., 0., 0.));
	}
	
	glm::mat4 VM2 = m_mVM * M2;
	glm::mat4 PVM2 = m_mPVM * M2;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM2));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM2));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_rightJ.TexName());

	m_rightJoint.RenderGL();

	/*leftshoulder*/
	m_program = m_leftshoulder.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "body"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	if (camNum == 2) {
		M = translate(mat4(1.f), vec3(-5.5, 0, 0.))* rotate(mat4(1.f), m_spinAngle, vec3(1., 0, 0.5));;

	}
	else {
		M = translate(mat4(1.f), vec3(-5.5, 0, 0.))* rotate(mat4(1.f), m_spinAngle, vec3(1., 0, 0));;
	}
	
	//M = translate(mat4(1.f), vec3(-10.5, 0, 0.))* rotate(mat4(1.f), 50.f+m_spinAngle, vec3(1, 1, 1.));;
	VM = m_mVM * M;
	PVM = m_mPVM * M;
	 

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_bodyTex.TexName());

	m_leftshoulder.RenderGL();

	/*rightshoulder*/
	m_program = m_rightshoulder.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "body"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	if (camNum == 2) {
		M = translate(mat4(1.f), vec3(5.5, 0, 0))* rotate(mat4(1.f), m_spinAngle, vec3(-1., 0, 0.5));;

	}
	else {
		M = translate(mat4(1.f), vec3(5.5, 0, 0.))* rotate(mat4(1.f), m_spinAngle, vec3(1., 0, 0));;
	}
	
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_bodyTex.TexName());

	m_rightshoulder.RenderGL();

	/*left eye*/

	m_program = m_lefteye.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "haha"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M3;
	if (camNum == 2) {
		M3 = translate(mat4(1.f), vec3(-1.5, 5.5, 3))
			* rotate(mat4(1.f), m_spinAngle, vec3(1., 0., 0));

	}
	else {
		M3 = translate(mat4(1.f), vec3(-1.5, 5.5, 3))
			* rotate(mat4(1.f), m_spinAngle, vec3(1., 0., 0.));
	}

	glm::mat4 VM3 = m_mVM * M3;
	glm::mat4 PVM3 = m_mPVM * M3;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM3));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM3));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_leftTex.TexName());

	m_lefteye.RenderGL();

	/*right eye*/

	m_program = m_righteye.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M4;
	if (camNum == 2) {
		M4 = translate(mat4(1.f), vec3(1.5, 5.5, 3))
			* rotate(mat4(1.f), m_spinAngle, vec3(1., 0., 0));

	}
	else {
		M4 = translate(mat4(1.f), vec3(1.5, 5.5, 3))
			* rotate(mat4(1.f), m_spinAngle, vec3(1., 0., 0.));
	}

	glm::mat4 VM4 = m_mVM * M4;
	glm::mat4 PVM4 = m_mPVM * M4;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM4));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM4));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_rightTex.TexName());

	m_righteye.RenderGL();

}

void CChildView::CleanGL()
{

}





void CChildView::OnProject1Rotation()
{
	camNum = 1; 
	// TODO: Add your command handler code here
	if (m_spinTimer == 0)
	{
		// Create the timer
		m_spinTimer = SetTimer(1, 30, NULL);
	}
	else
	{
		// Destroy the timer
		KillTimer(m_spinTimer);
		m_spinTimer = 0;
	}
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (camNum == 2) {
		m_spinAngle += 11;// 5 degrees every 30ms about 
	}
	else {
		m_spinAngle += 5; 
	}
	Invalidate();
	CShaderWnd::OnTimer(nIDEvent);
}


void CChildView::OnFreecamera()
{
	// TODO: Add your command handler code here
	camNum = 0;
	m_vEye = vec3(40, 20, 80);
	//m_vLookat = vec3(rotate(mat4(1.f), m_cameraAngle, vec3(0, 0, 0));
	m_vUp = vec3(0, 1, 0);

	glm::mat4 m_Rotation = translate(mat4(1.f), m_vLookat)
		* rotate(mat4(1.f), m_cameraAngle, vec3(0, 0, 25));
	
	//glm::mat4 m_mRotation = rotate(mat4(1.f), m_camera, axis);
	if (m_cameraTimer == 0)
	{
		// Create the timer
		m_cameraTimer = SetTimer(1, 30, NULL);
	}

	UpdateMatrix();
}


void CChildView::OnStaticcam()
{

	// TODO: Add your command handler code here
	camNum = 0;
	m_vEye = vec3(80, 20, 80);
	m_vLookat = vec3(0, 0, 0);
	m_vUp = vec3(0,1,0);
	if (m_cameraTimer == 0)
	{
		// Create the timer
		m_cameraTimer = SetTimer(1, 30, NULL);
	}

	UpdateMatrix();
}


void CChildView::OnProject1Rotation2()
{
	// TODO: Add your command handler code here
	camNum = 2; 
	// TODO: Add your command handler code here
	if (m_spinTimer == 0)
	{
		// Create the timer
		m_spinTimer = SetTimer(1, 30, NULL);
	}
	else
	{
		// Destroy the timer
		KillTimer(m_spinTimer);
		m_spinTimer = 0;
	}
}
