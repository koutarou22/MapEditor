#include <Windows.h>
#include<tchar.h>
#include "Direct3D.h"
#include "Quad.h"
#include "Camera.h"
//#include "Sprite.h"
#include "Input.h"
#include "Transform.h"
#include "Fbx.h"
//#include "Dice.h"
#include"Controller.h"
#include"Stage.h"
#include"resource.h"

Stage* pStage;
Controller* control;

using namespace Direct3D;

const wchar_t * WIN_CLASS_NAME = L"SampleGame";
const wchar_t* APP_NAME = L"Sample";
const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

//エントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
    wc.hInstance = hInstance;                   //インスタンスハンドル
    wc.lpszClassName = WIN_CLASS_NAME;          //ウィンドウクラス名
    wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
    wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
    wc.hIcon = LoadIcon(NULL, IDI_ERROR);       //アイコン
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
    wc.hCursor = LoadCursor(NULL, IDC_WAIT);    //マウスカーソル
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); //メニュー（なし）
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
    RegisterClassEx(&wc); //クラスを登録

    //ウィンドウサイズの計算(表示領域をWINDOW_WIDTH x WINDOW_HEIGHTに指定するための計算)
    RECT winRect = { 0, 0, Direct3D::WINDOW_WIDTH, Direct3D::WINDOW_HEIGHT };
    AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, TRUE);
    int winW = winRect.right - winRect.left;     //ウィンドウ幅
    int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ


    HWND hWnd = CreateWindow(
        WIN_CLASS_NAME,         //ウィンドウクラス名
        APP_NAME,     //タイトルバーに表示する内容
        WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
        CW_USEDEFAULT,       //表示位置左（おまかせ）
        CW_USEDEFAULT,       //表示位置上（おまかせ）
        winW,        //ウィンドウ幅
        winH,       //ウィンドウ高さ
        NULL,                //親ウインドウ（なし）
        NULL,                //メニュー（なし）
        hInstance,           //インスタンス
        NULL                 //パラメータ（なし）
    );

    ShowWindow(hWnd, nCmdShow);


    HWND hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc);
   

    //Direct3D初期化
   HRESULT hr =  Direct3D::Initialize(winW, winH, hWnd);
   
   Input::Initialize(hWnd);
   Camera::Initialize();

   pStage = new Stage;
   pStage->Initialize();
   control = new Controller;
   control->Initialize();
   
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Direct3Dの初期化に失敗しました", L"Error", NULL);
        return hr;
    }

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        //メッセージあり
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
           
        }

        //メッセージなし
        else
        {
            //カメラを更新
            Camera::Update();
            //ゲームの処理
            pStage->Update();
            control->Update();

            if (Input::IsKeyDown(DIK_ESCAPE))
            {
                static int cnt = 0;
                cnt++;
                if (cnt >= 3)
                {
                    PostQuitMessage(0);
                }
            }

            if (Input::IsMouseButton(0))
            {
               /*PostQuitMessage(0);*/
            }
         
            Direct3D::BeginDraw();
            Input::Update();
            pStage->Draw();
            Direct3D::EndDraw();
        }
    }

    //SAFE_DELETE(q);
    pStage->Release();
    SAFE_DELETE(pStage);
    SAFE_DELETE(control);

    Input::Release();
    Direct3D::Release();

  

	return 0;
}

//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return pStage->WndProc(hWnd, msg, wParam, lParam);//偽物のwindowプロシージャ
}

//ダイアログプロシージャ
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
   return pStage->DialogProc(hDlg, msg, wp, lp);//偽物のプロシージャ
}