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
		//Offsetx,y ��0
		//minZ =0 maxZ = 1

		XMMATRIX vp =
		{
			 w,  0,  0, 0,
			 0, -h,  0, 0,
			 0,  0,  1, 0,
			 w,  h,  0, 1
		};
		//�r���[�|�[�g
		XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
		//�v���W�F�N�V�����ϊ�
		XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
		//�r���[�ϊ�
		XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
		XMFLOAT3 mousePosFront = Input::GetMousePosition();
		mousePosFront.z = 0.0f;
		XMFLOAT3 mousePosBack = Input::GetMousePosition();
		mousePosBack.z = 1.0f;
		//�@�@mousePosFront���x�N�g���ɕϊ�
		XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
		//�A�@�@��invVP�AinvPrj�AinvView��������
		vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
		//�B�@mousePosBack���x�N�g���ɕϊ�
		XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
		//�C�@�B��invVP�AinvPrj�AinvView��������
		vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);
	}
}

void Stage::Draw()
{
	Transform transform;
	for (int x = 0; x < 20; x++) {
		for (int z = 0; z < 20; z++) {
			for (int y = 0; y < table_[x][z].height; y++) { // ����3�i�̃��[�v��ǉ�
				transform.position_.x = x;
				transform.position_.y = y; // �����Ɋ�Â���Y���W��ݒ�
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
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�f�t�H���g");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"�����K");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"����");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"���n");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)"��");
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