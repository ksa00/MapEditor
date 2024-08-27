#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include <DirectXMath.h>
#include"Transform.h"
using namespace DirectX;
//コンスタントバッファー

struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matW;

};
//XMFLOAT3でもよい。
//頂点情報
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
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;   //インデクスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;
public:
	Quad();
virtual ~Quad();
	HRESULT Initialize();

	 void Draw(Transform& transform);
	void Release();
//private:
	//---------Initializeから呼ばれる関数---------
	virtual void InitVertexData();		//頂点情報の準備
	HRESULT CreateVertexBuffer();		//頂点バッファを作成

	virtual void InitIndexData();		//インデックス情報を準備
	HRESULT CreateIndexBuffer();		//インデックスバッファを作成

	HRESULT CreateConstantBuffer();		//コンスタントバッファ作成

	HRESULT LoadTexture();				//テクスチャをロード


	//---------Draw関数から呼ばれる関数---------
	void PassDataToCB(Transform& transform);	//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();

};