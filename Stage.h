#pragma once
#include"Fbx.h"
namespace {
	const int MODEL_NUM{ 5 };
	const int XSIZE{ 15 };
	const int ZSIZE{ 15 };
	enum BLOCKTYPE
	{
		DEFAULT, BRICK, GRASS, SAND, WATER
	};
}
class Stage
{
	Fbx* pFbx[5];
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
	void SetBlock(int _x, int _z, BLOCKTYPE _type);
	void SetBlockHeight(int _x, int _z, int _height);
	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
};

