//#include "Quad.h"
//#include "Camera.h"
//using namespace Direct3D;
//
//HRESULT hr;
//Quad::Quad()
//	:pTexture_(nullptr), pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr),
//	vertexNum_(0), indexNum_(0)
//{
//}
//
//Quad::~Quad()
//{
//	Release();
//}
//
//HRESULT Quad::Initialize()
//{
//	InitVertexDate();
//	if (FAILED(CreateVertexDate()))
//	{
//		return E_FAIL;
//	}
//
//	InitIndexDate();
//	if (FAILED(CreateIndexBuffer()))
//	{
//		return E_FAIL;
//	}
//
//	if (FAILED(CreateConstantBuffer()))
//	{
//		return E_FAIL;
//	}
//
//	if (FAILED(LoadTexture()))
//	{
//		return E_FAIL;
//	}
//
//	return S_OK;
//
//}
//
//void Quad::Draw(Transform& transform)
//{
//
//	Direct3D::SetShader(SHADER_3D);
//	transform.Calculation();
//
//	PassDateToCB(transform);
//
//	SetBufferToPipeline();
//
//	Direct3D::pContext->DrawIndexed(index_.size(), 0, 0);
//}
//
//void Quad::Release()
//{
//	SAFE_RELEASE(pConstantBuffer_);
//	SAFE_RELEASE(pIndexBuffer_);
//	SAFE_RELEASE(pVertexBuffer_);
//}
//
//void Quad::InitVertexDate()
//{
//	vertices_ =
//	{
//		{{ -1.0f,  1.0f, 0.0f,0.0f },  XMVectorSet(0.0, 0.0, 0.0, 0.0) },
//		{{  1.0f,  1.0f, 0.0f ,0.0f},  XMVectorSet(1.0, 0.0, 0.0, 0.0) },
//		{{ 1.0f, -1.0f, 0.0f ,0.0f},  XMVectorSet(1.0, 1.0, 0.0, 0.0) },
//		{{  -1.0f, -1.0f, 0.0f ,0.0f},  XMVectorSet(0.0, 1.0, 0.0, 0.0)},
//	};
//	vertexNum_ = vertices_.size();
//}
//
//HRESULT Quad::CreateVertexDate()
//{
//	D3D11_BUFFER_DESC bd_vertex;
//	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
//	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
//	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd_vertex.CPUAccessFlags = 0;
//	bd_vertex.MiscFlags = 0;
//	bd_vertex.StructureByteStride = 0;
//	D3D11_SUBRESOURCE_DATA data_vertex;
//	data_vertex.pSysMem = vertices_.data();
//	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
//	if (FAILED(hr))
//	{
//		MessageBox(NULL, L"頂点バッファの作成に失敗", L"Error", MB_OK);
//		return hr;
//	}
//	return S_OK;
//}
//
//void Quad::InitIndexDate()
//{
//	index_ = { 0,2,3,0,1,2 };
//	indexNum_ = index_.size();
//}
//
//HRESULT Quad::CreateIndexBuffer()
//{
//	HRESULT hr;
//
//	D3D11_BUFFER_DESC  bd;
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(int) * indexNum_;
//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	bd.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA InitDate;
//	InitDate.pSysMem = index_.data();
//	InitDate.SysMemPitch = 0;
//	InitDate.SysMemSlicePitch = 0;
//
//
//	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitDate, &pIndexBuffer_);
//
//	if (FAILED(hr))
//	{
//		MessageBox(NULL, L"インデックスバッファの作成に失敗", L"Error", MB_OK);
//		return hr;
//	}
//	return S_OK;
//
//}
//
//HRESULT Quad::CreateConstantBuffer()
//{
//	HRESULT hr;
//
//	D3D11_BUFFER_DESC  cd;
//	cd.ByteWidth = sizeof(CONSTANT_BUFFER);
//	cd.Usage = D3D11_USAGE_DYNAMIC;
//	cd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	cd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	cd.MiscFlags = 0;
//	cd.StructureByteStride = 0;
//
//	hr = Direct3D::pDevice->CreateBuffer(&cd, nullptr, &pConstantBuffer_);
//
//	if (FAILED(hr))
//	{
//		MessageBox(NULL, L"インデックスバッファの作成に失敗", L"Error", MB_OK);
//		return hr;
//	}
//	return S_OK;
//}
//
//HRESULT Quad::LoadTexture()
//{
//	pTexture_ = new Texture;
//	HRESULT hr;
//
//	if (FAILED(hr = pTexture_->Load("Assets\\dice.png")))
//	{
//		MessageBox(NULL, L"テクスチャの作成に失敗", L"Error", MB_OK);
//		return hr;
//	}
//	return S_OK;
//}
//
//void Quad::PassDateToCB(Transform& transform)
//{
//	CONSTANT_BUFFER cd;
//	cd.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
//	cd.matW = XMMatrixTranspose(transform.GetNormalMatrix());
//
//	D3D11_MAPPED_SUBRESOURCE pdata;
//	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
//	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cd), sizeof(cd));
//	Direct3D::pContext->Unmap(pConstantBuffer_, 0);
//
//}
//
//void Quad::SetBufferToPipeline()
//{
//	UINT stride = sizeof(VERTEX);
//	UINT offset = 0;
//	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
//
//	stride = sizeof(int);
//	offset = 0;
//	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
//
//	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
//	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);
//
//	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
//	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
//
//	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
//	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
//}
