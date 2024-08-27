#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"


using std::vector;

//#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete[] p; p = nullptr;}



//�l�p�`�|���S���i�O�p�`���Q���j��`�悷��N���X
class Sprite
{
	//�R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;		//���[���h�s��
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;	//�ʒu
		XMVECTOR uv;		//UV
	};
protected:
	uint64_t vertexNum_;
	vector<VERTEX>vertices_;
	uint64_t indexNum_;
	vector<int>indices_;
	ID3D11Buffer* pIndexBuffer_;		//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

	Texture* pTexture_;		//�e�N�X�`��


public:
	Sprite();
	~Sprite();

	//�������i�|���S����\�����邽�߂̊e����������j
	//�ߒl�F�����^���s
	HRESULT Initialize();

	//�`��
	//�����Ftransform	�g�����X�t�H�[���I�u�W�F�N�g
	void Draw(Transform& transform);

	//���
	void Release();

private:
	//---------Initialize����Ă΂��֐�---------
	virtual void InitVertexData();		//���_���̏���
	HRESULT CreateVertexBuffer();		//���_�o�b�t�@���쐬

	virtual void InitIndexData();		//�C���f�b�N�X��������
	HRESULT CreateIndexBuffer();		//�C���f�b�N�X�o�b�t�@���쐬

	HRESULT CreateConstantBuffer();		//�R���X�^���g�o�b�t�@�쐬

	HRESULT LoadTexture();				//�e�N�X�`�������[�h


	//---------Draw�֐�����Ă΂��֐�---------
	void PassDataToCB(Transform& transform);	//�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline();

};