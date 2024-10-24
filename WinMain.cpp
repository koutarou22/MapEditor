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
const int WINDOW_WIDTH = 800;  //�E�B���h�E�̕�
const int WINDOW_HEIGHT = 600; //�E�B���h�E�̍���

//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

//�G���g���[�|�C���g
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);             //���̍\���̂̃T�C�Y
    wc.hInstance = hInstance;                   //�C���X�^���X�n���h��
    wc.lpszClassName = WIN_CLASS_NAME;          //�E�B���h�E�N���X��
    wc.lpfnWndProc = WndProc;                   //�E�B���h�E�v���V�[�W��
    wc.style = CS_VREDRAW | CS_HREDRAW;         //�X�^�C���i�f�t�H���g�j
    wc.hIcon = LoadIcon(NULL, IDI_ERROR);       //�A�C�R��
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //�������A�C�R��
    wc.hCursor = LoadCursor(NULL, IDC_WAIT);    //�}�E�X�J�[�\��
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); //���j���[�i�Ȃ��j
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //�w�i�i���j
    RegisterClassEx(&wc); //�N���X��o�^

    //�E�B���h�E�T�C�Y�̌v�Z(�\���̈��WINDOW_WIDTH x WINDOW_HEIGHT�Ɏw�肷�邽�߂̌v�Z)
    RECT winRect = { 0, 0, Direct3D::WINDOW_WIDTH, Direct3D::WINDOW_HEIGHT };
    AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, TRUE);
    int winW = winRect.right - winRect.left;     //�E�B���h�E��
    int winH = winRect.bottom - winRect.top;     //�E�B���h�E����


    HWND hWnd = CreateWindow(
        WIN_CLASS_NAME,         //�E�B���h�E�N���X��
        APP_NAME,     //�^�C�g���o�[�ɕ\��������e
        WS_OVERLAPPEDWINDOW, //�X�^�C���i���ʂ̃E�B���h�E�j
        CW_USEDEFAULT,       //�\���ʒu���i���܂����j
        CW_USEDEFAULT,       //�\���ʒu��i���܂����j
        winW,        //�E�B���h�E��
        winH,       //�E�B���h�E����
        NULL,                //�e�E�C���h�E�i�Ȃ��j
        NULL,                //���j���[�i�Ȃ��j
        hInstance,           //�C���X�^���X
        NULL                 //�p�����[�^�i�Ȃ��j
    );

    ShowWindow(hWnd, nCmdShow);


    HWND hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc);
   

    //Direct3D������
   HRESULT hr =  Direct3D::Initialize(winW, winH, hWnd);
   
   Input::Initialize(hWnd);
   Camera::Initialize();

   pStage = new Stage;
   pStage->Initialize();
   control = new Controller;
   control->Initialize();
   
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Direct3D�̏������Ɏ��s���܂���", L"Error", NULL);
        return hr;
    }

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        //���b�Z�[�W����
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
           
        }

        //���b�Z�[�W�Ȃ�
        else
        {
            //�J�������X�V
            Camera::Update();
            //�Q�[���̏���
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

//�E�B���h�E�v���V�[�W���i�������������ɂ�΂��֐��j
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return pStage->WndProc(hWnd, msg, wParam, lParam);//�U����window�v���V�[�W��
}

//�_�C�A���O�v���V�[�W��
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
   return pStage->DialogProc(hDlg, msg, wp, lp);//�U���̃v���V�[�W��
}