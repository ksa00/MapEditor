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
	int mode_;      //0:上げる　1：下げる　2：種類を変える
	int select_;    //種類
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
