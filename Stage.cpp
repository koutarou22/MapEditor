#include "Stage.h"
#include"Input.h"
#include"Camera.h"
#include"Direct3D.h"
#include "resource.h"

Stage::Stage() : pFbx(), Width(20), Height(20)
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Height; z++)
        {
            table[x][z].height = 1;
            table[x][z].type = 0;
        }
    }
    table[0][0].height = 5;
    table[3][3].height = 2;
    table[10][1].height = 3;

    table[0][0].type = 1;
    table[3][3].type = 2;
    table[10][1].type = 3;

}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    string fileName[] = { "BoxDefault", "BoxBrick", "BoxGrass", "BoxSand", "BoxWater" };

    for (int i = 0; i < 5; i++)
    {
        pFbx[i] = new FBX;
        string path = "Assets/" + fileName[i] + ".fbx";
        pFbx[i]->Load(path);
    }
 
}

void Stage::Update()
{
    //RayCastDeta data;
    //data.start = XMFLOAT4(100, 5, 0, 0);
    //data.dir = XMFLOAT4(0, -1, 0, 0);

    //Transform trans;
    //trans.position_.x = 1;

    ////�����܂�FBX�̃��C�L���X�g�Ȃ̂�
    //pFbx[0]->RayCast(data, trans);

    if (Input::IsMouseButtonDown(0))
    {
        float w = Direct3D::WINDOW_WIDTH / 2;
        float h = Direct3D::WINDOW_HEIGHT / 2;
        //Offsetx,y ��0
        //minZ =0 maxZ = 1

        XMMATRIX vp =
        {
             w,  0,  0, 0,
             0, -h,  0, 0,
             0,  0,  1, 0,
             w,  h,  0, 1
        };

        //���C���΂�����
        XMMATRIX matView = Camera::GetViewMatrix();//�r���[�s��
        XMMATRIX matProj = Camera::GetProjectionMatrix();//�v���W�F�N�V�����s��

        //�r���[�|�[�g
        XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
        //�v���W�F�N�V�����ϊ�
        XMMATRIX invProj = XMMatrixInverse(nullptr, matProj);
        //�r���[�ϊ�
        XMMATRIX invView = XMMatrixInverse(nullptr, matView);


        XMFLOAT3 mousePosBack = Input::GetMousePosition();
        mousePosBack.z = 0.0f;
        XMFLOAT3 mousePosFront = Input::GetMousePosition();
        mousePosFront.z = 1.0f;
        //�@�@mousePosFront���x�N�g���ɕϊ�
        XMVECTOR vMouseFront = XMLoadFloat3(&mousePosBack); // �J�������猩����͈͂ň�Ԍ��̖ʂ��擾
         
        //�����ŕϊ�����(�O) 
        // �@��invVP�AinvPrj�AinvView��������
        vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
        //�B�@mousePosBack���x�N�g���ɕϊ�
        XMVECTOR vMouseBack = XMLoadFloat3(&mousePosFront);
        //�C�@�B��invVP�AinvPrj�AinvView��������
        vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

        RayCastDeta data;
        XMStoreFloat3(&data.start, vMouseFront);
        XMStoreFloat3(&data.dir, vMouseBack - vMouseFront);
     
        for (int x = 0; x < Width; x++)
        {
            for (int z = 0; z < Height; z++)
            {
                for (int y = 0; y < table[x][z].height; y++)
                {
                    Transform trs;
                    trs.position_.x = x;
                    trs.position_.y = y;
                    trs.position_.z = z;


                    int type = table[x][z].type;
                    pFbx[type]->RayCast(data, trs);
                    if (data.hit == true)
                    {
                       // PostQuitMessage(0);
                        table[x][z].height++;
                        return;//���܂���
                    }
                }
            }
        }
    }
}

void Stage::Draw()
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Height; z++)
        {
            for (int y = 0; y < table[x][z].height; y++)
            {
                Transform trs;
                trs.position_.x = x;
                trs.position_.y = y;
                trs.position_.z = z;

                int type = table[x][z].type;
                pFbx[type]->Draw(trs);

            }
        }
    }

}

void Stage::Release()
{
    for (int i = 0; i < 5; i++)
    {
        pFbx[i]->Release();
        SAFE_DELETE(pFbx[i]);
    }
}

//�_�C�A���O�v���V�[�W�����ۂ��Ԃ̑��l
BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    int a = 0;
    switch (msg)//���������������̂Ȃ�TRUE��Ԃ�
    {
    case WM_INITDIALOG:
        /*HWND h = GetDlgItem(hDlg, IDC_RADIO_UP);*/
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);

        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"�f�t�H���g");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"�����K");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"��");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"��");
        break;

        //case WM_MOUSEMOVE:
        //      Input::SetMousePosition(LOWORD(lp), HIWORD(lp));
        //      PostQuitMessage(0);
        //      return 0;

    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case IDC_RADIO_UP:
            break;

        case IDC_RADIO_DOWN:
            break;

        case IDC_RADIO_CHANGE:
            break;

        case IDC_MENU_NEW:
            break;

        case IDC_MENU_OPEN:
            break;

        case IDC_MENU_SAVE:
            break;
        }
    }


    return FALSE;
}