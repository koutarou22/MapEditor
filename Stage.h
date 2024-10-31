#pragma once
#include"Fbx.h"
#include"Transform.h"
class Stage
{
    FBX* pFbx[5];
    int Width;
    int Height;
    struct Deta
    {
        int height;
        int type;
    };
    Deta table[20][20];
   // std::vector<std::vector<Deta>> table;


    int SelectMode;//�グ��̂�������̂��ς���̂��H
    int SelectType;//�f�t�H���g�Ȃ̂��H�����Ȃ̂�

    void Save();
    void Open();
public:
    //�R���X�g���N�^
    Stage();

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize();

    //�X�V
    void Update();

    //�`��
    void Draw();

    //�J��
    void Release();

    LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};

