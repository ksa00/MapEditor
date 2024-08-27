#include "Dice.h"
#include"Camera.h"

Dice::~Dice()
{
} 


//���_���̏���

void Dice::InitVertexData()
{
	vertices_ =
	{
		//front
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i����j
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f),	XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�E��j
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f),	XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�E���j
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f),XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f) },   // �l�p�`�̒��_�i�����j
		//left
		{ XMVectorSet(-1.0f,  1.0f, 1.0f, 0.0f),XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i����j
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f),XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i�E��j
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f),XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},   // �l�p�`�̒��_�i�E���j
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f),XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f), XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f)},   // �l�p�`�̒��_�i�����j
		//back
		{ XMVectorSet(1.0f,  1.0f, 1.0f, 0.0f),XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) },   // �l�p�`�̒��_�i����j
		{ XMVectorSet(-1.0f,  1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) },   // �l�p�`�̒��_�i�E��j
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) },   // �l�p�`�̒��_�i�E���j
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f),XMVectorSet(0.25f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f,1.0f, 0.0f) },   // �l�p�`�̒��_�i�����j
		//right
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f),XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)  },   // �l�p�`�̒��_�i����j
		{ XMVectorSet(1.0f,  1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f, 0.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)  },   // �l�p�`�̒��_�i�E��j
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f),  XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)  },   // �l�p�`�̒��_�i�E���j
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f),XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)  },   // �l�p�`�̒��_�i�����j
		//top
		{ XMVectorSet(-1.0f,  1.0f, 1.0f, 0.0f),XMVectorSet(0.75f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)  },   // �l�p�`�̒��_�i����j
		{ XMVectorSet(1.0f,  1.0f, 1.0f, 0.0f),	XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)  },   // �l�p�`�̒��_�i�E��j
		{ XMVectorSet(1.0f,  1.0f, -1.0f, 0.0f),	XMVectorSet(1.0f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)  },   // �l�p�`�̒��_�i�E���j
		{ XMVectorSet(-1.0f,  1.0f, -1.0f, 0.0f),XMVectorSet(0.75f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)  },   // �l�p�`�̒��_�i�����j
		//bottom
		{ XMVectorSet(-1.0f, -1.0f, 1.0f, 0.0f),XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f), XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i����j
		{ XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f),	XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f),XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i�E��j
		{ XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f),	XMVectorSet(0.25f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i�E���j
		{ XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f) },   // �l�p�`�̒��_�i�����j
	};
	vertexNum_ = vertices_.size();
}
//�C���f�b�N�X��������

void Dice::InitIndexData()
{
	index_ = { 
		0,2,3,
		0,1,2,
		4,6,7,
		4,5,6,
		8,10,11,
		8,9,10,
		12,14,15,
		12,13,14,
		16,18,19,
		16,17,18,
		22,20,23,
		22,21,20 };

	//�C���f�b�N�X��
	indexNum_ = index_.size();
}
