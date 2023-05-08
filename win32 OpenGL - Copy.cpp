

#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>

#pragma comment(lib, "opengl32.lib")

#include "framework.h"
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
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32OPENGL));

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
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

	if (!hWnd) {
		return FALSE;
	}

	hOGLWnd = hWnd;
	HDC hDC = ::GetDC(hOGLWnd);

	DWORD pixelFormatFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_COMPOSITION | PFD_GENERIC_ACCELERATED | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		pixelFormatFlags,         //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                       //Colordepth of the framebuffer.
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


	HMODULE hOpenGL = LoadLibrary(L"opengl32.dll");	//	begin "modern" opengl API appropriation
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

	glUniform3fv = reinterpret_cast<PFNGLUNIFORM3FVPROC>(wglGetProcAddress("glUniform3fv"));
	glUniform4fv = reinterpret_cast<PFNGLUNIFORM4FVPROC>(wglGetProcAddress("glUniform4fv"));
	glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(wglGetProcAddress("glGetUniformLocation"));
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

	if (glUniform3fv == NULL) return 0;
	if (glUniform4fv == NULL) return 0;
	if (glGetUniformLocation == NULL) return 0;
/*
	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		void main() {
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
	)";
*/



	const char* vertexShaderSource = R"(
		#version 330 core
		uniform mat4 modelviewProjection;
		uniform vec3 cameraPosition;
		layout (location = 0) in vec3 aPos;
		void main() {
			gl_Position = modelviewProjection * vec4(aPos, 1.0);
		}
	)";
/*
	const char* vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		uniform vec3 cameraPosition;
		uniform vec3 cameraRotation;
		uniform vec3 cameraUp;
		layout (location = 0) in vec3 aPos;
		void main() {
			// Apply camera rotation
			mat3 rotX = mat3(1.0, 0.0, 0.0,
							0.0, cos(cameraRotation.x), sin(cameraRotation.x),
							0.0, -sin(cameraRotation.x), cos(cameraRotation.x));
			mat3 rotY = mat3(cos(cameraRotation.y), 0.0, -sin(cameraRotation.y),
							0.0, 1.0, 0.0,
							sin(cameraRotation.y), 0.0, cos(cameraRotation.y));
			mat3 rotZ = mat3(cos(cameraRotation.z), sin(cameraRotation.z), 0.0,
							-sin(cameraRotation.z), cos(cameraRotation.z), 0.0,
							0.0, 0.0, 1.0);
			mat3 rotationMatrix = rotX * rotY * rotZ;

			// Apply camera position and rotation to vertex position
			vec3 rotatedPos = rotationMatrix * aPos;
			vec3 transformedPos = rotatedPos + cameraPosition : cameraUp;
			gl_Position = vec4(transformedPos.x, transformedPos.y, transformedPos.z, 1.0);
		}
	)";
*/



	const char* fragmentShaderSource = R"(
		#version 330 core
		// Input from vertex shader
		in vec3 FragPos;      // Fragment position in world space
		in vec3 Normal;       // Surface normal in world space
		in vec2 TexCoords;    // Texture coordinates
		// Uniforms
		uniform sampler2D texture1;    // Texture sampler
		// Output to framebuffer
		out vec4 FragColor;
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
	)";




	vertexShader;
	if (glCreateShader != nullptr) vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);


	ShowWindow(hOGLWnd, SW_SHOWDEFAULT);	//ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	FreeLibrary(hOpenGL);
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
	case WM_CREATE: {
		RECT clientRect;
		GetClientRect(hOGLWnd, &clientRect);
		glViewport(0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

		SetTimer(hWnd, 1, 10, NULL);
	}	break;
    case WM_PAINT:	{
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
		myx += .01f;	if (myx > 1.f) myx -= 2.f;
		display();
        EndPaint(hWnd, &ps);
    }	break;
	case WM_TIMER: {
		InvalidateRgn(hWnd, 0, 0);	UpdateWindow(hWnd);
	}   break;
	case WM_SIZE: {
		RECT clientRect;
		GetClientRect(hOGLWnd, &clientRect);
		glViewport(0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}	break;
    case WM_DESTROY:
		glUseProgram(0);
		glBindVertexArray(0);	//	unbinds
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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
