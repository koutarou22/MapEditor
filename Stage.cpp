#include "Stage.h"
#include"Input.h"
#include"Camera.h"
#include"Direct3D.h"

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
    //table[0][0].height = 5;
    //table[3][3].height = 2;
    //table[10][1].height = 3;

    //table[0][0].type = 1;
    //table[3][3].type = 2;
    //table[10][1].type = 3;
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
    RayCastDeta data;
    data.start = XMFLOAT4(-5, 5, 0, 0);
    data.dir = XMFLOAT4(0, -1, 0, 0);
    pFbx[0]->RayCast(data);


    if (Input::IsMouseButtonDown(0))
    {
        //レイを飛ばす処理
        XMMATRIX matView = Camera::GetViewMatrix();//ビュー行列
        XMMATRIX matProj = Camera::GetProjectionMatrix();//プロジェクション行列

        float w = Direct3D::WINDOW_WIDTH / 2;
        float h = Direct3D::WINDOW_HEIGHT / 2;
        XMMATRIX vp =//ビュー行列
        {
            w, 0, 0, 0,
            0,-h, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        XMMATRIX invView = XMMatrixInverse(nullptr, matView);
        XMMATRIX invProj = XMMatrixInverse(nullptr, matProj);
        XMMATRIX invVP = XMMatrixInverse(nullptr, vp);

        //////////////////////////////////////////////
        XMFLOAT3 mousePos = Input::GetMousePosition();
        mousePos.z = 0;

        XMVECTOR mouseFrontPos = XMLoadFloat3(&mousePos);
        //////////////////////////////////////////////

        mousePos.z = 1;

        XMVECTOR mouseBackPos = XMLoadFloat3(&mousePos);//カメラから見える範囲で一番後ろの面を取得
        //ここで変換する(前)
        mouseFrontPos = XMVector3TransformCoord(mouseFrontPos, invVP * invProj * invView);

        //ここで行列をする(後)
        mouseBackPos = XMVector3TransformCoord(mouseBackPos, invVP * invProj * invView);
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
