#pragma once
#include"Transform.h"

class Controller
{   
    Transform transform;
  

public:
    //コンストラクタ
    Controller();

    //デストラクタ
    ~Controller();

    //初期化
    void Initialize();

    //更新
    void Update();

    //void UpdateTarget();

    //描画
    void Draw();

    //開放
    void Release();
};

