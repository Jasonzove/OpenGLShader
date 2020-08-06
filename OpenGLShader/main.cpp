#include <windows.h>
#include "glew.h"
#include <stdio.h>
#include <math.h>

#include "shader.h"
#include "resource.h"
#include "GPUProgram.h"
#include "utils.h"

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

	GPUProgram program;
	program.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_sample_vs));
	program.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_sample_fs));
	if (!program.Link())
	{
		printf("link program failed!\n");
	}
	GLuint mainTexture = CreateTexture("Res/image/test.bmp");

	float identity[] = {
		1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1.0f
	};
	float *projection = CreatePerspective(50.0f,800.0f/600.0f,0.1f,1000.0f);

	struct Vertex
	{
		float v[3];
		float rgba[4];
		float texcoord[4];
	};

	Vertex vertex[4];
	memset(vertex, 0, sizeof(Vertex) * 4);
	vertex[0].v[0] = 0.0f;
	vertex[0].v[1] = 0.0f;
	vertex[0].v[2] = -30.0f;
	vertex[0].rgba[0] = 1.0f;
	vertex[0].rgba[1] = 1.0f; 
	vertex[0].rgba[2] = 1.0f; 
	vertex[0].rgba[3] = 1.0f;
	vertex[0].texcoord[0] = 0.0f;
	vertex[0].texcoord[1] = 0.0f;

	vertex[1].v[0] = 10.0f;
	vertex[1].v[1] = 0.0f;
	vertex[1].v[2] = -30.0f;
	vertex[1].rgba[0] = 1.0f;
	vertex[1].rgba[1] = 1.0f;
	vertex[1].rgba[2] = 1.0f;
	vertex[1].rgba[3] = 1.0f;
	vertex[1].texcoord[0] = 1.0f;
	vertex[1].texcoord[1] = 0.0f;

	vertex[2].v[0] = 10.0f;
	vertex[2].v[1] = 10.0f;
	vertex[2].v[2] = -30.0f;
	vertex[2].rgba[0] = 1.0f;
	vertex[2].rgba[1] = 1.0f;
	vertex[2].rgba[2] = 1.0f;
	vertex[2].rgba[3] = 1.0f;
	vertex[2].texcoord[0] = 1.0f;
	vertex[2].texcoord[1] = 1.0f;

	vertex[3].v[0] = 0.0f;
	vertex[3].v[1] = 10.0f;
	vertex[3].v[2] = -30.0f;
	vertex[3].rgba[0] = 1.0f;
	vertex[3].rgba[1] = 1.0f;
	vertex[3].rgba[2] = 1.0f;
	vertex[3].rgba[3] = 1.0f;
	vertex[3].texcoord[0] = 0.0f;
	vertex[3].texcoord[1] = 1.0f;

	unsigned short indexes[] = {0,1,2,3};
	GLuint vbo,ebo;
	vbo = CreateGPUBufferObject(GL_ARRAY_BUFFER, sizeof(vertex), GL_STATIC_DRAW, vertex);
	ebo = CreateGPUBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), GL_STATIC_DRAW, indexes);

	glClearColor(41.0f/255.0f,  71.0f/255.0f, 121.0f / 255.0f, 1.0f);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

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
		glClear(GL_COLOR_BUFFER_BIT);

		program.Bind();
		program.SetUniformfv("M", identity, 16);
		program.SetUniformfv("V", identity, 16);
		program.SetUniformfv("P", projection, 16);
		program.SetTexture("U_MainTexture", 0, 1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		program.SetAttribPointer("pos", 3, sizeof(Vertex), (void*)0);
		program.SetAttribPointer("color", 4, sizeof(Vertex), (void*)(sizeof(float) * 3));
		program.SetAttribPointer("texcoord", 4, sizeof(Vertex), (void*)(sizeof(float) * 7));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		program.UnBind();
		SwapBuffers(dc);
	}
	return 0;
}