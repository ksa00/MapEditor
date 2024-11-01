#include "Stage.h"
#include "resource.h"
#include"Input.h"
#include"Direct3D.h"
#include <string>
#include <sstream>
#include<iostream>

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
	table_[_x][_z].type = _type;
	unsavedChanges = true; // Mark as changed
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
	table_[_x][_z].height = _height;
	unsavedChanges = true; // Mark as changed
}
Stage::Stage() : pFbx(), mode_(0), select_(0)
{
	for (int i = 0; i < FBX_NUM; i++) {
		pFbx[i] = new Fbx;
	}
	for (int z = 0; z < Height; z++) {
		for (int x = 0; x < Width; x++) {
			SetBlock(x, z, DEFAULT);
		}
	}
	unsavedChanges = false;

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
		pFbx[i]->Load(fname_base + modelname[i]);

	}
	for (int z = 0; z < Height; z++) {
		for (int x = 0; x < Width; x++) {
			SetBlock(x, z, (BLOCKTYPE)(0));
			SetBlockHeight(x, z, 1);
		}
	}
	unsavedChanges = false;

}

void Stage::Update()
{
	if (!Input::IsMouseButtonDown(0)) return;
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
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"デフォルト");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"レンガ");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"草原");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"砂地");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"水");
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

void Stage::NewFile()
{

	if (unsavedChanges) {
		if (MessageBox(NULL, TEXT("You have unsaved changes. Do you want to save them?"), TEXT("Unsaved Changes"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
			Save(); // Save the current file
		}
	}

	// Reset the stage data
	for (int z = 0; z < Height; z++) {
		for (int x = 0; x < Width; x++) {
			SetBlock(x, z, DEFAULT); // Reset the block type
			SetBlockHeight(x, z, 1); // Reset the block height (or any default height you prefer)
		}
	}

	// Optionally reset other state variables
	mode_ = 0; // Reset mode to default
	select_ = 0; // Reset selected block type
	unsavedChanges = false; // No changes in the new file

}
void Stage::Save()
{
	char fileName[MAX_PATH] = "UNTITLED.csv";  //ファイル名を入れる変数

	//「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
	ofn.lpstrFilter =
		TEXT("CSV Files(*.csv)\0*.csv\0\0")
		TEXT("MapEditor(*.map)\0*.map\0")        //─┬ファイルの種類
		TEXT("TEXT DATA(*.txt*)\0*.txt*\0\0")
		TEXT("ALL FILES(*.*)\0*.*\0\0");   //─┘
	ofn.lpstrFile = fileName;               	//ファイル名
	ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
	ofn.Flags = OFN_OVERWRITEPROMPT;   		//フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "csv";                  	//デフォルト拡張子

	//「ファイルを保存」ダイアログ
	if (!GetSaveFileName(&ofn)) return;

	//やり方1と2に必須
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
	if (hFile == INVALID_HANDLE_VALUE) return;

	//やり方1
	//// Write fixed dimensions (Width and Height)
	//int fixedWidth = 20;
	//int fixedHeight = 20;
	//DWORD bytes = 0;
	////WriteFile(
	////	hFile,              //ファイルハンドル
	////	"ABCDEF",          //保存したい文字列
	////	12,                  //保存する文字数
	////	&bytes,             //保存したサイズ
	////	NULL
	////);
	////幅と高さの書き込み
	//WriteFile(hFile, &fixedWidth, sizeof(int), &bytes, NULL);
	//WriteFile(hFile, &fixedHeight, sizeof(int), &bytes, NULL);
	//
	//for (int x = 0; x < Width; x++) {
	//	for (int z = 0; z < Height; z++) {
	//		WriteFile(hFile, &table_[x][z], sizeof(table_[x][z]), &bytes, NULL);
	//	}
	//}

	// やり方2
	DWORD bytesWritten;
	// Write dimensions first
	std::string header = "Type,Height\n";
	WriteFile(hFile, header.c_str(), header.size(), &bytesWritten, NULL);

	for (int x = 0; x < Width; x++) {
		for (int z = 0; z < Height; z++) {
			std::string line = std::to_string(table_[x][z].type) + "," + std::to_string(table_[x][z].height) + "\n";
			WriteFile(hFile, line.c_str(), line.size(), &bytesWritten, NULL);
		}
	}
	CloseHandle(hFile);
	unsavedChanges = false; // No changes in the new file



}

void Stage::Open()
{
	if (unsavedChanges) {
		if (MessageBox(NULL, TEXT("You have unsaved changes. Do you want to save them?"), TEXT("Unsaved Changes"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
			Save(); // Save the current file
		}
	}
	char fileName[MAX_PATH] = "UNTITLED.csv";  //ファイル名を入れる変数

	//「ファイルを保存」ダイアログの設定
	OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
	ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
	ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
	ofn.lpstrFilter = TEXT("CSV Files(*.csv)\0*.csv\0\0")
		TEXT("MapEditor(*.map)\0*.map\0")        //─┬ファイルの種類
		TEXT("TEXT DATA(*.txt*)\0*.txt*\0\0")
		TEXT("ALL FILES(*.*)\0*.*\0\0");   //─┘
	ofn.lpstrFile = fileName;               	//ファイル名
	ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
	ofn.Flags = OFN_FILEMUSTEXIST;   		//フラグ（同名ファイルが存在したら上書き確認）
	ofn.lpstrDefExt = "csv";                  	//デフォルト拡張子

	//「ファイルを保存」ダイアログ
	//BOOL selFile;
//	selFile = GetOpenFileName(&ofn);
	//キャンセルしたら中断
	//if (selFile == FALSE) return;
	if (!GetOpenFileName(&ofn)) return;

	//やり方1と2の共通部分
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

	//やり方1
	////ファイルのサイズを取得
	//DWORD fileSize = GetFileSize(hFile, NULL);
	//// Temporary variables for dimensions
	//int fileWidth;
	//int fileHeight;
	//DWORD bytes = 0; //読み込み位置
	////幅と高さ読み込み
	//ReadFile(hFile, &fileWidth, sizeof(int), &bytes, NULL);
	//ReadFile(hFile, &fileHeight, sizeof(int), &bytes, NULL);
	////ReadFile(
	////	hFile,     //ファイルハンドル
	////	data,      //データを入れる変数
	////	fileSize,  //読み込むサイズ
	////	&dwBytes,  //読み込んだサイズ
	////	NULL);     //オーバーラップド構造体（今回は使わない）
	// // Read the block data
	//for (int x = 0; x < Width; x++) {
	//	for (int z = 0; z < Height; z++) {
	//		ReadFile(hFile, &table_[x][z], sizeof(table_[0][0]), &bytes,NULL);
	//	}
	//}
	//CloseHandle(hFile);

//	やり方②
	if (hFile == INVALID_HANDLE_VALUE) return;
	DWORD fileSize = GetFileSize(hFile, NULL);
	char* data = new char[fileSize + 1];
	DWORD bytesRead;
	ReadFile(hFile, data, fileSize, &bytesRead,NULL);
	data[fileSize] = '\0'; // Null-terminate the string

	// Parse the CSV data
	std::istringstream ss(data);
	std::string line;
	// Read the header line
	std::getline(ss, line); // Skip the header
	// Read the blocks
	for (int x = 0; x < Width; x++) {
		for (int z = 0; z < Height; z++) {
			if (!std::getline(ss, line)) break; // Check for end of data
			std::istringstream lineStream(line);
			std::string typeStr, heightStr;
			std::getline(lineStream, typeStr, ',');
			std::getline(lineStream, heightStr, ',');
			table_[x][z].type = static_cast<BLOCKTYPE>(std::stoi(typeStr));
			table_[x][z].height = std::stoi(heightStr);
		}
	}
	delete[] data;

	CloseHandle(hFile);

}
