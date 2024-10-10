//#include "Dice.h"
//#include "Camera.h"
//
//using namespace Direct3D;
//
//Dice::~Dice()
//{
//}
//
//void Dice::InitVertexDate()
//{
//	HRESULT hr;
//	// í∏ì_èÓïÒ
//	VERTEX vertices[] =
//	{
//		{{ -1.0f,  1.0f, -1.0f },  XMVectorSet(0.0, 0.0, 0.0, 0.0) ,XMVectorSet(0.0, 0.0, 0.0, -1.0)},
//		{{  1.0f,  1.0f, -1.0f },  XMVectorSet(0.25, 0.0, 0.0, 0.0) ,XMVectorSet(0.0, 0.0, 0.0, -1.0)},
//		{{ -1.0f, -1.0f, -1.0f },  XMVectorSet(0.0, 0.5, 0.0, 0.0) ,XMVectorSet(0.0, 0.0, 0.0, -1.0)},
//		{{  1.0f, -1.0f, -1.0f },  XMVectorSet(0.25, 0.5, 0.0, 0.0) ,XMVectorSet(0.0, 0.0, 0.0, -1.0)},
//
//		{{ -1.0f,  1.0f, 1.0f },  XMVectorSet(0.25, 0.5, 0.0, 0.0) ,XMVectorSet(0.0, 0.0, 0.0, 1.0)},
//		{{ -1.0f, -1.0f, 1.0f },  XMVectorSet(0.25, 1.0, 0.0, 0.0) ,XMVectorSet(0.0, 0.0, 0.0, 1.0)},
//		{{  1.0f,  1.0f, 1.0f },  XMVectorSet(0.5, 0.5, 0.0, 0.0) ,XMVectorSet(0.0, 0.0, 0.0, 1.0)},
//		{{  1.0f, -1.0f, 1.0f },  XMVectorSet(0.5, 1.0, 0.0, 0.0) ,XMVectorSet(0.0, 0.0, 0.0, 1.0)},
//
//		{{ -1.0f,  1.0f,  1.0f },  XMVectorSet(0.25, 0.0, 0.0, 0.0) ,XMVectorSet(-1.0, 0.0, 0.0, 0.0)},
//		{{ -1.0f,  1.0f, -1.0f },  XMVectorSet(0.5, 0.0, 0.0, 0.0) ,XMVectorSet(-1.0, 0.0, 0.0, 0.0)},
//		{{ -1.0f, -1.0f,  1.0f },  XMVectorSet(0.25, 0.5, 0.0, 0.0) ,XMVectorSet(-1.0, 0.0, 0.0, 0.0)},
//		{{ -1.0f, -1.0f, -1.0f },  XMVectorSet(0.5, 0.5, 0.0, 0.0) ,XMVectorSet(-1.0, 0.0, 0.0, 0.0)},
//
//		{{  1.0f,  1.0f,  1.0f },  XMVectorSet(0.0, 0.5, 0.0, 0.0) ,XMVectorSet(1.0, 0.0, 0.0, 0.0)},
//		{{  1.0f, -1.0f,  1.0f },  XMVectorSet(0.25, 0.5, 0.0, 0.0) ,XMVectorSet(1.0, 0.0, 0.0, 0.0)},
//		{{  1.0f,  1.0f, -1.0f },  XMVectorSet(0.0, 1.0, 0.0, 0.0) ,XMVectorSet(1.0, 0.0, 0.0, 0.0)},
//		{{  1.0f, -1.0f, -1.0f },  XMVectorSet(0.25, 1.0, 0.0, 0.0) ,XMVectorSet(1.0, 0.0, 0.0, 0.0)},
//
//		{{ -1.0f,  1.0f,  1.0f },  XMVectorSet(0.5, 0.0, 0.0, 0.0) ,XMVectorSet(0.0, 1.0, 0.0, 0.0)},
//		{{ -1.0f,  1.0f, -1.0f },  XMVectorSet(0.5, 0.5, 0.0, 0.0) ,XMVectorSet(0.0, 1.0, 0.0, 0.0)},
//		{{  1.0f,  1.0f,  1.0f },  XMVectorSet(0.75, 0.0, 0.0, 0.0) ,XMVectorSet(0.0, 1.0, 0.0, 0.0)},
//		{{  1.0f,  1.0f, -1.0f },  XMVectorSet(0.75, 0.5, 0.0, 0.0) ,XMVectorSet(0.0, 1.0, 0.0, 0.0)},
//
//		{{ -1.0f, -1.0f,  1.0f }, XMVectorSet(0.75, 0.0, 0.0, 0.0) ,XMVectorSet(0.0, -1.0, 0.0, 0.0)},
//		{{ -1.0f, -1.0f, -1.0f }, XMVectorSet(0.75, 0.5, 0.0, 0.0) ,XMVectorSet(0.0, -1.0, 0.0, 0.0)},
//		{{  1.0f, -1.0f,  1.0f }, XMVectorSet(1.0, 0.0, 0.0, 0.0) ,XMVectorSet(0.0, -1.0, 0.0, 0.0)},
//		{{  1.0f, -1.0f, -1.0f }, XMVectorSet(1.0, 0.5, 0.0, 0.0) ,XMVectorSet(0.0, -1.0, 0.0, 0.0)}
//
//	};
//	vertexNum_ = vertices_.size();
//}
//
//void Dice::InitIndexDate()
//{
//	int index[] = {
//	 0,  1,  2,     3,  2,  1,
//	 4,  5,  6,     7,  6,  5,
//	 8,  9, 10,    11, 10,  9,
//	12, 13, 14,    15, 14, 13,
//	16, 18, 17,    19, 17, 18,
//	20, 21, 22,    23, 22, 21, };
//	indexNum_ = index_.size();
//}
