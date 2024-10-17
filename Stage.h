#pragma once
#include"Fbx.h"
class Stage
{
	Fbx* pFbx[5];
	int Width;
	int Height;
	int mode_;      //0:上げる　1：下げる　2：種類を変える
	int select_;    //種類
	struct 
	{
		int type;
		int height;
	}table_[20][20];
	 
public:
	Stage();
	~Stage();
	void Initialize();
	void Update();
	void Draw();
	void Release();
	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};

