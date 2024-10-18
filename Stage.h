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

    int SelectMode;//上げるのか下げるのか変えるのか？
    int SelectType;//デフォルトなのか？草原なのか
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

    BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};

