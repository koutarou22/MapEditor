#include "Stage.h"
#include"Input.h"
#include"Camera.h"
#include"Direct3D.h"
#include "resource.h"
#include <iostream>

using std::string;

Stage::Stage() : pFbx(), Width(20), Height(20),SelectMode(0),SelectType(0)
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Height; z++)
        {
            table[x][z].height = 1;
            table[x][z].type = 0;
        }
    }
 /*   table[0][0].height = 5;
    table[3][3].height = 2;
    table[10][1].height = 3;

    table[0][0].type = 1;
    table[3][3].type = 2;
    table[10][1].type = 3;*/
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
        XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront); // �J�������猩����͈͂ň�Ԍ��̖ʂ��擾
         
        //�����ŕϊ�����(�O) 
        // �@��invVP�AinvPrj�AinvView��������
        vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
        //�B�@mousePosBack���x�N�g���ɕϊ�
        XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
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
                        switch (SelectMode)
                        {
                        case 0:
                            table[x][z].height++;
                            break;
                        case 1:
                            if (table[x][z].height > 1)
                            {
                                table[x][z].height--;
                            }
                            break;
                        case 2:
                            table[x][z].type = SelectType;
                            break;
                        }

                        return;
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

void Stage::Save()
{
 /*   string s = "";
    s += "" + table[0][0].height;
    s += ",";
    s += "" + table[0][0].height;
    s += ",";
    s += "" + table[0][0].height;
    s += ",";
    s += "" + table[0][0].height;
    s += ",";
    s += "" + table[0][0].height;
    s += ",";*/

   // ///////////////////////////////////////
   // char d[256];
   // sprintf(d, "%d", table[0][0].height);
   /////////////////////////////////////////

    //���C�h������ɂ���Ή���
    WCHAR fileName[MAX_PATH] = L"����.txt";  //�t�@�C����������ϐ�

    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.txt)\0*.txt\0")                         //�����t�@�C���̎�ނ�I�ԂƂ��ɏo�Ă�����
                      TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");//��ԉ���\0�����     //�������g���q��ς��Ă����g(.txt)�͕ς��Ȃ���I
    ofn.lpstrFile = fileName;               	//�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	    //�p�X�̍ő啶����// window���񂩂�260�������p�ӂ���Ă���
    ofn.Flags = OFN_OVERWRITEPROMPT;   		    //�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = L"txt";                  	//�f�t�H���g�g���q

    //�u�t�@�C����ۑ��v�_�C�A���O
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);//���ꂪ���O��t���ĕۑ��̉�ʂ��o���֐�

    //�L�����Z�������璆�f
    if (selFile == FALSE) return;//�����L�����Z���������ꂽ��



      //�P.�܂�����ȃm�[�g�������Ă���(�C���[�W)
      //���t�@�C���n���h��
      HANDLE hFile;            //�t�@�C���̃n���h��
      hFile = CreateFile(
      L"Stage.txt",            //�t�@�C����
      GENERIC_WRITE,           //�A�N�Z�X���[�h�i�������ݗp�j
      0,                       //���L�i�Ȃ��j
      NULL,                    //�Z�L�����e�B�����i�p�����Ȃ��j
      CREATE_ALWAYS,           //�쐬���@
      FILE_ATTRIBUTE_NORMAL,   //�����ƃt���O�i�ݒ�Ȃ��j
      NULL);                   //�g�������i�Ȃ��j
    
      //�Q.�m�[�g�ɏ�������(�C���[�W)
      DWORD dwBytes = 0; //�������݈ʒu

      //��������Map�̏���ǉ����Ă���
      string MapData = "";
      for (int x = 0; x < Width; x++)
      {
          for (int z = 0; z < Height; z++)
          {
              MapData += std::to_string(table[x][z].height) + ",";
              MapData += std::to_string(table[x][z].type)   + ",";
          }
      }

      WriteFile(
      hFile,                   //�t�@�C���n���h��
      MapData.c_str(),         //�ۑ�����f�[�^�i������j
      MapData.size(),          //�������ޕ�����
      &dwBytes,                //�������񂾃T�C�Y������ϐ�
      NULL);                   //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j
     
      //�R.�{�I�ɖ߂� �������茩�Ă�{(text)����邱��(�C���[�W)
      CloseHandle(hFile);
}


void Stage::Open()
{
    ///
    //����Ă邱�Ƃ�Save()�Ƃقړ���
    ///
    WCHAR fileName[MAX_PATH] = L"����.txt";  //�t�@�C����������ϐ�

    //�u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�
    OPENFILENAME ofn;                         	//���O�����ĕۑ��_�C�A���O�̐ݒ�p�\����
    ZeroMemory(&ofn, sizeof(ofn));            	//�\���̏�����
    ofn.lStructSize = sizeof(OPENFILENAME);   	//�\���̂̃T�C�Y
    ofn.lpstrFilter = TEXT("�}�b�v�f�[�^(*.txt)\0*.txt\0")                         //�����t�@�C���̎�ނ�I�ԂƂ��ɏo�Ă�����
                      TEXT("���ׂẴt�@�C��(*.*)\0*.*\0\0");//��ԉ���\0�����     //�������g���q��ς��Ă����g(.txt)�͕ς��Ȃ���I
    ofn.lpstrFile = fileName;               	//�t�@�C����
    ofn.nMaxFile = MAX_PATH;               	    //�p�X�̍ő啶����// window���񂩂�260�������p�ӂ���Ă���
    ofn.Flags = OFN_FILEMUSTEXIST;   		    //�t���O�i�����t�@�C�������݂�����㏑���m�F�j
    ofn.lpstrDefExt = L"txt";                  	//�f�t�H���g�g���q

    //�u�t�@�C�����J���v�_�C�A���O
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);//���O��t���ĕۑ��̉�ʂ��o���֐�

    //�L�����Z�������璆�f
    if (selFile == FALSE) return;//�����L�����Z���������ꂽ��



    HANDLE hFile;              //�t�@�C���̃n���h��
    hFile = CreateFile(
    L"Stage.txt",              //�t�@�C����
    GENERIC_READ,              //�A�N�Z�X���[�h�i�ǂݍ��ݗp�jtext��ǂݍ���
    0,                         //���L�i�Ȃ��j
    NULL,                      //�Z�L�����e�B�����i�p�����Ȃ��j
    OPEN_EXISTING,             //�쐬���@
    FILE_ATTRIBUTE_NORMAL,     //�����ƃt���O�i�ݒ�Ȃ��j
    NULL);                     //�g�������i�Ȃ��j

    //�t�@�C���̃T�C�Y���擾
    DWORD fileSize = GetFileSize(hFile, NULL);//hFile�����o�C�g�Ȃ̂��𒲂ׂĂ����

    //�t�@�C���̃T�C�Y�����������m��
    char* MapData;//���݃|�C���^�ɂ��Ă��邽�ߐ擪�̕��������\������Ȃ�(�z��Ȃ猩���)
    MapData = new char[fileSize];

    DWORD dwBytes = 0; //�ǂݍ��݈ʒu�@����܊֌W�Ȃ����ǂȂ��ƃG���[���N����񂾂���

    ReadFile(
        hFile,     //�t�@�C���n���h��
        MapData,      //�f�[�^������ϐ�
        fileSize,  //�ǂݍ��ރT�C�Y
        &dwBytes,  //�ǂݍ��񂾃T�C�Y
        NULL);     //�I�[�o�[���b�v�h�\���́i����͎g��Ȃ��j

    CloseHandle(hFile);//�����������(�厖)

    //��������ǉ�
    string data(MapData);//Char��string�ɕϊ�

    size_t pos = 0;
    
    //�������ł͏���T��
    if (!data.empty())
    {
        for (int x = 0; x < Width; x++)
        {
            for (int z = 0; z < Height; z++)
            {
                //find��,�̈ʒu��T��
                size_t next = data.find(',', pos);//����pos����n�܂�R���}�̈ʒu��T��
              
                //������int�^�ɕϊ����Ȃ��Ɠ{����̂Łwstoi�x���g���ĕϊ�
                table[x][z].height = std::stoi(data.substr(pos,next - pos));//�wsubstr�x�œ���̕����̏ꏊ����肷��

                pos = next + 1;//����data�Ɍ�����

                next = data.find(',', pos);

                table[x][z].type = std::stoi(data.substr(pos, next - pos));

                pos = next + 1;

            }
        }
    }
 
}

//�E�B���h�E�v���V�[�W���̋U������
LRESULT Stage::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:

        PostQuitMessage(0);  //�v���O�����I��
        return 0;

    case WM_MOUSEMOVE:
        Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDC_MENU_NEW:
            break;

        case IDC_MENU_OPEN:
            Open();
            break;

        case IDC_MENU_SAVE:
            Save();
            break;
        }
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//�_�C�A���O�v���V�[�W�����ۂ��Ԃ̑��l
BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    
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
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_SETCURSEL, 0,0);
        break;

    case WM_COMMAND:

        switch (LOWORD(wp))
        {
        case IDC_RADIO_UP:
            SelectMode = 0;
            break;

        case IDC_RADIO_DOWN:
            SelectMode = 1;
            break;

        case IDC_RADIO_CHANGE:
            SelectMode = 2;
            break;
        case IDC_COMBO3:
            SelectType = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_GETCURSEL, 0, 0);
            break;
        }
    }

    return FALSE;
}

