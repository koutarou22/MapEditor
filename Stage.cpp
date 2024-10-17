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

    ////あくまでFBXのレイキャストなので
    //pFbx[0]->RayCast(data, trans);

    if (Input::IsMouseButtonDown(0))
    {
        float w = Direct3D::WINDOW_WIDTH / 2;
        float h = Direct3D::WINDOW_HEIGHT / 2;
        //Offsetx,y は0
        //minZ =0 maxZ = 1

        XMMATRIX vp =
        {
             w,  0,  0, 0,
             0, -h,  0, 0,
             0,  0,  1, 0,
             w,  h,  0, 1
        };

        //レイを飛ばす処理
        XMMATRIX matView = Camera::GetViewMatrix();//ビュー行列
        XMMATRIX matProj = Camera::GetProjectionMatrix();//プロジェクション行列

        //ビューポート
        XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
        //プロジェクション変換
        XMMATRIX invProj = XMMatrixInverse(nullptr, matProj);
        //ビュー変換
        XMMATRIX invView = XMMatrixInverse(nullptr, matView);


        XMFLOAT3 mousePosBack = Input::GetMousePosition();
        mousePosBack.z = 0.0f;
        XMFLOAT3 mousePosFront = Input::GetMousePosition();
        mousePosFront.z = 1.0f;
        //①　mousePosFrontをベクトルに変換
        XMVECTOR vMouseFront = XMLoadFloat3(&mousePosBack); // カメラから見える範囲で一番後ろの面を取得
         
        //ここで変換する(前) 
        // ①にinvVP、invPrj、invViewをかける
        vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
        //③　mousePosBackをベクトルに変換
        XMVECTOR vMouseBack = XMLoadFloat3(&mousePosFront);
        //④　③にinvVP、invPrj、invViewをかける
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
                        return;//ごまかし
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

//ダイアログプロシージャっぽい赤の他人
BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    int a = 0;
    switch (msg)//何か処理をしたのならTRUEを返す
    {
    case WM_INITDIALOG:
        /*HWND h = GetDlgItem(hDlg, IDC_RADIO_UP);*/
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);

        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"デフォルト");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"レンガ");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"草");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"砂");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"水");
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