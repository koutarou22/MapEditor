#pragma once
#include"Transform.h"

class Controller
{   
    Transform transform;
  

public:
    //�R���X�g���N�^
    Controller();

    //�f�X�g���N�^
    ~Controller();

    //������
    void Initialize();

    //�X�V
    void Update();

    //void UpdateTarget();

    //�`��
    void Draw();

    //�J��
    void Release();
};

