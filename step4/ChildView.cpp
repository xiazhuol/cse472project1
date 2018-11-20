
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
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CShaderWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CShaderWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::InitGL()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	/*Loading a Mesh*/
	/*m_catTex.LoadFile(L"models/cat-atlas.jpg");

	m_cat.LoadOBJ("models\\cat.obj");
	m_cat.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_cat.InitGL();*/
	/*Bump Map a Sphere*/
	m_sphereTex.LoadFile(L"textures/bumpmap.jpg");
	m_sphere.SetRadius(3);
	m_sphere.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_sphere.InitGL();
	/*Sky Box*/
	m_cubeTex.LoadFiles(L"textures/right.jpg", L"textures/left.jpg", L"textures/top.jpg",
		L"textures/bottom.jpg", L"textures/front.jpg", L"textures/back.jpg");

	m_skybox.CreateCube();
	m_skybox.m_program = LoadShaders("ShaderWnd/vertexSky.glsl", "ShaderWnd/fragmentSky.glsl");
	m_skybox.InitGL();
	//test that I can update 
	/*body*/

	m_bodyTex.LoadFiles(L"textures/plank01.bmp", L"textures/plank01.bmp", L"textures/plank01.bmp",
		L"textures/plank01.bmp", L"textures/plank01.bmp", L"textures/plank01.bmp");
	m_body.Createbody();
	m_body.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_body.InitGL();

	/*left joint*/
	m_leftJointTex.LoadFile(L"textures/marble10.bmp");
	m_leftJoint.SetRadius(1.5);
	m_leftJoint.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_leftJoint.InitGL();

	/*right joint*/
	m_rightJointTex.LoadFile(L"textures/marble10.bmp");
	m_rightJoint.SetRadius(1.5);
	m_rightJoint.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_rightJoint.InitGL();

	/*leftshoulder*/

	m_leftshoulderTex.LoadFiles(L"textures/plank01.bmp", L"textures/plank01.bmp", L"textures/plank01.bmp",
		L"textures/plank01.bmp", L"textures/plank01.bmp", L"textures/plank01.bmp");
	m_leftshoulder.CreateLeftshoulder();
	m_leftshoulder.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_leftshoulder.InitGL();

	/*rightshoulder*/

	m_rightshoulderTex.LoadFiles(L"textures/plank01.bmp", L"textures/plank01.bmp", L"textures/plank01.bmp",
		L"textures/plank01.bmp", L"textures/plank01.bmp", L"textures/plank01.bmp");
	m_rightshoulder.Createrightshoulder();
	m_rightshoulder.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_rightshoulder.InitGL();

}

void CChildView::RenderGL()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*Loading a Mesh*/
	/*m_program = m_cat.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "diffuse_mat"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(m_mPVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(m_mVM));

	glm::vec4 light_position(-5.f, 5.f, -5.f, 0.f);
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

	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_catTex.TexName());

	m_cat.RenderGL();*/
	/*Bump Map a Sphere*/
	m_program = m_sphere.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M = translate(mat4(1.f), vec3(0., 5., 0.))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.));
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
	/*Environment Map*/
	/*m_program = m_metallicSphere.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "env_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(10., 0., 0.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	m_metallicSphere.RenderGL();*/

	/*torus*/
	/*m_program = m_tori1.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(0., 0., 10.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_marble1.TexName());

	m_tori1.RenderGL();*/

	/*body*/
	m_program = m_body.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "body"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(-2.5, -18, -2.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_bodyTex.TexName());

	m_body.RenderGL();

	/*left Joint*/

	m_program = m_leftJoint.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M1 = translate(mat4(1.f), vec3(-4, 0., 0))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.));
	glm::mat4 VM1 = m_mVM * M1;
	glm::mat4 PVM1 = m_mPVM * M1;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM1));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM1));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_leftJointTex.TexName());

	m_leftJoint.RenderGL();

	/*right Joint*/

	m_program = m_rightJoint.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M2 = translate(mat4(1.f), vec3(4, 0., 0))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.));
	glm::mat4 VM2 = m_mVM * M2;
	glm::mat4 PVM2 = m_mPVM * M2;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM2));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM2));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_rightJointTex.TexName());

	m_rightJoint.RenderGL();

	/*leftshoulder*/
	m_program = m_leftshoulder.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "body"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(-15.5, -1, -2.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_leftshoulderTex.TexName());

	m_leftshoulder.RenderGL();

	/*rightshoulder*/
	m_program = m_rightshoulder.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "body"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(5.5, -1, -6));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_rightshoulderTex.TexName());

	m_rightshoulder.RenderGL();



	/*m_program = m_letter2.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(5, -10, 1.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_letterTex2.TexName());

	m_letter2.RenderGL();

	m_program = m_letter3.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(5, -10, 1.));
	VM = m_mVM * M;
	PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_letterTex3.TexName());

	m_letter3.RenderGL();*/

}

void CChildView::CleanGL()
{

}
