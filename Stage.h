#pragma once
#include"Fbx.h"
class Stage
{
	Fbx* pFbx;
	int mode_;      //0:上げる　1：下げる　2：種類を変える
	int select_;    //種類
public:
	Stage();
	~Stage();
	void Initialize();
	void Update();
	void Draw();
	void Release();
	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};

