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
	if (!obj.Load("./Res/model/Sphere.obj"))
	{
		printf("load obj model failed!\n");
		return -1;
	}

	GPUProgram program;
	program.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_x_ray_vs));
	program.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_x_ray_fs));
	if (!program.Link())
	{
		printf("link program failed!\n");
		//return -1;
	}

	GPUProgram originalFsProgram;
	originalFsProgram.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_vs));
	originalFsProgram.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_fs));
	if (!originalFsProgram.Link())
	{
		printf("link program failed!\n");
		//return -1;
	}

	GPUProgram dilationFsProgram;
	dilationFsProgram.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_vs));
	dilationFsProgram.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_fs));
	if (!dilationFsProgram.Link())
	{
		printf("link program failed!\n");
		//return -1;
	}

	GPUProgram erosionFsProgram;
	erosionFsProgram.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_vs));
	erosionFsProgram.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_fs));
	if (!erosionFsProgram.Link())
	{
		printf("link program failed!\n");
		//return -1;
	}

	GPUProgram gaussionFsProgram;
	gaussionFsProgram.AttachShader(GPUProgram::VERTEX_SHADER, Shader::GetShaderCode(IDR_SHADER_full_screen_quad_vs));
	gaussionFsProgram.AttachShader(GPUProgram::FRAGEMENT_SHADER, Shader::GetShaderCode(IDR_SHADER_gaussion_fs));
	if (!gaussionFsProgram.Link())
	{
		printf("link program failed!\n");
		//return -1;
	}

	FrameBufferObject fbo;
	fbo.AttachColorBuffer(NORMALCOLOR, GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fbo.AttachDepthBuffer(DEPTH, width, height);
	fbo.Finish();

	FrameBufferObject fboBlur1;
	fboBlur1.AttachColorBuffer(NORMALCOLOR, GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fboBlur1.AttachDepthBuffer(DEPTH, width, height);
	fboBlur1.Finish();

	FrameBufferObject fboBlur2;
	fboBlur2.AttachColorBuffer(NORMALCOLOR, GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
	fboBlur2.AttachDepthBuffer(DEPTH, width, height);
	fboBlur2.Finish();

	FullScreenQuad fs;
	fs.Init();

	GLuint textureId = CreateTexture("./Res/image/wood.bmp");

	glm::mat4 viewMat = glm::lookAt(glm::vec3(-0.5f, 2.5f, -3.0f), glm::vec3(0.0f, 0.0f, -6.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 modelMat = glm::translate<float>(0.0f, 0.0f, -6.0f) * glm::rotate(-30.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 normalMat = glm::inverseTranspose(modelMat);
	glm::mat4 projectMat = glm::perspective<float>(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

	glClearColor(41.0f/255.0f,  71.0f/255.0f, 121.0f / 255.0f, 1.0f);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//一些开关
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//光照
	float eyePos[] = { -0.5f, 2.5f, -3.0f };

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

		fbo.Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		program.Bind();
		program.SetUniformfv("M", glm::value_ptr(modelMat), 16);
		program.SetUniformfv("V", glm::value_ptr(viewMat), 16);
		program.SetUniformfv("P", glm::value_ptr(projectMat), 16);
		program.SetUniformfv("NM", glm::value_ptr(normalMat), 16);
		program.SetUniformfv("U_EyePos", eyePos, 3);

		obj.Bind(program.GetLocation("pos", GPUProgram::ATTRIBUTE),BINDNORMAL, 
			program.GetLocation("texcoord", GPUProgram::ATTRIBUTE),
			program.GetLocation("normal", GPUProgram::ATTRIBUTE));
		obj.Draw();
		program.UnBind();
		glDisable(GL_BLEND);
		fbo.UnBind();

		fboBlur1.Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gaussionFsProgram.Bind();
		fs.DrawWithTexture(FULLSCREEN, gaussionFsProgram.GetLocation("pos", GPUProgram::ATTRIBUTE),
			gaussionFsProgram.GetLocation("texcoord", GPUProgram::ATTRIBUTE),
			gaussionFsProgram.GetLocation("U_MainTexture", GPUProgram::UNIFORM), fbo.GetBufferByType(NORMALCOLOR));
		gaussionFsProgram.UnBind();
		fboBlur1.UnBind();

		fboBlur2.Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gaussionFsProgram.Bind();
		fs.DrawWithTexture(FULLSCREEN, erosionFsProgram.GetLocation("pos", GPUProgram::ATTRIBUTE),
			erosionFsProgram.GetLocation("texcoord", GPUProgram::ATTRIBUTE),
			erosionFsProgram.GetLocation("U_MainTexture", GPUProgram::UNIFORM), fboBlur1.GetBufferByType(NORMALCOLOR));
		gaussionFsProgram.UnBind();
		fboBlur2.UnBind();

		originalFsProgram.Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fs.DrawWithTexture(LTFULLSCREEN, originalFsProgram.GetLocation("pos", GPUProgram::ATTRIBUTE),
			originalFsProgram.GetLocation("texcoord", GPUProgram::ATTRIBUTE),
			originalFsProgram.GetLocation("U_MainTexture", GPUProgram::UNIFORM), fbo.GetBufferByType(NORMALCOLOR));

		dilationFsProgram.Bind();
		fs.DrawWithTexture(RTFULLSCREEN, gaussionFsProgram.GetLocation("pos", GPUProgram::ATTRIBUTE),
			gaussionFsProgram.GetLocation("texcoord", GPUProgram::ATTRIBUTE),
			gaussionFsProgram.GetLocation("U_MainTexture", GPUProgram::UNIFORM), fboBlur1.GetBufferByType(NORMALCOLOR));

		erosionFsProgram.Bind();
		fs.DrawWithTexture(LBFULLSCREEN, erosionFsProgram.GetLocation("pos", GPUProgram::ATTRIBUTE),
			erosionFsProgram.GetLocation("texcoord", GPUProgram::ATTRIBUTE),
			erosionFsProgram.GetLocation("U_MainTexture", GPUProgram::UNIFORM), fboBlur2.GetBufferByType(NORMALCOLOR));
		erosionFsProgram.UnBind();

		gaussionFsProgram.Bind();
		fs.DrawWithTexture(RBFULLSCREEN, gaussionFsProgram.GetLocation("pos", GPUProgram::ATTRIBUTE),
			gaussionFsProgram.GetLocation("texcoord", GPUProgram::ATTRIBUTE),
			gaussionFsProgram.GetLocation("U_MainTexture", GPUProgram::UNIFORM), fboBlur2.GetBufferByType(NORMALCOLOR));
		
		originalFsProgram.UnBind();

		SwapBuffers(dc);
	}
	return 0;
}