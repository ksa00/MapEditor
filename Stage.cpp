#include "Stage.h"5
#include "resource.h"
#include"Input.h"
#include"Direct3D.h"

Stage::Stage()
{
	for (int i = 0; i < 5; i++) {
		pFbx [i] = nullptr;
	}
	for (int x = 0; x < 20; x++) {
		for (int z = 0; z < 20; z++) {
			table_[x][z].height = 1;
		}
	}
	table_[3][5].height = 0;
	table_[2][6].height = 5;
	table_[2][8].height = 4;
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
	RayCastData data;
	data.start = XMFLOAT4(0, 5, 0,0);
	data.dir = XMFLOAT4(0, -1, 0,0);
	pFbx[0]->RayCast(data);


	if (!Input::IsMouseButtonDown(0))
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
	}
}

void Stage::Draw()
{
	Transform transform;
	for (int x = 0; x < 20; x++) {
		for (int z = 0; z < 20; z++) {
			for (int y = 0; y < table_[x][z].height; y++) { // 高さ3段のループを追加
				transform.position_.x = x;
				transform.position_.y = y; // 高さに基づいてY座標を設定
				transform.position_.z = z;
				pFbx[0]->Draw(transform);
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