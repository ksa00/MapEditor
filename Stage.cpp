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
	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++) {
			transform.position_.x = x;
			transform.position_.z = z;
			pFbx->Draw(transform);
		}

	}
}
	

void Stage::Release()
{
	pFbx->Release();
	SAFE_DELETE(pFbx);
}
