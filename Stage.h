#pragma once
#include"Fbx.h"
namespace {
	const int FBX_NUM{ 5 };
	const int Width{ 20 };
	const int Height{ 20 };
	enum BLOCKTYPE
	{
		DEFAULT, BRICK, GRASS, SAND, WATER
	};
}
class Stage
{
	Fbx* pFbx[FBX_NUM];
	int mode_;      //0:�グ��@1�F������@2�F��ނ�ς���
	int select_;    //���
	struct 
	{
		int type;
		int height;
	}table_[Width][Height];
	 
public:
void SetBlock(int _x, int _z, BLOCKTYPE _type);
	void SetBlockHeight(int _x, int _z, int _height);
	Stage();
	~Stage();
	void Initialize();
	void Update();
	void Draw();
	void Release();
	
	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
		void Save();
		void Open();

};
