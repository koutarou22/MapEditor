#pragma once
#include "Direct3D.h"
#include <fbxsdk.h>
#include <DirectXMath.h>
#include "Texture.h"
#include "Transform.h"
#include <vector>
#include <filesystem>
#include "Texture.h"

//#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

struct RayCastDeta
{
	XMFLOAT4 start;
	XMFLOAT4 dir;
	bool hit;
	float dist;
};

class FBX
{
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP; //ワールドビュープロジェクション
		XMMATRIX	matNormal;   //法線ベクトルの変換用
		XMFLOAT4    diffuseColor;
		int isTextured;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //ノーマル追加（法線ベクトル）
	};

	//std::vector<VERTEX> vertices(vertexCount_);
	std::vector<VERTEX> vertices;
	std::vector<int>* index;

	ID3D11Buffer* pVertexBuffer_; //頂点バッファ用メモリ 
	ID3D11Buffer** pIndexBuffer_; //インデックスバッファ用メモリ
	ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ用メモリ
	std::vector<MATERIAL> pMaterialList_; //マテリアルの配列
	std::vector<int> indexCount_;//マテリアルごとのインデックス数

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_; //マテリアル数

	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
public:
	FBX();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();
	void    RayCast(RayCastDeta& rayDate);
};