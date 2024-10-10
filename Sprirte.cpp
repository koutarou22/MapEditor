#include "Sprite.h"
#include "Camera.h"
#include <filesystem>



Sprite::Sprite()
	:pTexture_(nullptr), pVertexBuffer_(nullptr),
	pIndexBuffer_(nullptr), pConstantBuffer_(nullptr),
	vertexNum_(0), indexNum_(0)
{
}

Sprite::~Sprite()
{
	Release();
}

HRESULT Sprite::Load(std::string fileName)
{


	//頂点情報
	InitVertexData();//データを用意して
	if (FAILED(CreateVertexBuffer()))//頂点バッファ作成
	{
		return E_FAIL;
	}
	//インデックス情報
	InitIndexData();					//データを用意して
	if (FAILED(CreateIndexBuffer()))	//インデックスバッファ作成
	{
		return E_FAIL;
	}

	//コンスタントバッファ作成
	if (FAILED(CreateConstantBuffer()))//コンスタントバッファ作成
	{
		return E_FAIL;
	}

	//テクスチャのロード
	if (FAILED(LoadTexture(fileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}


void Sprite::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_TYPE::SHADER_2D);
	//ワールドマトリクスを計算
	//transform.Calculation();

	//XMMATRIX fixRateM = XMMatrixScaling(1.0 / Direct3D::WINDOW_WIDTH, 1.0 / Direct3D::WINDOW_HEIGHT, 1.0);
	//transform.position_ = { transform.position_.x / Direct3D::WINDOW_WIDTH, transform.position_.y / Direct3D::WINDOW_HEIGHT, 0.0 };
	transform.Calculation();

	//コンスタントバッファに情報を渡す
	PassDataToCB(transform.GetWorldMatrix());
	//頂点バッファ、インデックスバッファ、コンスタントバッファをパイプラインにセット
	SetBufferToPipeline();
	//描画
	Direct3D::pContext->DrawIndexed(indexNum_, 0, 0);
}

void Sprite::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Sprite::InitVertexData()
{
	// 頂点情報
	vertices_ =
	{
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0, 0.0, 0.0, 0.0)}, 	// 四角形の頂点（左上）, UV
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0, 0.0, 0.0, 0.0)},	// 四角形の頂点（右上）, UV
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),  XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },// 四角形の頂点（右下）
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },	// 四角形の頂点（左下）		

	};
	vertexNum_ = vertices_.size();
}

HRESULT Sprite::CreateVertexBuffer()
{
	HRESULT hr;
	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data(); //配列のアドレスをゲットして代入
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	//hr = E_FAIL;
	if (FAILED(hr))
	{
		//int MessageBox(
		//	[in, optional] HWND    hWnd,
		//	[in, optional] LPCTSTR lpText,
		//	[in, optional] LPCTSTR lpCaption,
		//	[in]           UINT    uType
		//);
		MessageBox(NULL, L"頂点バッファの作成に失敗", NULL, MB_OK);
		return hr;
	}

	return S_OK;
}

void Sprite::InitIndexData()
{
	//indices_ = { 0,1,2, 1,3,2 };

	indices_ = { 0,2,3, 0,1,2 };
	//インデックス数
	indexNum_ = indices_.size();
}

HRESULT Sprite::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_; //自分のクラスに合わせる
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices_.data(); //自分のクラスに合わせる
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Sprite::CreateConstantBuffer()
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"コンスタントバッファの作成に失敗", NULL, MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Sprite::LoadTexture(std::string fileName)
{
	namespace fs = std::filesystem;

	pTexture_ = new Texture;
	assert(fs::is_regular_file(fileName));


	HRESULT hr;
	hr = pTexture_->Load(fileName);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"テクスチャの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

void Sprite::PassDataToCB(DirectX::XMMATRIX worldMatrix)
{
	CONSTANT_BUFFER cb;
	cb.matW = XMMatrixTranspose(worldMatrix); //MATRIXの掛け算のやり方がDirectXと違うので転置をとる（なんそれ）

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	//サンプラーとシェーダーリソースビューをシェーダにセット
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

}

void Sprite::SetBufferToPipeline()
{
	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用


}