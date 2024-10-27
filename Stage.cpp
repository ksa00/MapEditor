#include "Stage.h"
#include "resource.h"
#include"Input.h"
#include"Direct3D.h"

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
	table_[_x][_z].type = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
	table_[_x][_z].height = _height;
}
Stage::Stage() : pFbx(), mode_(0), select_(0)
{
	for (int i = 0; i < FBX_NUM; i++) {
		pFbx[i] =new Fbx ;
	}
	for (int z = 0; z < Height; z++) {
		for (int x = 0; x < Width; x++) {
			SetBlock(x, z, DEFAULT);
		}
	}
}

Stage::~Stage()
{

}

void Stage::Initialize()
{
	string modelname[] = {
		"BoxDefault.fbx",
		"BoxBrick.fbx",
		"BoxGrass.fbx",
		"BoxSand.fbx",
		"BoxWater.fbx"
	};
	string fname_base = "Asset/";
	for (int i = 0; i < FBX_NUM; i++) {
		pFbx[i] ->Load(fname_base + modelname[i]);
		
	}
	for (int z = 0; z < Height; z++) {
		for (int x = 0; x < Width; x++) {
			SetBlock(x, z, (BLOCKTYPE)(0));
			SetBlockHeight(x, z,1);
		}
	}
	
}

void Stage::Update()
{

	if (!Input::IsMouseButtonDown(0))
	{
		return;
	}
		float w = (float)(Direct3D::scrWidth / 2.0f);
		float h = (float)(Direct3D::scrHeight / 2.0f);
		//Offsetx,y ‚Í0
		//minZ =0 maxZ = 1

		XMMATRIX vp =
		{
			 w,  0,  0, 0,
			 0, -h,  0, 0,
			 0,  0,  1, 0,
			 w,  h,  0, 1
		};
		//ƒrƒ…[ƒ|[ƒg
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		//ƒvƒƒWƒFƒNƒVƒ‡ƒ“•ÏŠ·
		XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
		//ƒrƒ…[•ÏŠ·
		XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
		XMFLOAT3 mousePosFront = Input::GetMousePosition();
		mousePosFront.z = 0.0f;
		XMFLOAT3 mousePosBack = Input::GetMousePosition();
		mousePosBack.z = 1.0f;
		//‡@@mousePosFront‚ðƒxƒNƒgƒ‹‚É•ÏŠ·
		XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
		//‡A@‡@‚ÉinvVPAinvPrjAinvView‚ð‚©‚¯‚é
		vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
		//‡B@mousePosBack‚ðƒxƒNƒgƒ‹‚É•ÏŠ·
		XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
		//‡C@‡B‚ÉinvVPAinvPrjAinvView‚ð‚©‚¯‚é
		vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

		int bufX = -1, bufZ; // bufX starts as -1 to indicate no block has been found
		float minDistance = 9999999; // To track the closest block hit
		
		for (int x = 0; x < Width; x++)
		{
			for (int z = 0; z < Height; z++)
			{
				for (int y = 0; y < table_[x][z].height; y++)
				{
					RayCastData data;
					XMStoreFloat3(&data.start, vMouseFront);
					XMStoreFloat3(&data.dir, vMouseBack - vMouseFront);
					Transform trans;
					trans.position_.x = x;
					trans.position_.y = y;
					trans.position_.z = z;

					pFbx[0]->RayCast(data, trans);
					if (data.hit == true) {
						if (minDistance > data.dist)
						{
							minDistance = data.dist;
							bufX = x;// Store the X coordinate of the closest block
							bufZ = z;// Store the Z coordinate of the closest block
						}
					}
				}
			}
		}
		if (bufX >= 0)
		{
			switch (mode_)
			{
			case 0:
				table_[bufX][bufZ].height++;
				break;
			case 1:
				if (table_[bufX][bufZ].height > 1)
				{
					table_[bufX][bufZ].height--;
				}
				break;
			case 2:
				table_[bufX][bufZ].type = select_;
				break;
			}
		}

	

}

void Stage::Draw()
{
	
	for (int x = 0; x < Width; x++)
	{
		for (int z = 0; z < Height; z++)
		{
			for (int y = 0; y < table_[x][z].height; y++)
			{
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;

				int type = table_[x][z].type;
				pFbx[type]->Draw(trans);

			}
		}
	}
}
	

void Stage::Release()
{
	for (int i = 0; i < 5; i++) {
		pFbx[i]->Release();
		SAFE_DELETE(pFbx[i]);
	}
}


BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"ƒfƒtƒHƒ‹ƒg");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"ƒŒƒ“ƒK");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"‘Œ´");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"»’n");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"…");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_SETCURSEL, 0, 0);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case IDC_RADIO_UP:
			mode_ = 0;
			return TRUE;

		case IDC_RADIO_DOWN:
			mode_ = 1;
			return TRUE;

		case IDC_RADIO_CHANGE:
			mode_ = 2;
			return TRUE;

		case IDC_COMBO2:
			select_ = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0);
			return TRUE;
		}
		return FALSE;

	}
	return FALSE;
}

void Stage::Save()
{
	char fileName[MAX_PATH] = "UNTITLED.map";  //ファイル名を入れる変数

	//「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
	ofn.lpstrFilter = TEXT("MapEditor(*.map)\0*.map\0")        //─┬ファイルの種類
		TEXT("TEXT DATA(*.txt*)\0*.txt*\0\0");   
	TEXT("ALL FILES(*.*)\0*.*\0\0");   //─┘
	ofn.lpstrFile = fileName;               	//ファイル名
	ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
	ofn.Flags = OFN_OVERWRITEPROMPT;   		//フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "map";                  	//デフォルト拡張子

	//「ファイルを保存」ダイアログ
	BOOL selFile;
	selFile = GetSaveFileName(&ofn);

	//キャンセルしたら中断
	if (selFile == FALSE) return;



	HANDLE hFile;
	hFile = CreateFile(
		fileName,    //ファイル名
		GENERIC_WRITE,  //アクセスモード
		0,
		NULL,
		CREATE_ALWAYS,     //作成方法
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	std::string data = "";



	//data.length()


	DWORD bytes = 0;
	WriteFile(
		hFile,              //ファイルハンドル
		"ABCDEF",          //保存したい文字列
		12,                  //保存する文字数
		&bytes,             //保存したサイズ
		NULL
	);



	CloseHandle(hFile);

}

void Stage::Open()
{
	char fileName[MAX_PATH] = "UNTITLED.map";  //ファイル名を入れる変数

	//「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
	ofn.lpstrFilter = TEXT("MapEditor(*.map)\0*.map\0")        //─┬ファイルの種類
		TEXT("TEXT DATA(*.txt*)\0*.txt*\0\0");
	TEXT("ALL FILES(*.*)\0*.*\0\0");   //─┘
	ofn.lpstrFile = fileName;               	//ファイル名
	ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
	ofn.Flags = OFN_FILEMUSTEXIST;   		//フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "map";                  	//デフォルト拡張子

	//「ファイルを保存」ダイアログ
	BOOL selFile;
	selFile = GetOpenFileName(&ofn);

	//キャンセルしたら中断
	if (selFile == FALSE) return;



	HANDLE hFile;
	hFile = CreateFile(
		fileName,    //ファイル名
		GENERIC_READ,  //アクセスモード
		0,
		NULL,
		OPEN_EXISTING,     //作成方法
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	//ファイルのサイズを取得
	DWORD fileSize = GetFileSize(hFile, NULL);

	//ファイルのサイズ分メモリを確保
	char* data;
	data = new char[fileSize];

	DWORD dwBytes = 0; //読み込み位置

	ReadFile(
		hFile,     //ファイルハンドル
		data,      //データを入れる変数
		fileSize,  //読み込むサイズ
		&dwBytes,  //読み込んだサイズ
		NULL);     //オーバーラップド構造体（今回は使わない）
	
	CloseHandle(hFile);

}