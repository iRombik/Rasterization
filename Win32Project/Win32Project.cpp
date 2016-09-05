// Win32Project.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Win32Project.h"
#include "Painter.h"
#include "Cube.h"
#include "Camera.h"
#include "Pixel.h"

#define MAX_LOADSTRING 100
float material_specular_exponent;
Cube cube;
Camera camera;
bool rotate_flag;

HDC hdcScr; //���������� ���� ����
HDC _hdc; //���������� ����

int buffer_size;
BYTE* image_buffer;

Pixel* left_border_buffer;
Pixel* right_border_buffer;

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����

// ��������� ���������� �������, ���������� � ���� ������ ����:
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

    // TODO: ���������� ��� �����.
	material_specular_exponent = 20.0f;

	float angle_of_incline = float(10.0 / 180.0 * 3.14);
	//Vector4d z_vector_of_cube(0, sinf(angle_of_incline), cosf(angle_of_incline), 0);
	Vector4d z_vector_of_cube(0, 0, 1, 0);
	Vector4d offset(0, 0, 0, 1);
	float radius = 10000;
	cube = Cube(z_vector_of_cube, offset, radius);
	cube.calulateCenterMatrix();

	Vector4d z_vector_of_camera(0, 0, 1, 0);
	float camera_sphere_radius = 50000;
	camera = Camera(z_vector_of_camera, camera_sphere_radius);
	camera.CalculatePosition();

    // ������������� ���������� �����
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ��������� ������������� ����������:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT));

    MSG msg;

    // ���� ��������� ���������:
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



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName =  MAKEINTRESOURCEW(IDC_WIN32PROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND � ���������� ���� ����������
//  WM_PAINT � ���������� ������� ����
//  WM_DESTROY � ��������� ��������� � ������ � ���������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
		_hdc = GetDC(hWnd);
		hdcScr = CreateCompatibleDC(_hdc);//������� ����������
		//find width and height of the virtual screen, in pixels
		int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		buffer_size = 4 * h * w;

		SelectObject(hdcScr, CreateCompatibleBitmap(_hdc, w, h)); //������� ���� ��� � �������
		PatBlt(hdcScr, 0, 0, w, h, PATCOPY);//�������� ���������
		
		image_buffer = new BYTE[buffer_size];
		left_border_buffer = new Pixel[h];
		right_border_buffer = new Pixel[h];
		}
		break;
		
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // ��������� ����� � ����:
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
        }
        break;
		

	case WM_KEYDOWN:
		switch (wParam)
		{
		case 87: // W
			camera.goUp();
			break;

		case 83: // S
			camera.goDown();
			break;

		case 65: // A
			camera.goLeft();
			break;

		case 68: // D
			camera.goRight();
			break;
	
		case 81: // Q
			if (GetKeyState(VK_CONTROL) & 0x8000) { // Ctrl + Q
				if(material_specular_exponent > 0.0f)
					material_specular_exponent -= 20.0f;
			}
			else {
				camera.rotateToLeft();
			}
			break;

		case 69: // E
			if (GetKeyState(VK_CONTROL) & 0x8000) { // Ctrl + E
				material_specular_exponent += 20.0f;
			}
			else {
				camera.rotateToRight();
			}
			break;

		//return to start position
		case 82: // R
			camera.CalculatePosition();
			break;
		
		//stop cube rotating
		case VK_SPACE:
			rotate_flag = !rotate_flag;
			break;
		
		default:
			break;
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �������� ���� ����� ��� ����������, ������������ HDC...
			Painter p(hdcScr, ps.rcPaint.right, ps.rcPaint.bottom);
			p.DrawCube(cube, camera, 
				rotate_flag, material_specular_exponent,
				left_border_buffer, right_border_buffer,
				image_buffer);

			BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcScr, 0, 0, SRCCOPY);
			memset(image_buffer, 0, buffer_size);
            EndPaint(hWnd, &ps);
			InvalidateRect(hWnd, 0, true);
        }
        break;
	
	case WM_ERASEBKGND:
		{
		}
		break;
		
    case WM_DESTROY:
		delete[] image_buffer;
		delete[] left_border_buffer, right_border_buffer;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���������� ��������� ��� ���� "� ���������".
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
