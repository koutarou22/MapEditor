#pragma once

#include <d3d11.h>
#include <assert.h>
enum SHADER_TYPE
{
	SHADER_2D,
	SHADER_3D,
	SHADER_MAX,
};
//???Åg?J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
namespace Direct3D
{
	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;
	//?ÅÒ??ÅÒ?
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	HRESULT InitShader();
	HRESULT InitShader2D();
	HRESULT InitShader3D();

	void SetShader(SHADER_TYPE type);

	void BeginDraw();

	void EndDraw();

	void Release();

	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;
	extern const int WINDOW_WIDTH;
	extern const int WINDOW_HEIGHT;
};