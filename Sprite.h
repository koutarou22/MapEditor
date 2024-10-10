#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

using std::vector;
using namespace DirectX;



class Sprite
{
	//コンスタントバッファー:　アプリ側から、シェーダーに毎フレーム渡したい情報
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
	};

	//XMFLOAT3でもよい。
	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};

	uint64_t vertexNum_;			//頂点数
	vector<VERTEX> vertices_;		//頂点情報
	uint64_t indexNum_;				//インデックス数
	vector<int> indices_;			//インデックス情報

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;    //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_; //テクスチャへのポインタ
public:
	Sprite();//コンストラクタ
	~Sprite();//デストラクタ
	HRESULT Load(std::string fileName);//初期化用（コンストラクタでできない奴はこっちで初期化）
	void Draw(Transform& transform);//描画関数
	void Release();//解放処理
private:
	//---------Initializeから呼ばれる関数---------
	void InitVertexData();			//頂点情報の準備
	HRESULT CreateVertexBuffer();	//頂点バッファを作成

	void InitIndexData();			//インデックス情報を準備
	HRESULT CreateIndexBuffer();	//インデックスバッファを作成

	HRESULT CreateConstantBuffer();	//コンスタントバッファ作成

	HRESULT LoadTexture(std::string fileName);			//テクスチャをロード
	//---------Draw関数から呼ばれる関数---------
	void PassDataToCB(DirectX::XMMATRIX worldMatrix);	//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();							//各バッファをパイプラインにセット
};