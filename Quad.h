//#pragma once
//#include <DirectXMath.h>
//#include "Direct3D.h"
//#include "Texture.h"
//#include "Transform.h"
//#include <vector>
//
//using namespace DirectX;
//
//struct CONSTANT_BUFFER
//{
//	XMMATRIX	matWVP;  
//	XMMATRIX	matW;
//};
//
//struct VERTEX
//{
//	XMVECTOR position;
//	XMVECTOR uv;
//	XMVECTOR normal;
//};
//
//class Quad
//{
//protected:
//
//	int vertexNum_;
//	std::vector<VERTEX>vertices_;
//	int indexNum_;
//	std::vector<int> index_;
//    ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
//	ID3D11Buffer* pIndexBuffer_;
//	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
//	Texture* pTexture_;
//public:
//
//	Quad();
//	virtual ~Quad();
//	HRESULT Initialize();
//	void Draw(Transform& transform);
//	void Release();
//
//private:
//	virtual  void InitVertexDate();
//	HRESULT CreateVertexDate();
//	virtual  void InitIndexDate();
//	HRESULT CreateIndexBuffer();
//	HRESULT CreateConstantBuffer();
//	HRESULT LoadTexture();
//
//	void PassDateToCB(Transform& transform);
//	void SetBufferToPipeline();
//
//};
//
