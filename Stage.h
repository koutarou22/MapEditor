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
public:
    //コンストラクタ
    Stage();

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize();

    //更新
    void Update();

    //描画
    void Draw();

    //開放
    void Release();
};

