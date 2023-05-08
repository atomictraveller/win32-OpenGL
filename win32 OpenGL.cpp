
#include <cmath>
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>

#pragma comment(lib, "opengl32.lib")


#include "framework.h"	//	default msvc win32 header
#include "ogl.h"	//	my opengl header
#include "win32 OpenGL.h"	//	header for this project


#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32OPENGL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow)) return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32OPENGL));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32OPENGL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32OPENGL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) return FALSE;


//	begin opengl
	hOGLWnd = hWnd;
	HDC hDC = GetDC(hOGLWnd);

	DWORD pixelFormatFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_COMPOSITION | PFD_GENERIC_ACCELERATED | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1, pixelFormatFlags, PFD_TYPE_RGBA, 32,	//Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		24,                       //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);

	hrc0 = wglCreateContext(hDC);
	hrc1 = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hrc1);

	HMODULE hOpenGL = LoadLibrary(L"opengl32.dll");	//	begin loading "modern" opengl API
	if (hOpenGL == NULL) return 0;
	//glCreateShader = (PFNGLCREATESHADERPROC)GetProcAddress(hOpenGL, "glCreateShader");
	glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(wglGetProcAddress("glCreateShader"));
	glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(wglGetProcAddress("glShaderSource"));
	glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(wglGetProcAddress("glCompileShader"));
	glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>(wglGetProcAddress("glGetShaderiv"));
	glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(wglGetProcAddress("glGetShaderInfoLog"));
	glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(wglGetProcAddress("glCreateProgram"));
	glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(wglGetProcAddress("glAttachShader"));
	glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(wglGetProcAddress("glLinkProgram"));
	glValidateProgram = reinterpret_cast<PFNGLVALIDATEPROGRAMPROC>(wglGetProcAddress("glValidateProgram"));
	glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(wglGetProcAddress("glGetProgramiv"));
	glGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(wglGetProcAddress("glGetProgramInfoLog"));////was missing
	glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(wglGetProcAddress("glGenBuffers"));
	glGenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(wglGetProcAddress("glGenVertexArrays"));
	glGetAttribLocation = reinterpret_cast<PFNGLGETATTRIBLOCATIONPROC>(wglGetProcAddress("glGetAttribLocation"));
	glBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(wglGetProcAddress("glBindVertexArray"));
	glEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(wglGetProcAddress("glEnableVertexAttribArray"));
	glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(wglGetProcAddress("glVertexAttribPointer"));
	glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(wglGetProcAddress("glBindBuffer"));
	glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(wglGetProcAddress("glBufferData"));
	glGetVertexAttribPointerv = reinterpret_cast<PFNGLGETVERTEXATTRIBPOINTERVPROC>(wglGetProcAddress("glGetVertexAttribPointerv"));
	glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(wglGetProcAddress("glUseProgram"));
	glDeleteVertexArrays = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(wglGetProcAddress("glDeleteVertexArrays"));
	glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(wglGetProcAddress("glDeleteBuffers"));
	glDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(wglGetProcAddress("glDeleteProgram"));
	glDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>(wglGetProcAddress("glDeleteShader"));

	glUniform1fv = reinterpret_cast<PFNGLUNIFORM1FVPROC>(wglGetProcAddress("glUniform1fv"));
	glUniform3f = reinterpret_cast<PFNGLUNIFORM3FPROC>(wglGetProcAddress("glUniform3f"));
	glUniform4f = reinterpret_cast<PFNGLUNIFORM4FPROC>(wglGetProcAddress("glUniform4f"));
	glUniform3fv = reinterpret_cast<PFNGLUNIFORM3FVPROC>(wglGetProcAddress("glUniform3fv"));
	glUniform4fv = reinterpret_cast<PFNGLUNIFORM4FVPROC>(wglGetProcAddress("glUniform4fv"));
	glUniformMatrix3fv = reinterpret_cast<PFNGLUNIFORMMATRIX3FVPROC>(wglGetProcAddress("glUniformMatrix3fv"));
	glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(wglGetProcAddress("glUniformMatrix4fv"));
	glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(wglGetProcAddress("glGetUniformLocation"));

	glCreateVertexArrays = reinterpret_cast<PFNGLCREATEVERTEXARRAYSPROC>(wglGetProcAddress("glCreateVertexArrays"));

	FreeLibrary(hOpenGL);
	if (glCreateShader == NULL) return 0;
	if (glShaderSource == NULL) return 0;
	if (glCompileShader == NULL) return 0;
	if (glGetShaderiv == NULL) return 0;
	if (glGetShaderInfoLog == NULL) return 0;
	if (glCreateProgram == NULL) return 0;
	if (glAttachShader == NULL) return 0;
	if (glLinkProgram == NULL) return 0;
	if (glValidateProgram == NULL) return 0;
	if (glGetProgramiv == NULL) return 0;
	if (glGetProgramInfoLog == NULL) return 0;	//	
	if (glGenBuffers == NULL) return 0;
	if (glGenVertexArrays == NULL) return 0;
	if (glGetAttribLocation == NULL) return 0;
	if (glBindVertexArray == NULL) return 0;
	if (glEnableVertexAttribArray == NULL) return 0;
	if (glVertexAttribPointer == NULL) return 0;
	if (glBindBuffer == NULL) return 0;
	if (glBufferData == NULL) return 0;
	if (glGetVertexAttribPointerv == NULL) return 0;
	if (glUseProgram == NULL) return 0;
	if (glDeleteVertexArrays == NULL) return 0;
	if (glDeleteBuffers == NULL) return 0;
	if (glDeleteProgram == NULL) return 0;
	if (glDeleteShader == NULL) return 0;

	if (glUniform1fv == NULL) return 0;
	if (glUniform3f == NULL) return 0;
	if (glUniform4f == NULL) return 0;
	if (glUniform3fv == NULL) return 0;
	if (glUniform4fv == NULL) return 0;
	if (glUniformMatrix3fv == NULL) return 0;
	if (glUniformMatrix4fv == NULL) return 0;
	if (glGetUniformLocation == NULL) return 0;

	if (glCreateVertexArrays == NULL) return 0;

	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 pos;
		layout (location = 1) in vec3 color;
		layout (location = 2) in vec3 normal;
		out vec4 vertexColor;
		out vec3 texCoord;
		out float norm;
		uniform mat4 modelMatrix; 
		uniform mat4 viewMatrix;
		uniform mat4 projectionMatrix;
		void main() {
			gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos.x, pos.y, pos.z, 1.0);
			vertexColor = vec4(color, 1.0);
			texCoord = vec3(pos.x + 128.0, pos.y + 128.0, pos.z);
			norm = (normal.x * .3535 + normal.y * .7071 + normal.z * .6123 - 0.7) * 0.7;
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		in vec4 vertexColor;
		in vec3 texCoord;
		in float norm;
		uniform vec4 ourColor;
		uniform float rv[49];

		void main() {
			uint ix, iy, iz;
			float scale = 0.027;//.003
			float gain = 0.7;
			int octaves = 3; 
			float hf = 0.0;
			for (int i = 0; i < octaves; i++) {
				float hfx = texCoord.x * scale;
				float hfy = texCoord.y * scale * 5.0;
				float hfz = texCoord.z * scale;
				hfx = fract(hfx) * 25.0;	hfy = fract(hfy) * 25.0;	hfz = fract(hfz) * 25.0;
				ix = uint(hfx);	iy = uint(hfy);	iz = uint(hfz);
				float dx = hfx - ix;	float dy = hfy - iy;	float dz = hfz - iz;
				hfx = rv[ix];	hfy = rv[iy];	hfz = rv[iz];
				ix++;	iy++;	iz++;
				float vt0 = fract(hfx + hfy + hfz);	//	vertices of cube for interpolation
				float vt1 = fract(rv[ix] + hfy + hfz);
				float vt2 = fract(hfx + hfy + rv[iz]);
				float vt3 = fract(rv[ix] + hfy + rv[iz]);
				float vt4 = fract(hfx + rv[iy] + hfz);
				float vt5 = fract(rv[ix] + rv[iy] + hfz);
				float vt6 = fract(hfx + rv[iy] + rv[iz]);
				float vt7 = fract(rv[ix] + rv[iy] + rv[iz]);
				dx *= dx * (3.0 - dx - dx);
				dy *= dy * (3.0 - dy - dy);
				dz *= dz * (3.0 - dz - dz);
				float vi0 = vt0 + dx * (vt1 - vt0);
				float vi1 = vt2 + dx * (vt3 - vt2);
				float vi2 = vt4 + dx * (vt5 - vt4);
				float vi3 = vt6 + dx * (vt7 - vt6);
				vi0 += dz * (vi1 - vi0);
				vi2 += dz * (vi3 - vi2);
				vi0 += dy * (vi2 - vi0);
				//vi0 = abs(1.f - vi0 - vi0);
				hf += vi0 * gain;
				scale *= 1.8;
				gain *= 0.74;//.6
			}
			hf *= 0.1;
			hf += norm;
			FragColor = vec4(0.25 + hf, 0.12 + hf, hf * 1.2, 1.0);
		} 
	)";


	int  success;
	char infoLog[512];
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);	//	fault checking luxury version
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);	//	do something with infolog
	}
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);	//	do something with infolog
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);	//	do something with infolog
	}
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);	//	set as current vao


	makeheightfield();

	float rvals[] = {
	0.28f, 0.76f, 0.12f, 0.48f, 0.84f, 0.2f, 0.92f, 0.64f, 0.04f, 0.32f,
	0.8f, 0.16f, 0.52f, 0.24f, 0.72f, 0.4f, 0.56f, 0.0f, 0.36f, 0.96f,
	0.08f, 0.68f, 0.88f, 0.44f, 0.6f,
	0.28f, 0.76f, 0.12f, 0.48f, 0.84f, 0.2f, 0.92f, 0.64f, 0.04f, 0.32f,
	0.8f, 0.16f, 0.52f, 0.24f, 0.72f, 0.4f, 0.56f, 0.0f, 0.36f, 0.96f,
	0.08f, 0.68f, 0.88f, 0.44f
	};

	int rvalsLocation = glGetUniformLocation(shaderProgram, "rv");
	glUniform1fv(rvalsLocation, 49, &rvals[0]);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	//	this has to be after vao binding
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo);	//	element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);


	//index of uniform/attribute, vecn,,[0,1] normalise flag! :),vecn or 0 to let gl decide, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);	// pointer to the vertex data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));	//	offset for color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));	//	offset for normal
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glDisable(GL_CULL_FACE);

	ShowWindow(hOGLWnd, SW_SHOWDEFAULT);	//ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:	{
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }	break;
	case WM_KEYDOWN: {
		if ((HIWORD(lParam) & KF_REPEAT) < 1) {
			switch (LOWORD(wParam)) {
			case 'A':		xrv += .005f;	if (xrv > 1.f) xrv = 1.f;	break;
			case 'Z':		xrv -= .005f;	if (xrv < -1.f) xrv = -1.f;	break;
			case VK_LEFT:	yrv += .005f;	if (yrv > 1.f) yrv = 1.f;	break;
			case VK_RIGHT:	yrv -= .005f;	if (yrv < -1.f) yrv = -1.f;	break;
			case VK_UP:		vel += .0001f;	if (vel > 1.f) vel = 1.f;	break;
			case VK_DOWN:	vel -= .0001f;	if (vel < -1.f) vel = -1.f;	break;
			case VK_ESCAPE:	PostMessage(hWnd, WM_CLOSE, 0, 0);	break;
			case VK_SPACE:  break;
			}
		}
	}   break;
	case WM_LBUTTONDOWN: {
		POINT mouloc;	GetCursorPos(&mouloc);	ScreenToClient(hWnd, &mouloc);
	}   break;
	case WM_CREATE: {
		RECT clientRect;	GetClientRect(hOGLWnd, &clientRect);
		dispx = clientRect.right - clientRect.left;	dispy = clientRect.bottom - clientRect.top;
		glViewport(0, 0, dispx, dispy);
		
		SetTimer(hWnd, 1, 20, NULL);
	}	break;
    case WM_PAINT:	{
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
		display();
        EndPaint(hWnd, &ps);
    }	break;
	case WM_TIMER: {
		InvalidateRgn(hWnd, 0, 0);	UpdateWindow(hWnd);
	}   break;
	case WM_SIZE: {
		RECT clientRect;	GetClientRect(hOGLWnd, &clientRect);
		dispx = clientRect.right - clientRect.left;	dispy = clientRect.bottom - clientRect.top;
		glViewport(0, 0, dispx, dispy);
	}	break;
    case WM_DESTROY:
		glUseProgram(0);
		glDeleteBuffers(1, &vao);
		glDeleteBuffers(1, &vbo);
		if (HDC currentDC = wglGetCurrentDC()) wglMakeCurrent(currentDC, NULL);
		DestroyWindow(hOGLWnd);
		wglDeleteContext(hrc0);
		wglDeleteContext(hrc1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
