#include "Stage.h"

Stage::Stage()
{
	pFbx = nullptr;
}

Stage::~Stage()
{

}

void Stage::Initialize()
{
	pFbx = new Fbx;
	pFbx->Load("Asset/BoxDefault.fbx");
}

void Stage::Update()
{
}

void Stage::Draw()
{
	Transform transform;
	for (int i = 0; i < 20; i++)
	{
		transform.position_.x = i;
		pFbx->Draw(transform);
	}transform.position_.x = 0;
	for (int i = 0; i < 20; i++)
	{
		transform.position_.y = i;
		pFbx->Draw(transform);
	}
		
	}
	

void Stage::Release()
{
	pFbx->Release();
	SAFE_DELETE(pFbx);
}
