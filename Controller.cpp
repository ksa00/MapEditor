#include "Controller.h"
#include "Input.h"
#include "Camera.h"

Controller::Controller()
{
    transform.position_ = XMFLOAT3(7, 0, 7);
   
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
  
}

void Controller::Update()
{

 
  //  XMVECTOR front = XMVectorSet(0, 0, 0.1f, 0);                
    if (Input::IsKey(DIK_W))
    {
       
        transform.position_.z += 0.01f;
    }
    if (Input::IsKey(DIK_S))
    {
        transform.position_.z -= 0.01f;
    }


   // XMVECTOR right = XMVectorSet(0.1f, 0, 0, 0);              

    if (Input::IsKey(DIK_D))
    {
        transform.position_.x += 0.01f;
    }
    if (Input::IsKey(DIK_A))
    {
        transform.position_.x -= 0.01f;
    }
    
  XMVECTOR Pos = XMLoadFloat3(&transform.position_);

    //ÉJÉÅÉâ
    XMVECTOR Cam = XMVectorSet(0, 7, -10, 0);    
    XMVECTOR cameraPosition =Pos + Cam;
    Camera::SetPosition(cameraPosition);          
    Camera::SetTarget(transform.position_);
}

void Controller::Draw()
{
}

void Controller::Release()
{
}
