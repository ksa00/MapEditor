#pragma once
#include"Fbx.h"
class Stage
{
	Fbx* pFbx;
	int mode_;      //0:�グ��@1�F������@2�F��ނ�ς���
	int select_;    //���
	int table_[20][20];
public:
	Stage();
	~Stage();
	void Initialize();
	void Update();
	void Draw();
	void Release();
	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};

