
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

	glUniform3f = reinterpret_cast<PFNGLUNIFORM3FPROC>(wglGetProcAddress("glUniform3f"));
	glUniform4f = reinterpret_cast<PFNGLUNIFORM4FPROC>(wglGetProcAddress("glUniform4f"));
	glUniform3fv = reinterpret_cast<PFNGLUNIFORM3FVPROC>(wglGetProcAddress("glUniform3fv"));
	glUniform4fv = reinterpret_cast<PFNGLUNIFORM4FVPROC>(wglGetProcAddress("glUniform4fv"));
	glUniformMatrix3fv = reinterpret_cast<PFNGLUNIFORMMATRIX3FVPROC>(wglGetProcAddress("glUniformMatrix3fv"));
	glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(wglGetProcAddress("glUniformMatrix4fv"));
	glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(wglGetProcAddress("glGetUniformLocation"));

	glCreateVertexArrays = reinterpret_cast<PFNGLCREATEVERTEXARRAYSPROC>(wglGetProcAddress("glCreateVertexArrays"));
	//glDrawElements = reinterpret_cast<PFNGLDRAWELEMENTSPROC>(wglGetProcAddress("glDrawElements"));

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

	if (glUniform3f == NULL) return 0;
	if (glUniform4f == NULL) return 0;
	if (glUniform3fv == NULL) return 0;
	if (glUniform4fv == NULL) return 0;
	if (glUniformMatrix3fv == NULL) return 0;
	if (glUniformMatrix4fv == NULL) return 0;
	if (glGetUniformLocation == NULL) return 0;

	if (glCreateVertexArrays == NULL) return 0;
	//if (glDrawElements == NULL) return 0;

	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 pos;
		layout (location = 1) in vec3 color;
		out vec4 vertexColor;
		out vec3 texCoord;
		uniform mat4 modelMatrix; 
		uniform mat4 viewMatrix;
		uniform mat4 projectionMatrix;
		void main() {
			gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos.x, pos.y, pos.z, 1.0);
			vertexColor = vec4(color, 1.0);
			texCoord = vec3(pos.x, pos.y, pos.z);
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		in vec4 vertexColor;
		in vec3 texCoord;
		uniform vec4 ourColor;
		float rv[49] = float[](
			0.28, 0.76, 0.12, 0.48, 0.84, 0.2, 0.92, 0.64, 0.04, 0.32,
			0.8, 0.16, 0.52, 0.24, 0.72, 0.4, 0.56, 0.0, 0.36, 0.96,
			0.08, 0.68, 0.88, 0.44, 0.6,
			0.28, 0.76, 0.12, 0.48, 0.84, 0.2, 0.92, 0.64, 0.04, 0.32,
			0.8, 0.16, 0.52, 0.24, 0.72, 0.4, 0.56, 0.0, 0.36, 0.96,
			0.08, 0.68, 0.88, 0.44
		);
		void main() {
			uint ix, iy, iz;
			float scale = 0.003;
			float gain = 0.7;
			int octaves = 4; 
			float hf = 0.0;
			for (int i = 0; i < octaves; i++) {
				float hfx = texCoord.x * scale;
				float hfy = texCoord.y * scale * 7.0;
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
				hf += vi0 * gain;
				scale *= 2.0;
				gain *= 0.6;
			}	//	1.4, 0.6 ... 2.2, 6.7

			hf *= 0.5;
			
/*
			float hfr;
			hf -= 0.64;
			if (hf > 0.0) {
				hf = abs(hf) * 25.0;
				if (hf > 0.2) hf = 0.2 + (hf - 0.2) * 0.025;
				hfr = hf;
			}
			else {
				hf = abs(hf) * 18.0;
				if (hf > 0.2) hf = 0.2 + (hf - 0.2) * 0.025;
				hfr = hf - .01;
				hf += 0.02;
			}
			//hf = min(0.5, hf * 12.0);
*/			

			FragColor = vec4(0.4 + hf, 0.23 + hf, 0.1 + hf, 1.0);
		} 
	)";

/*
			//gl_Position = viewMatrix * vec4(pos.x, pos.y, pos.z, 1.0);
			//gl_Position = rotationMatrix * viewMatrix * projectionMatrix * vec4(pos.x, pos.y, pos.z, 1.0);
		#version 330 core
		in vec3 FragPos;      // Fragment position in world space
		in vec3 Normal;       // Surface normal in world space
		in vec2 TexCoords;    // Texture coordinates
		uniform sampler2D texture1;    // Texture sampler
		out vec4 FragColor;		// Output to framebuffer
		void main() {
			// Calculate lighting
			vec3 lightPos = vec3(1.0, 1.0, 1.0);   // Example light position
			vec3 lightColor = vec3(1.0, 1.0, 1.0); // Example light color
			vec3 ambientColor = vec3(0.1, 0.1, 0.1); // Example ambient color
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(lightPos - FragPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = diff * lightColor;
			// Texture mapping
			vec4 texColor = texture(texture1, TexCoords);
			// Combine lighting and texture
			vec3 result = (ambientColor + diffuse) * texColor.rgb;
			FragColor = vec4(result, texColor.a);
		}
*/

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



	float grid[256][256];	//	y,x
	for (int j = 0; j < 256; j++) for (int i = 0; i < 256; i++) grid[j][i] = sin((i * .07f + j * (j - i) * .00047f) * .5f) * cos(i * j * .00053f) * 4.f - 15.f;
	//1 3 5..
	//0 2 4  two to begin, +1 per tri
	for (int j = 0; j < 255; j++) {
		int jx = j * 3060;	int jm = j + 1;
		for (int i = 0; i < 510; i += 2) {
			register int ix = jx + i * 6;	int tx = i >> 1;
			vertices[ix] = tx - 127;	vertices[ix + 1] = grid[j][tx];	vertices[ix + 2] = j - 127;
			vertices[ix + 3] = vertices[ix + 4] = vertices[ix + 5] = .5f;
			vertices[ix + 6] = tx - 127;	vertices[ix + 7] = grid[jm][tx];	vertices[ix + 8] = j - 126;
			vertices[ix + 9] = vertices[ix + 10] = vertices[ix + 11] = .5f;
		}
	}
	for (int k = 0; k < 255; k++) {
		unsigned long int tj = k * 510;
		for (int i = 0; i < 510; i += 2) {
			unsigned long int j = tj + i;
			indices[j] = j;		indices[j + 1] = j + 1;
		}
	}
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	//	this has to be after vao binding
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo);	//	element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	//was 0,3,,,3*so(float) for 3d points no colors
	//index of uniform/attribute, vecn,,[0,1] normalise flag! :),vecn or 0 to let gl decide, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// Specify the vertex array pointer to the vertex data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));	//	offset for color
	glEnableVertexAttribArray(1);

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
			//case 'T': do10x = !do10x;   break;
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
		
		SetTimer(hWnd, 1, 10, NULL);
	}	break;
    case WM_PAINT:	{
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
		myx += .005f;	if (myx > 1.f) myx -= 2.f;
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
