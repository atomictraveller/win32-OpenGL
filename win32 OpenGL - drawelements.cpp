
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
		out vec2 texCoord;
		uniform vec3 cam;
		uniform mat4 modelMatrix; 
		uniform mat4 viewMatrix;
		uniform mat4 projectionMatrix;
		void main() {
			gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos.x, pos.y, pos.z, 1.0);
			vertexColor = vec4(color, 1.0);
			texCoord = vec2(pos.x + pos.y, pos.z + pos.y);
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor; 
		in vec4 vertexColor;
		in vec2 texCoord;
		uniform vec4 ourColor;
 




precision mediump int;
precision mediump float;

vec4 mod289(vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec2 P)
{
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod289(Pi); // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;

  vec4 i = permute(permute(ix) + iy);

  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  vec4 gy = abs(gx) - 0.5 ;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
  g00 *= norm.x;
  g01 *= norm.y;
  g10 *= norm.z;
  g11 *= norm.w;

  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));

  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}

// Classic Perlin noise, periodic variant
float pnoise(vec2 P, vec2 rep)
{
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod(Pi, rep.xyxy); // To create noise with explicit period
  Pi = mod289(Pi);        // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;

  vec4 i = permute(permute(ix) + iy);

  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  vec4 gy = abs(gx) - 0.5 ;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
  g00 *= norm.x;
  g01 *= norm.y;
  g10 *= norm.z;
  g11 *= norm.w;

  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));

  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}

//=========================================================================
// Fragment shader
// By: Dr. Wayne Brown, Spring 2016

uniform float u_Scale = 3.7f;
uniform float u_S_factor = -.916016f;
uniform float u_T_factor = -.818766f;

varying vec4 v_vertex_color;
varying vec2 v_Texture_coordinate;

//-------------------------------------------------
// Calculate a color based on a pattern defined
// by the texture coordinates
vec4 noise(vec2 tex_coords) {

  float percent = (1.0 + pnoise(u_Scale * tex_coords, vec2(u_S_factor, u_T_factor))) / 2.0;

  return vec4(percent, percent, percent, 1.0);
}

//-------------------------------------------------
//void main() {
//  gl_FragColor = noise(v_Texture_coordinate);
//}










		void main() {
			float hf = fract(texCoord.x + texCoord.y);
			hf *= (1.f - hf) * 4.f; 
			FragColor = vec4(hf, hf, hf, 1.f);
		} 
	)";

/*gl_Position = project * view * model * vec4(position, 1.0);
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

/*	float vertices[] = {
		-.25f, 0.f, -.5f,	.5f, 0.f, .5f,
		.25f, 0.f, -.5f,		0.f, .5f, .5f,
		-.25f, .5f, -.5f,		.5f, .5f, 0.f,
		.25f, .5f, -.5f,		.5f, 0.f, .5f
	};
	unsigned int indices[] = {  //start from 0!
		0, 1, 2,
		1, 2, 3
	};
*/

/*
	float vertices[] = {
		-2.f,-1.f,-2.f,		1.f,.0f,.0f,
		-2.f,-1.f,-1.f,		.5f,.5f,.5f,
		-1.f,-1.f,-2.f,		.5f,.5f,.5f,
		-1.f,-1.f,-1.f,		.5f,.5f,.5f,
		0.f,-1.f,-2.f,		.5f,.5f,.5f,
		0.f,-1.f,-1.f,		.5f,1.f,.5f,
		1.f,-1.f,-2.f,		.5f,.5f,.5f,
		1.f,-1.f,-1.f,		.5f,.5f,.5f,
		2.f,-1.f,-1.f,		.5f,.5f,.5f,
		2.f,-1.f,-2.f,		0.f,.0f,1.f,

		-2.f,-1.f,-1.f,		.5f,.5f,.5f,
		-2.f,-1.f,0.f,		.5f,.5f,.5f,
		-1.f,-1.f,-1.f,		.5f,.5f,.5f,
		-1.f,-1.f,0.f,		.5f,.5f,.5f,
		0.f,-1.f,-1.f,		.5f,.5f,.5f,
		0.f,-1.f,0.f,		.5f,.5f,.5f,
		1.f,-1.f,-1.f,		.5f,.5f,.5f,
		1.f,-1.f,0.f,		.5f,.5f,.5f,
		2.f,-1.f,-1.f,		.5f,.5f,.5f,
		2.f,-1.f,0.f,		.5f,.5f,.5f
	};
	unsigned int indices[] = {  //start from 0!
		0, 1, 2,
		1, 2, 3,
		2, 3, 4,
		3, 4, 5,
		4, 5, 6,
		5, 6, 7,
		6, 7, 8,
		6, 8, 9,

		10, 11, 12,
		11, 12, 13,
		12, 13, 14,
		13, 14, 15,
		14, 15, 16,
		15, 16, 17,
		16, 17, 18,
		17, 18, 19

	};
*/

	const int p2 = 256;
	float grid[p2][p2];	//	y,x
	for (int j = 0; j < 256; j++) for (int i = 0; i < 256; i++) grid[j][i] = sin((i + j) * .25f) - 1.5f;
	//1 3 5..
	//0 2 4  two to begin, +1 per tri

	for (int j = 0; j < 255; j++) {
		register int jx = j * 3072;	int jm = (j + 1) % 255;
		vertices[jx] = -127.f;	vertices[jx + 1] = grid[j][0];	vertices[jx + 2] = j - 127;
		vertices[jx + 3] = vertices[jx + 4] = vertices[jx + 5] = .5f;
		vertices[jx + 6] = -127.f;	vertices[jx + 7] = grid[jm][0];	vertices[jx + 8] = j - 126;
		vertices[jx + 9] = vertices[jx + 10] = vertices[jx + 11] = .5f;
		for (int i = 2; i < 510; i += 2) {
			int ix = jx + i * 6;	int tx = i >> 1;
			vertices[ix] = tx - 127;	vertices[ix + 1] = grid[j][tx];	vertices[ix + 2] = j - 127;
			vertices[ix + 3] = vertices[ix + 4] = vertices[ix + 5] = .5f;
			vertices[ix + 6] = tx - 127;	vertices[ix + 7] = grid[jm][tx];	vertices[ix + 8] = j - 126;
			vertices[ix + 9] = vertices[ix + 10] = vertices[ix + 11] = .5f;
		}
	}
/*

	for (int j = 0; j < 255; j++) {
		register int jx = j * 3072;	int jm = (j + 1) % 255;

		for (int i = 0; i < 510; i += 2) {
			int ix = jx + i * 6;	int tx = i >> 1;

			vertices[ix] = tx - 127;	vertices[ix + 1] = grid[j][tx];	vertices[ix + 2] = j - 127;
			vertices[ix + 3] = vertices[ix + 4] = vertices[ix + 5] = .5f;
			vertices[ix + 6] = tx - 127;	vertices[ix + 7] = grid[jm][tx];	vertices[ix + 8] = j - 126;
			vertices[ix + 9] = vertices[ix + 10] = vertices[ix + 11] = .5f;
		}
	}


*/

	//for (int i = 0; i < 87040; i++) {
	for (int k = 0; k < 255; k++) {
		unsigned long int tj = k * 1530;
		for (int i = 0; i < 510; i += 2) {
			unsigned long int j = tj + i * 3;
			indices[j] = j;		indices[j + 1] = j + 1;		indices[j + 2] = j + 2;
			indices[j + 3] = j + 3;	indices[j + 4] = j + 4;	indices[j + 5] = j + 5;
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
			case 'A':		zrv += .005f;	if (zrv > 1.f) zrv = 1.f;	break;
			case 'Z':		zrv -= .005f;	if (zrv < -1.f) zrv = -1.f;	break;
			case VK_LEFT:	xrv += .005f;	if (xrv > 1.f) xrv = 1.f;	break;
			case VK_RIGHT:	xrv -= .005f;	if (xrv < -1.f) xrv = -1.f;	break;
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
