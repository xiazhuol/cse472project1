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

	m_vEye = vec3(60, 40, 100);
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
	ON_COMMAND(ID_PROJECT1_STATICCAM, &CChildView::OnStaticcam)
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
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	/*Sun*/
	m_drawSun.LoadFile(L"Bitmaps/sunmap.bmp");
	m_sun.SetRadius(8);
	m_sun.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_sun.InitGL();

	/*earth*/
	m_drawEarth.LoadFile(L"Bitmaps/earthmap.bmp");
	m_earth.SetRadius(1.5);
	m_earth.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere2.glsl");
	m_earth.InitGL();


	/*Mercury*/
	m_drawMer.LoadFile(L"Bitmaps/mercurymap.bmp");
	m_mercury.SetRadius(0.5);
	m_mercury.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere3.glsl");
	m_mercury.InitGL();

	/*Venus*/
	m_drawVenus.LoadFile(L"Bitmaps/venusmap.bmp");
	m_venus.SetRadius(1.2);
	m_venus.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere3.glsl");
	m_venus.InitGL();

	//Mars
	m_drawMars.LoadFile(L"Bitmaps/marsmap.bmp");
	m_mars.SetRadius(0.8);
	m_mars.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere3.glsl");
	m_mars.InitGL();

	//Jupiter
	m_drawJupiter.LoadFile(L"Bitmaps/jupitermap.bmp");
	m_juiter.SetRadius(5);
	m_juiter.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere3.glsl");
	m_juiter.InitGL();

	//Saturn
	m_drawSa.LoadFile(L"Bitmaps/saturnmap.bmp");
	m_saturn.SetRadius(3.5);
	m_saturn.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere3.glsl");
	m_saturn.InitGL();

	//Uranus
	m_drawU.LoadFile(L"Bitmaps/uranusmap.bmp");
	m_uranus.SetRadius(2.8);
	m_uranus.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere4.glsl");
	m_uranus.InitGL();

	//Neptune
	m_drawNep.LoadFile(L"Bitmaps/neptunemap.bmp");
	m_neptune.SetRadius(3);
	m_neptune.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere5.glsl");
	m_neptune.InitGL();



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

	/*sun*/
	m_program = m_sun.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M = translate(mat4(1.f), vec3(-50., 5., 0.))* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	glm::mat4 VM = m_mVM * M;
	glm::mat4 PVM = m_mPVM * M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawSun.TexName());

	m_sun.RenderGL();
	
	/*earth*/
	m_program = m_earth.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M1 = translate(mat4(1.f), vec3(-10., 5., 0.))
		* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	glm::mat4 VM1 = m_mVM * M1;
	glm::mat4 PVM1 = m_mPVM * M1;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM1));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM1));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawEarth.TexName());

	m_earth.RenderGL();


	/*Mercury*/
	m_program = m_mercury.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M2 = translate(mat4(1.f), vec3(-30., 5., 0.))
		* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	glm::mat4 VM2 = m_mVM * M2;
	glm::mat4 PVM2 = m_mPVM * M2;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM2));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM2));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawMer.TexName());

	m_mercury.RenderGL();

	/*Venus*/
	m_program = m_venus.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M3 = translate(mat4(1.f), vec3(-20., 5., 0.))
		* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	glm::mat4 VM3 = m_mVM * M3;
	glm::mat4 PVM3 = m_mPVM * M3;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM3));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM3));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawVenus.TexName());

	m_venus.RenderGL();

	/*Mars*/
	m_program = m_mars.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M4 = translate(mat4(1.f), vec3(0., 5., 0.))
		* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	glm::mat4 VM4 = m_mVM * M4;
	glm::mat4 PVM4 = m_mPVM * M4;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM4));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM4));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawMars.TexName());

	m_mars.RenderGL();

	/*Jupiter*/
	m_program = m_juiter.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M5 = translate(mat4(1.f), vec3(10., 5., 0.))
		* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	glm::mat4 VM5 = m_mVM * M5;
	glm::mat4 PVM5 = m_mPVM * M5;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM5));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM5));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawJupiter.TexName());

	m_juiter.RenderGL();

	/*saturn*/
	m_program = m_saturn.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M6 = translate(mat4(1.f), vec3(30., 5., 0.))
		* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	glm::mat4 VM6 = m_mVM * M6;
	glm::mat4 PVM6 = m_mPVM * M6;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM6));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM6));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawSa.TexName());

	m_saturn.RenderGL();

	/*uranus*/
	m_program = m_uranus.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M7 = translate(mat4(1.f), vec3(50., 5., 0.))
		* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	glm::mat4 VM7 = m_mVM * M7;
	glm::mat4 PVM7 = m_mPVM * M7;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM7));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM7));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawU.TexName());

	m_uranus.RenderGL();

	/*neptune*/
	m_program = m_neptune.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");
	
	glm::mat4 M8 = translate(mat4(1.f), vec3(70., 5., 0.))
			* rotate(mat4(1.f), m_spinAngle, vec3(0., 1., 0.));
	
	glm::mat4 VM8 = m_mVM * M8;
	glm::mat4 PVM8 = m_mPVM * M8;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM8));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM8));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawNep.TexName());

	m_neptune.RenderGL();
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
		m_spinAngle += 5;// 5 degrees every 30ms about 
		Invalidate();
		CShaderWnd::OnTimer(nIDEvent);
	}
	else if (camNum == 0) {
		
		m_spinAngle += 1.5;
		CShaderWnd::m_spinAngle = m_spinAngle;
		Invalidate();
		CShaderWnd::OnTimer(nIDEvent);
		UpdateMatrix();

	}
	else {
		m_spinAngle += 5; 
		Invalidate();
		CShaderWnd::OnTimer(nIDEvent);
	}

}




void CChildView::OnStaticcam()
{

	// TODO: Add your command handler code here
	camNum = 0;

	if (CShaderWnd::getSpinTimer() == 0)
	{
		// Create the timer
		CShaderWnd::m_spinTimer = SetTimer(1, 30, NULL);
	}
	else
	{
		// Destroy the timer
		KillTimer(CShaderWnd::m_spinTimer);
		CShaderWnd::m_spinTimer=0;

	}

}

