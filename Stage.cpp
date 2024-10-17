#include "Stage.h"5
#include "resource.h"
#include"Input.h"
#include"Direct3D.h"

Stage::Stage() : pFbx(), Width(20), Height(20)
{
	for (int x = 0; x < Width; x++)
	{
		for (int z = 0; z < Height; z++)
		{
			table_[x][z].height = 1;
			table_[x][z].type = 0;
		}
	}
	table_[0][0].height = 5;
	table_[3][3].height = 2;
	table_[10][1].height = 3;

	table_[0][0].type = 1;
	table_[3][3].type = 2;
	table_[10][1].type = 3;
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
	for (int i = 0; i < 5; i++) {
		pFbx[i] = new Fbx;
		pFbx[i] ->Load(fname_base + modelname[i]);
		
	}

	
	
}

void Stage::Update()
{

	if (Input::IsMouseButtonDown(0))
	{

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


		RayCastData data;
		XMStoreFloat3(&data.start, vMouseFront);
		XMStoreFloat3(&data.dir, vMouseBack - vMouseFront);
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
					pFbx[type]->RayCast(data, trans);
					if (data.hit == true) {
						table_[x][z].height++;
						return;
					}
				}
			}
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