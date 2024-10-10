#pragma once
#include"Transform.h"

#include <dInput.h>
#include <DirectXMath.h>
#include "XInput.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")
#pragma comment(lib, "Xinput.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

using namespace DirectX;

namespace Input
{
	

	void Initialize(HWND hWnd);
	void Update();
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);//‰Ÿ‚µ‚½î•ñ‚ğ‹L˜^‚·‚é
	void Release();

	bool IsMouseButton(int buttonCode);
	bool IsMouseButtonDown(int buttonCode);
	bool IsMouseButtonUp(int buttonCode);
	XMFLOAT3 GetMousePosition();
	void SetMousePosition(int x, int y);
	XMFLOAT3 GetMouseMove();



};