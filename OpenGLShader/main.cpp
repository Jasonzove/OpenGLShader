#include <windows.h>
#include <stdio.h>

#include "glew.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "shader.h"
#include "resource.h"
#include "GPUProgram.h"
#include "utils.h"
#include "model_obj.h"
#include "model_fbo.h"
#include "full_screen_quad.h"


LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hIcon = NULL;
	wndClass.hIconSm = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc=GLWindowProc;
	wndClass.lpszClassName = "OpenGL";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndClass);

	HWND hwnd = CreateWindowEx(NULL, "OpenGL", "RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormatID = ChoosePixelFormat(dc, &pfd);

	SetPixelFormat(dc,pixelFormatID,&pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	glewInit();

	ObjMoel obj;
	if (!obj.Load("./Res/model/Cube.obj"))
	{
		printf("load obj model failed!\n");
		return -1;
	}

	GPUProgram program;
	program.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_sample_vs));
	program.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_sample_fs));
	if (!program.Link())
	{
		printf("link program failed!\n");
		//return -1;
	}
	GLuint mainTexture = CreateTexture("./Res/image/test.bmp");
	GLuint secondaryTexture = CreateTexture("./Res/image/wood.bmp");
	
	glm::mat4 viewMat = glm::mat4();
	glm::mat4 modelMat = glm::translate<float>(0.0f, 0.0f, -2.0f)*glm::rotate<float>(45.0f, 0.0f, 1.0f, 0.0f);
	glm::mat4 projectMat = glm::perspective<float>(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);

	glClearColor(41.0f/255.0f,  71.0f/255.0f, 121.0f / 255.0f, 1.0f);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//一些开关
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//FBO
	FrameBufferObject fbo;
	fbo.AttachColorBuffer(NORMALCOLOR, GL_COLOR_ATTACHMENT0, GL_RGBA, 800, 600);
	fbo.AttachDepthBuffer(DEPTH, 800, 600);
	fbo.Finish();

	//full screen
	FullScreenQuad fsq;
	fsq.Init();
	GPUProgram fsqProgram;
	fsqProgram.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_vs));
	fsqProgram.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_fs));
	if (!fsqProgram.Link())
	{
		printf("link fsqProgram failed!\n");
		//return -1;
	}


	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//off screen fbo
		fbo.Bind();

		glClearColor(71.0f / 255.0f, 41.0f / 255.0f, 121.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		program.Bind();
		program.SetUniformfv("M", glm::value_ptr(modelMat), 16);
		program.SetUniformfv("V", glm::value_ptr(viewMat), 16);
		program.SetUniformfv("P", glm::value_ptr(projectMat), 16);
		program.SetTexture("U_MainTexture", mainTexture, GPUProgram::TEXTURE0, 1);
		program.SetTexture("U_SecondaryTexture", secondaryTexture, GPUProgram::TEXTURE1, 1);

		obj.Bind(program.GetLocation("pos", GPUProgram::ATTRIBUTE),
			program.GetLocation("texcoord", GPUProgram::ATTRIBUTE));
		obj.Draw();

		program.UnBind();
		fbo.UnBind();

		//full screen quads
		glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsqProgram.Bind();
		fsq.DrawWithTexture(fsqProgram.GetLocation("pos", GPUProgram::ATTRIBUTE), fsqProgram.GetLocation("U_MainTexture", GPUProgram::UNIFORM), fbo.GetBufferByType(NORMALCOLOR));
		fsqProgram.UnBind();

		SwapBuffers(dc);
	}
	return 0;
}