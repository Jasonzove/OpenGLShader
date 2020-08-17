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

	RECT rect;
	rect.left = 0; rect.right = 800;
	rect.top = 0; rect.bottom = 600;
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowEx(NULL, "OpenGL", "RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
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
	::GetClientRect(hwnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	glewInit();

	ObjMoel obj;
	if (!obj.Load("./Res/model/Quad.obj"))
	{
		printf("load obj model failed!\n");
		return -1;
	}

	GPUProgram program;
	program.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_light_vs));
	program.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_light_fs));
	if (!program.Link())
	{
		printf("link program failed!\n");
		//return -1;
	}
	
	glm::mat4 viewMat = glm::mat4();
	glm::mat4 modelMat = glm::translate<float>(0.0f, -0.5f, -4.0f)*glm::rotate(90.0f, -1.0f,0.0f,0.0f)*glm::scale(2.0f,2.0f,2.0f);
	glm::mat4 normalMat = glm::inverseTranspose(modelMat);
	glm::mat4 projectMat = glm::perspective<float>(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

	glClearColor(41.0f/255.0f,  71.0f/255.0f, 121.0f / 255.0f, 1.0f);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//一些开关
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//光照
	float lightPos[] = { 0.0f, 1.5f, -4.0f, 1.0 };
	float spotDir[] = { 0.0, -1.0, 0.0, 64.0 }; //聚光灯方向，第四个用于软化边缘
	float cutOff = 15.0f; //聚光灯半角
	float ambientLight[] = { 0.4f,0.4f,0.4f,1.0f };
	float ambientMaterial[] = { 0.1f,0.1f,0.1f,1.0f };
	float diffuseLight[] = { 1.0f,1.0f,1.0f,1.0f };
	float diffuseMaterial[] = { 0.4f,0.4f,0.4f,1.0f };
	float eyePos[] = { 0.0f, 0.0f, 0.0f };
	float specularLight[] = { 1.0f,1.0f,1.0f,1.0f };
	float specularMaterial[] = { 1.0f,1.0f,1.0f,1.0f };

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

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		program.Bind();
		program.SetUniformfv("M", glm::value_ptr(modelMat), 16);
		program.SetUniformfv("V", glm::value_ptr(viewMat), 16);
		program.SetUniformfv("P", glm::value_ptr(projectMat), 16);
		program.SetUniformfv("NM", glm::value_ptr(normalMat), 16);
		program.SetUniformfv("U_LightPos", lightPos, 4);
		program.SetUniformfv("U_AmbientLigth", ambientLight, 4);
		program.SetUniformfv("U_AmbientMaterial", ambientMaterial, 4);
		program.SetUniformfv("U_DiffuseLight", diffuseLight, 4);
		program.SetUniformfv("U_DiffuseMaterial", diffuseMaterial, 4);
		program.SetUniformfv("U_EyePos", eyePos, 3);
		program.SetUniformfv("U_SpecularLight", specularLight, 4);
		program.SetUniformfv("U_SpecularMaterial", specularMaterial, 4);
		program.SetUniformfv("U_SpotDirection", spotDir, 4);
		program.SetUniformf("U_CutOff", cutOff, 1);

		obj.Bind(program.GetLocation("pos", GPUProgram::ATTRIBUTE),BINDNORMAL, 
			program.GetLocation("texcoord", GPUProgram::ATTRIBUTE),
			program.GetLocation("normal", GPUProgram::ATTRIBUTE));
		obj.Draw();
		program.UnBind();

		SwapBuffers(dc);
	}
	return 0;
}