#pragma once
#include <DirectXMath.h>

using namespace DirectX;

//�ʒu�A�����A�g�嗦�Ȃǂ��Ǘ�����N���X
class Transform
{
	XMMATRIX matTranslate_;	//�ړ��s��
	XMMATRIX matRotate_;	//��]�s��	
	XMMATRIX matScale_;	//�g��s��

public:
	XMFLOAT3 position_;	//�ʒu
	XMFLOAT3 rotate_;	//����
	XMFLOAT3 scale_;	//�g�嗦
	Transform* pParent_;	//�e�I�u�W�F�N�g�̃g�����X�t�H�[��

	//�R���X�g���N�^
	Transform();

	//�f�X�g���N�^
	~Transform();

	//�e�s��̌v�Z
	void Calculation();

	//���[���h�s����擾
	XMMATRIX GetWorldMatrix();

	//�@���ό`�p�s����擾
	XMMATRIX GetNormalMatrix();
};