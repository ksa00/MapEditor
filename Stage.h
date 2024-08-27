#pragma once
#include"Fbx.h"
class Stage
{
	Fbx* pFbx;
public:
	Stage();
	~Stage();
	void Initialize();
	void Update();
	void Draw();
	void Release();

};

