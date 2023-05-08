
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>	//	mysteriously enough, "modern" OpenGL still only requires this header and the .lib

#include "framework.h"
#include "ogl.h"	//	my header trying to make modern opengl happen
#include "win32 OpenGL.h"	//	header for this project


#pragma comment(lib, "opengl32.lib")

// Function pointer types for OpenGL functions
//typedef void (WINAPI* GLPROC)(void);
typedef GLuint(APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef GLuint(APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef GLint(APIENTRYP PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint index, GLenum pname, GLvoid** pointer);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);

PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
//PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;


float myx = 0.f;

HWND hOGLWnd = NULL;
HGLRC hrc0 = NULL;	//	two render contexts to flip
HGLRC hrc1 = NULL;

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

   // Create OpenGL context
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


   //bool LoadOpenGLFunctions() {	// Function to load OpenGL function pointers
//	   glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(wglGetProcAddress("glGenBuffers"));
//	   glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(wglGetProcAddress("glBindBuffer"));
//	   glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(wglGetProcAddress("glBufferData"));
	   // Check if any function pointers are still nullptr, indicating failure to load
	   //if (!glGenBuffers || !glBindBuffer || !glBufferData) return false;
	   //if (!glGenBuffers) return false;
	   //return true;
   //}




   hrc0 = wglCreateContext(hDC);
   hrc1 = wglCreateContext(hDC);
   wglMakeCurrent(hDC, hrc1);




   HMODULE hOpenGL = LoadLibrary(L"opengl32.dll");
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

   //if (glGenVertexArrays == NULL) return 0;


   // release device context
//   ::ReleaseDC(hOGLWnd, hDC);

   const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";


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

    void main()
    {
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



   // Create and compile vertex shader
   GLuint vertexShader;
   if (glCreateShader != nullptr) vertexShader = glCreateShader(GL_VERTEX_SHADER);// 0x8B31GL_VERTEX_SHADER); 8B30 fragment shader  0x8DD9 geometry shader 0x91B9 compute shader
   glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
   glCompileShader(vertexShader);

   // Create and compile fragment shader
   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
   glCompileShader(fragmentShader);

   // Create shader program and attach vertex and fragment shaders
    GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   // Delete individual shaders as they are no longer needed
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   // Use the shader program for rendering
   glUseProgram(shaderProgram);



   ShowWindow(hOGLWnd, SW_SHOWDEFAULT);
   //ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   FreeLibrary(hOpenGL);

   return TRUE;
}


void display(void) {
	// Vertex data for a simple 3D triangle
	GLfloat vertices[] = {
		-0.6f, -0.6f, 0.0f, // Vertex 1: position (-0.6, -0.6, 0.0)
		0.6f, -0.6f, 0.0f, // Vertex 2: position (0.6, -0.6, 0.0)
		0.0f, 0.7f, 0.0f // Vertex 3: position (0.0, 0.6, 0.0)
	};

	// Create and bind Vertex Array Object (VAO)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and bind Vertex Buffer Object (VBO)
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Load vertex data into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specify the vertex array pointer to the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Clear the framebuffer
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f); // Black color
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Swap buffers to display the rendered image
	HDC hDC = GetDC(hOGLWnd);
	SwapBuffers(hDC);
	ReleaseDC(hOGLWnd, hDC);
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
