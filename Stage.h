#pragma once
#include"Fbx.h"
class Stage
{
	Fbx* pFbx[5];
	int Width;
	int Height;
	int mode_;      //0:�グ��@1�F������@2�F��ނ�ς���
	int select_;    //���
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

