#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "glew.h"
#include "shader.h"
#include "resource.h"
#include "macros.h"
#include "gpu_program.h"
#include "utils.h"
#include "obj_model.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "texture_2d.h"

using namespace LH;

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

float* CreatePerspective(float fov, float aspect, float zNear, float zFar)
{
	float *matrix = new float[16];
	float half = fov / 2.0f;
	float randiansOfHalf = (half / 180.0f)*3.14f;
	float yscale = cos(randiansOfHalf) / sin(randiansOfHalf);
	float xscale = yscale / aspect;
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = xscale;
	matrix[5] = yscale;
	matrix[10] = (zNear + zFar) / (zNear - zFar);
	matrix[11] = -1.0f;
	matrix[14] = (2.0f*zNear*zFar) / (zNear - zFar);
	return matrix;
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
	wndClass.lpszClassName = L"OpenGL";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndClass);

	HWND hwnd = CreateWindowEx(NULL, L"OpenGL", L"RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);
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
	//init texture
	Texture2D mainTexture, secondTexture;
	mainTexture.Init("Res/image/test.bmp");
	secondTexture.Init("Res/image/wood.bmp");

	//init program
	GPUProgram gpuProgram;
	gpuProgram.AttachShader(GL_VERTEX_SHADER, IDR_SHADER_MULTI_TEXTURE_VS);
	gpuProgram.AttachShader(GL_FRAGMENT_SHADER, IDR_SHADER_MULTI_TEXTURE_FS);
	gpuProgram.Link();

	//获取属性位置
	gpuProgram.DetectAttribute("pos");
	gpuProgram.DetectAttribute("normal");
	gpuProgram.DetectAttribute("texcoord");
	gpuProgram.DetectUniform("M");
	gpuProgram.DetectUniform("V");
	gpuProgram.DetectUniform("P");
	gpuProgram.DetectUniform("U_MainTexture");
	gpuProgram.DetectUniform("U_SecondTexture");

	//init model
	ObjModel model;
	model.Init("Res/model/Cube.obj");

	float identity[] = {
		1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1.0f
	};
	float *projection = CreatePerspective(50.0f,800.0f/600.0f,0.1f,1000.0f);

	glm::mat4 modeMatrix = glm::translate(0.0f, 0.0f, -2.0f) * glm::rotate(30.0f, 0.0f, 0.0f, -1.0f) * glm::rotate(30.0f, 0.0f, -1.0f, 0.0f);

	glClearColor(41.0f/255.0f,  71.0f/255.0f, 121.0f / 255.0f, 1.0f);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(gpuProgram.mProgram);
		glUniformMatrix4fv(gpuProgram.GetLocation("M"), 1,GL_FALSE, glm::value_ptr(modeMatrix));
		glUniformMatrix4fv(gpuProgram.GetLocation("V"), 1, GL_FALSE, identity);
		glUniformMatrix4fv(gpuProgram.GetLocation("P"), 1, GL_FALSE, projection);
		
		//纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mainTexture.mTexture);
		glUniform1i(gpuProgram.GetLocation("U_MainTexture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, secondTexture.mTexture);
		glUniform1i(gpuProgram.GetLocation("U_SecondTexture"), 1);

		model.Bind(gpuProgram.GetLocation("pos"), gpuProgram.GetLocation("texcoord"), gpuProgram.GetLocation("normal"));
		model.Draw();

		glUseProgram(0);
		SwapBuffers(dc);
	}
	return 0;
}