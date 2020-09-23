#include <windows.h>
#include <gl/gl.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_CUBES 256

#define CUBE_ROTATING_SPEEED 0.2f
#define SMALL_CUBES_ROTATING_SPEED 0.2f

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);



float cubeRot = 0.0f;

BOOL allRotationPaused = FALSE;


BOOL cube[10][10][10];

typedef struct {
    float rotX;
    float rotY;
    float rotZ;
} CubeRotationMode;

typedef struct {
    float x;
    float y;
    float z;
    float x1;
    float y1;
    float z1;
    float rotation;
    CubeRotationMode rotMode;
    BOOL isInitialized;
} Cube;


Cube curCubes[MAX_CUBES];
int curCount = 0;

CubeRotationMode curRotMode = {
    1,
    1,
    0
};

typedef struct {
    float r;
    float g;
    float b;
} COLOR;

COLOR black;

COLOR white = {1.0f, 1.0f, 1.0f};

COLOR red = {1.0f, 0.0f, 0.0f};
COLOR green = {0.0f, 1.0f, 0.0f};
COLOR blue = {0.0f, 0.0f, 1.0f};
COLOR yellow = {1.0f, 1.0f, 0.0f};
COLOR magenta = {1.0f, 0.0f, 1.0f};



void DrawRectangle(float x, float y, float z, float w, float h, COLOR clr) {
    glBegin(GL_TRIANGLES);
        glColor3f(clr.r, clr.g, clr.b);

        glVertex3f(x, y, z);
        glVertex3f(x+w, y, z);
        glVertex3f(x, y+h, z);

        glVertex3f(x, y+h, z);
        glVertex3f(x+w, y+h, z);
        glVertex3f(x+w, y, z);
    glEnd();
}


void DrawPoint(float x, float y, COLOR clr, float pointSize) {
    glPointSize(pointSize);

    glBegin(GL_POINTS);
        glColor3f(clr.r, clr.g, clr.b);
        glVertex2f(x, y);
    glEnd();
}


void DrawCube(float x, float y, float z,
              float x1, float y1, float z1,
              COLOR firstColor, COLOR secondColor) {
    glBegin(GL_TRIANGLES);

        // Front side
        glColor3f(white.r, white.g, white.b);
        glVertex3f(x-x1, y-y1, z-z1); glVertex3f(x-x1, y+y1, z-z1); glVertex3f(x+x1, y+y1, z-z1);
        glVertex3f(x-x1, y-y1, z-z1); glVertex3f(x+x1, y-y1, z-z1); glVertex3f(x+x1, y+y1, z-z1);
        // Back side
        glColor3f(red.r, red.g, red.b);
        glVertex3f(x-x1, y-y1, z+z1); glVertex3f(x-x1, y+y1, z+z1); glVertex3f(x+x1, y+y1, z+z1);
        glVertex3f(x-x1, y-y1, z+z1); glVertex3f(x+x1, y-y1, z+z1); glVertex3f(x+x1, y+y1, z+z1);
        // Upper side
        glColor3f(blue.r, blue.g, blue.b);
        glVertex3f(x-x1, y+y1, z-z1); glVertex3f(x-x1, y+y1, z+z1); glVertex3f(x+x1, y+y1, z+z1);
        glVertex3f(x-x1, y+y1, z-z1); glVertex3f(x+x1, y+y1, z-z1); glVertex3f(x+x1, y+y1, z+z1);
        // Down side
        glColor3f(green.r, green.g, green.b);
        glVertex3f(x-x1, y-y1, z-z1); glVertex3f(x-x1, y-y1, z+z1); glVertex3f(x+x1, y-y1, z+z1);
        glVertex3f(x-x1, y-y1, z-z1); glVertex3f(x+x1, y-y1, z-z1); glVertex3f(x+x1, y-y1, z+z1);
        // Right side
        glColor3f(yellow.r, yellow.g, yellow.b);
        glVertex3f(x+x1, y-y1, z-z1); glVertex3f(x+x1, y+y1, z-z1); glVertex3f(x+x1, y+y1, z+z1);
        glVertex3f(x+x1, y-y1, z-z1); glVertex3f(x+x1, y-y1, z+z1); glVertex3f(x+x1, y+y1, z+z1);
        // Left side
        glColor3f(magenta.r, magenta.g, magenta.b);
        glVertex3f(x-x1, y-y1, z-z1); glVertex3f(x-x1, y+y1, z-z1); glVertex3f(x-x1, y+y1, z+z1);
        glVertex3f(x-x1, y-y1, z-z1); glVertex3f(x-x1, y-y1, z+z1); glVertex3f(x-x1, y+y1, z+z1);

    glEnd();
}


void RandomizeCube() {
    int step = 0;
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            for (int z = 0; z < 10; z++) {
                // step % 2 == 0
                if (0+rand()%100 < 75 && ((x != 3) || (x != 4)) && ((y != 3) || (y != 4)))
                    cube[x][y][z] = TRUE;
                else cube[x][y][z] = FALSE;

                step++;
            }
            step++;
        }
        step++;
    }
}


void RenderAll() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    for (int x = -5; x < 5; x++) {
        for (int y = -5; y < 5; y++) {
            for (int z = -5; z < 5; z++) {
                if (TRUE) {
                    if (cube[x+5][y+5][z+5]) DrawRectangle(x*0.1, y*0.1, z*0.1, 0.1f, 0.1f, white);
                } else {
                    // DrawRectangle(x*0.1, y*0.1, z*0.1, 0.1f, 0.1f, white);
                }
            }
        }
    }

    glPopMatrix();
}


void DrawRotationModes() {
    COLOR rxclr;
    if (curRotMode.rotX == -1.0f) rxclr = magenta;
    else if (curRotMode.rotX == 0.0f) rxclr = red;
    else if (curRotMode.rotX == 1.0f) rxclr = green;

    DrawRectangle(-0.9f, 0.8f, 0.0f, 0.1f, 0.1f, rxclr);

    COLOR ryclr;
    if (curRotMode.rotY == -1.0f) ryclr = magenta;
    else if (curRotMode.rotY == 0.0f) ryclr = red;
    else if (curRotMode.rotY == 1.0f) ryclr = green;

    DrawRectangle(-1.0f, 0.9f, 0.0f, 0.1f, 0.1f, ryclr);

    COLOR rzclr;
    if (curRotMode.rotZ == -1.0f) rzclr = magenta;
    else if (curRotMode.rotZ == 0.0f) rzclr = red;
    else if (curRotMode.rotZ == 1.0f) rzclr = green;

    DrawRectangle(-1.0f, 0.8f, 0.0f, 0.1f, 0.1f, rzclr);
}


void ScreenToOpenGL(HWND hwnd, int x, int y, float *ox, float *oy) {
    RECT rct;
    GetClientRect(hwnd, &rct);
    *ox = x / (float)rct.right * 2 - 1;
    *oy = 1 - y / (float)rct.bottom * 2;
}


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    srand(time(NULL));

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          700,
                          700,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glEnable(GL_DEPTH_TEST);

    RandomizeCube();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //glPushMatrix();
            //RenderAll();

            glLoadIdentity();

            glRotatef(theta, 1.0f, 1.0f, 0.0f);

            int step = 0;
            for (int x = 0; x < 10; x++) {
                for (int y = 0; y < 10; y++) {
                    for (int z = 0; z < 10; z++) {
                        if (cube[x][y][z]) {
                            DrawCube((x-5)*0.06f, (y-5)*0.06f, (z-5)*0.06f,
                                     0.03f, 0.03f, 0.03f,
                                     red, green);
                        }
                        step++;
                    }
                    step++;
                }
                step++;
            }
            // printf("%i\n", step);

            for (int i = 0; i < MAX_CUBES; i++) {
                Cube curCube = curCubes[i];
                if (curCube.isInitialized == TRUE) {
                    // printf("Initialized");

                    glLoadIdentity();

                    glRotatef(curCube.rotation, curCube.rotMode.rotX,
                                                curCube.rotMode.rotY,
                                                curCube.rotMode.rotZ);

                    DrawCube(curCube.x, curCube.y, curCube.z, curCube.x1, curCube.y1, curCube.z1, red, blue);

                    if (!allRotationPaused) {
                        curCubes[i].rotation += SMALL_CUBES_ROTATING_SPEED;
                        if (curCubes[i].rotation > 360) cubeRot = 0;
                    }
                }
            }

            glLoadIdentity();
            DrawRotationModes();

            SwapBuffers(hDC);

            if (!allRotationPaused) {
                theta += CUBE_ROTATING_SPEEED;
                if (theta > 360) theta = 0;
            }

            Sleep(1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_LBUTTONDOWN:
        {
            POINTFLOAT pf;
            ScreenToOpenGL(hwnd, LOWORD(lParam), HIWORD(lParam), &pf.x, &pf.y);
            printf("%f %f\n", (float)pf.x, (float)pf.y);
            Cube c;
            c.x = (float)pf.x;
            c.y = (float)pf.y;
            c.z = -0.5f;
            c.x1 = 0.05f;
            c.y1 = 0.05f;
            c.z1 = 0.05f;
            c.rotation = 0.0f;
            c.rotMode = curRotMode;
            c.isInitialized = TRUE;

            if (curCount == MAX_CUBES-1) {
                curCount = 0;
            }
            curCubes[curCount] = c;
            printf("%i\n", curCount);
            curCount++;

            // DrawPoint((float)pf.x, (float)pf.y, red, 10);
        }
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;

                case VK_RETURN:
                    printf("Randomizing\n");
                    RandomizeCube();
                break;

                case 0x54: // T key ( for y rotation )
                {
                    curRotMode.rotY += 1.0f;
                    if (curRotMode.rotY == 2.0f) curRotMode.rotY = -1.0f;
                    printf("curRotMode.rotY: %f\n", curRotMode.rotY);
                }
                break;

                case 0x47: // G key ( for z rotation )
                {
                    curRotMode.rotZ += 1.0f;
                    if (curRotMode.rotZ == 2.0f) curRotMode.rotZ = -1.0f;
                    printf("curRotMode.rotZ: %f\n", curRotMode.rotZ);
                }
                break;

                case 0x48: // H key ( for x rotation )
                {
                    curRotMode.rotX += 1.0f;
                    if (curRotMode.rotX == 2.0f) curRotMode.rotX = -1.0f;
                    printf("curRotMode.rotX: %f\n", curRotMode.rotX);
                }
                break;

                case 0x43: // C key ( to clear all cubes )
                {
                    for (int i = 0; i < MAX_CUBES; i++) {
                        Cube cube;
                        curCubes[i] = cube;
                    }
                }
                break;

                case VK_SPACE:
                {
                    allRotationPaused = !allRotationPaused;
                }
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
