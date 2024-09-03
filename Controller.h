#pragma once
#include"Transform.h"

class Controller 
{
private:
    Transform transform;
public:
    //�R���X�g���N�^
    Controller();

    //�f�X�g���N�^
    ~Controller();

    //������
    void Initialize() ;

    //�X�V
    void Update() ;

    //�`��
    void Draw() ;

    //�J��
    void Release() ;
};