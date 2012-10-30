#include "global.h"


HWND			hWnd;
GInput			Input;
bool			bEndFlag;
CartGE			test;
Camera			Cam1;

LRESULT CALLBACK BasicWindowProc(HWND wpHWnd,
						    UINT msg, 
                            WPARAM wParam, 
                            LPARAM lParam);


void MakeWindow(HINSTANCE hInstance, char *szTitle, char *szClassName, int iX, int iY, int iWidth, int iHeight, bool bFullScreen)
{
	WNDCLASSEX winClass;

	winClass.cbSize         = sizeof(WNDCLASSEX); 
	winClass.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc	= BasicWindowProc;
	winClass.cbClsExtra		= 0;
	winClass.cbWndExtra		= 0; 
	winClass.hInstance		= hInstance; 
	winClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION); 
	winClass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
	winClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH); 
	winClass.lpszMenuName	= NULL; 
	winClass.lpszClassName	= szClassName; 
	winClass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);
	
	if (!RegisterClassEx(&winClass)) 
		return;

	if (bFullScreen==false)
	{
		hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,  
							 szClassName,     
							 szTitle, 
							 WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE, 
					 		 iX, iY,
							 iWidth, iHeight,
							 NULL,
							 NULL,
							 hInstance,  
							 NULL);	
	}
	else
	{
		hWnd = CreateWindowEx(NULL,  
							 szClassName,     
							 szTitle,  
							 WS_POPUP | WS_VISIBLE, 
					 		 iX, iY,
							 iWidth, iHeight,
							 NULL,
							 NULL,
							 hInstance,  
							 NULL);	
	}
}


LRESULT CALLBACK BasicWindowProc(HWND wpHWnd, 
						    UINT msg, 
                            WPARAM wParam, 
                            LPARAM lParam)
{


	switch(msg)
	{	
		case WM_DESTROY: 
		{ 
			bEndFlag=true;
//			PostQuitMessage(0);
			return 0;
		} break;

		case WM_KEYDOWN:									
			Input.KeySet(wParam, true);
			break;

		case WM_KEYUP:										
			Input.KeySet(wParam, false);
			break;

		default:
			return DefWindowProc(wpHWnd, msg, wParam, lParam);
	} 
	return (0);
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) 
{ 
	MSG			msg;
	CMap		testmap;
	bool		bFullscreen=true;

	MakeWindow(hInstance, "artGE Demo", "SyncLabs", 0, 0, 800, 600, bFullscreen);
	test.Create(hWnd,800,600,bFullscreen);
	test.Init();
	test.LoadMap("map1.txt",&testmap);
	test.Timer.fTimeElapsed=1.0f;
	Cam1.Create(0.0f, -2.0f, -20.0f,"Follow CAM");



	while(!bEndFlag)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
			
				return TRUE;
			}
		}
	//CODE			
	if (Input.KeyGet(VK_LEFT))  Cam1.Rotate(0.0f, -0.002f*test.Timer.fTimeElapsed, 0.0f);
	if (Input.KeyGet(VK_RIGHT)) Cam1.Rotate(0.0f, 0.002f*test.Timer.fTimeElapsed, 0.0f);
	if (Input.KeyGet(VK_UP))  Cam1.Translate(0.0f,0.0f,0.02f*test.Timer.fTimeElapsed);
	if (Input.KeyGet(VK_DOWN))  Cam1.Translate(0.0f,0.0f,-0.02f*test.Timer.fTimeElapsed);
	if (Input.KeyGet('A'))  Cam1.Translate(-0.02f*test.Timer.fTimeElapsed,0.0f,0.0f);
	if (Input.KeyGet('D'))  Cam1.Translate(0.02f*test.Timer.fTimeElapsed,0.0f,0.0f);
	if (Input.KeyGet('W'))  Cam1.Rotate(-0.002f*test.Timer.fTimeElapsed,0.0f, 0.0f);
	if (Input.KeyGet('S'))  Cam1.Rotate(0.002f*test.Timer.fTimeElapsed,0.0f, 0.0f);
	
	if (Input.KeyGet('Q'))  {test.TakeScreenShot("bild.bmp",800,600); Input.KeySet('Q',false);};

	Cam1.Update();
	test.lightDir=Cam1.vDir;
	test.CameraSet(&Cam1.matView);
	test.FrameBegin();
	test.RenderShadowedMap(&testmap);
	test.RenderMap(&testmap);
	test.Print(10,60,"%f",Cam1.vDir.x);
	test.Print(10,90,"%s",testmap.cName);
	test.FrameEnd();
	test.Timer.TimeSet(timeGetTime());
	test.Timer.Update();
	
	
	} 
	
	
	return 0;
} 