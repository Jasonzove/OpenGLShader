#include <stdio.h>
#include <windows.h>
#include "glew.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"


struct Vertex
{
	float pos[3];
	float color[4];
};

/* �����û���������;LRESULT(��������ֵ����); CALLBACK(���÷�ʽ)
   hwnd(���ھ�������ڱ���û���������һ������); msg(��ϢID������1��ʾ�û���ק�˴���);
   wParam(��Ϣ���������������û���ק���ڣ������ϵ�ʲô�ط�ȥ��); lParam(��Ϣ��������)
*/
LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE: //Ŀǰֻ�����˳����ڵ���Ϣ
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

char* LoadShaderContent(const char* path)
{
	FILE* pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nlen = ftell(pFile);
		char* buffer = new char[nlen + 1];
		rewind(pFile);
		fread(buffer, nlen, 1, pFile);
		buffer[nlen] = '\0';
		fclose(pFile);
		return buffer;
	}
	fclose(pFile);
	return nullptr;
}

GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath)
{
	GLuint vsShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vsCode = LoadShaderContent(vsShaderPath);
	const char* fsCode = LoadShaderContent(fsShaderPath);
	//����GPU
	glShaderSource(vsShader, 1, &vsCode, nullptr);
	glShaderSource(fsShader, 1, &fsCode, nullptr);
	//����
	glCompileShader(vsShader);
	glCompileShader(fsShader);
	//����program��attach
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	//Link
	glLinkProgram(program);
	//detach and delete
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);

	return program;
}


/* INT����������ֵ���ͣ�; WINAPI���������η�,ǿ�����÷�ʽ��;
   ����ϵͳ����ʱ����Ĳ�����hInstance����ǰӦ�ó����ʵ��),hPrevInstance����һ�θ�Ӧ�ó���������ʵ������
   lpCmdLine������������������Ĳ�����,nShowCmd��������ʾ��ز�����*/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//�������ڰ����Ĳ���ע�ᣬ��������ʾ�������û�����

	//ע�ᴰ��
	WNDCLASSEX windClass; //�ô˽ṹ����߲���ϵͳ������Ҫһ��ʲô���Ĵ���
	windClass.cbClsExtra = NULL; //�������͵Ķ���ռ䣨�ڴ����ǲ���Ҫ����Ŀռ䣩
	windClass.cbSize = sizeof(WNDCLASSEX); //windClass�������ʵ��ռ�õ��ڴ�
	windClass.cbWndExtra = 0; //���ڵĶ����ڴ�
	windClass.hbrBackground = NULL; //ʹ��OpenGL��������ɫ��������Ҫ��GDI����ΪNULL
	windClass.hCursor = LoadCursor(NULL, IDC_ARROW); //���ù�꣨�ڴ�����Ϊ��ͷ��꣩
	windClass.hIcon = NULL; //Ӧ�ó������ļ�������ʾ��ͼ�꣨�ڴ˲����ã�
	windClass.hIconSm = NULL; //Ӧ�ó��������������Ͻ���ʾ��ͼ��
	windClass.hInstance = hInstance; //����ϵͳ����Ĳ���
	windClass.lpfnWndProc = GLWindowProc; //�˺��������û���������������˭���������ǵĴ���
	windClass.lpszClassName = L"GLWindow"; //��������
	windClass.lpszMenuName = NULL; //�˵������֣�û�в˵������գ�
	windClass.style = CS_VREDRAW | CS_HREDRAW; //����ˢ��ʱ���õ��ػ淽ʽ���ڴ˲���ˮƽ���ߴ�ֱ�ػ�ķ�ʽ��
	ATOM atom = RegisterClassEx(&windClass);
	if (!atom)
	{
		MessageBox(NULL, L"Notice", L"Error", MB_OK);
		return 0;
	}

	//�ӿڴ�С
	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.top = 0;
	rect.bottom = 600;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL); // ���ڷ��Ͳ˵����
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	//��������
	//�������ζ�ӦΪ�����ڴ����ķ�񣬴������ƣ�ע��ʱ�����ƣ�, �������ϽǱ����������ڵķ�񣬴�����ʼλ�ã����ڵĿ�͸�, �����ڵľ�����˵������ ����ʵ��
	HWND hwnd = CreateWindowEx(NULL, L"GLWindow", L"Opengl Window", WS_OVERLAPPEDWINDOW, 100, 100, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	//����opengl��Ⱦ����
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32; //һ������ռ��32bit��RGBA�ֱ�ռ��8bit
	pfd.cDepthBits = 24; //��Ȼ�����ÿ������ռ24it��������
	pfd.cStencilBits = 8; //�ɰ建����ÿ������ռ8bit
	pfd.iPixelType = PFD_TYPE_RGBA; //������������
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; //����һ��������flag
	int pixelFormat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormat, &pfd);
	HGLRC rc = wglCreateContext(dc); //��Ⱦ����
	wglMakeCurrent(dc, rc); //ʹ��Ⱦ������Ч

	//glewInit�������wglMakeCurrent֮��
	glewInit();
	GLuint proram = CreateGPUProgram("sample.vs", "sample.fs"); //�������glewInit֮��
	GLint posLoaction, colorLocation, MLocation, VLocation, PLocation;
	posLoaction = glGetAttribLocation(proram, "pos");
	colorLocation = glGetAttribLocation(proram, "color");
	MLocation = glGetUniformLocation(proram, "M");
	VLocation = glGetUniformLocation(proram, "V");
	PLocation = glGetUniformLocation(proram, "P");

	Vertex vertexs[3];
	vertexs[0].pos[0] = 0.0f;
	vertexs[0].pos[1] = 0.0f;
	vertexs[0].pos[2] = -5.0f;
	vertexs[0].color[0] = 1.0f;
	vertexs[0].color[1] = 1.0f;
	vertexs[0].color[2] = 1.0f;
	vertexs[0].color[3] = 1.0f;

	vertexs[1].pos[0] = 2.0f;
	vertexs[1].pos[1] = 0.0f;
	vertexs[1].pos[2] = -5.0f;
	vertexs[1].color[0] = 1.0f;
	vertexs[1].color[1] = 1.0f;
	vertexs[1].color[2] = 1.0f;
	vertexs[1].color[3] = 1.0f;

	vertexs[2].pos[0] = 0.0f;
	vertexs[2].pos[1] = 2.0f;
	vertexs[2].pos[2] = -5.0f;
	vertexs[2].color[0] = 1.0f;
	vertexs[2].color[1] = 1.0f;
	vertexs[2].color[2] = 1.0f;
	vertexs[2].color[3] = 1.0f;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, vertexs, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int indexes[] = { 0,1,2 };
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int), indexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//����׼��
	float identity[] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);

	//��ѭ�������ִ�����ʾ
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(proram);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLoaction);
		glVertexAttribPointer(posLoaction, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(colorLocation);
		glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);
		SwapBuffers(dc);
	}

	return 0;
}