#include "Controller.h"
#include "Input.h"
#include "Camera.h"

using namespace Camera;

Controller::Controller()
{transform.position_ = {7, 3, 7};
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
	
	
}

void Controller::Update()
{
	if (Input::IsKey(DIK_D))
	{
		transform.position_.x += 0.02;

		//target.x += 0.2;
	}
	if (Input::IsKey(DIK_A))
	{
		transform.position_.x -= 0.02;
	//	target.x -= 0.2;
	}
	if (Input::IsKey(DIK_W))
	{
		transform.position_.z += 0.02;
	//	target.z += 0.02;
	}
	if (Input::IsKey(DIK_S))
	{
		transform.position_.z -= 0.02;
		//target.z += 0.02;
	}

	//Camera Limit Range
	if (transform.position_.x < 4)
	{
		transform.position_.x = 4;
	}
	if (transform.position_.x > 15)
	{
		transform.position_.x = 15;
	}
	if (transform.position_.z < 2)
	{
		transform.position_.z = 2;
	}
	if (transform.position_.z > 10)
	{
		transform.position_.z = 10;
	}

	XMVECTOR POS_ = XMLoadFloat3(&transform.position_);//transform　はxmfloat型からXMVECTORに変換した
    XMVECTOR target =XMVectorSet (0, 7.0,-10.0,0 );
    XMVECTOR CamPos = POS_ + target;
	Camera::SetPosition(CamPos);
	Camera::SetTarget(POS_);
}

void Controller::Draw()
{
}

void Controller::Release()
{
}
