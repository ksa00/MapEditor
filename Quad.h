#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include <DirectXMath.h>
#include"Transform.h"
using namespace DirectX;
//�R���X�^���g�o�b�t�@�[

struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matW;

};
//XMFLOAT3�ł��悢�B
//���_���
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal;

};
class Quad
{protected:
	int vertexNum_;
	std::vector<VERTEX>vertices_;
	int indexNum_;
	std::vector<int>index_;
	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;   //�C���f�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;
public:
	Quad();
virtual ~Quad();
	HRESULT Initialize();

	 void Draw(Transform& transform);
	void Release();
//private:
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