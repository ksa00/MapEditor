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
		//Offsetx,y は0
		//minZ =0 maxZ = 1

		XMMATRIX vp =
		{
			 w,  0,  0, 0,
			 0, -h,  0, 0,
			 0,  0,  1, 0,
			 w,  h,  0, 1
		};
		//ビューポート
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		//プロジェクション変換
		XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
		//ビュー変換
		XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
		XMFLOAT3 mousePosFront = Input::GetMousePosition();
		mousePosFront.z = 0.0f;
		XMFLOAT3 mousePosBack = Input::GetMousePosition();
		mousePosBack.z = 1.0f;
		//①　mousePosFrontをベクトルに変換
		XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
		//②　①にinvVP、invPrj、invViewをかける
		vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
		//③　mousePosBackをベクトルに変換
		XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
		//④　③にinvVP、invPrj、invViewをかける
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