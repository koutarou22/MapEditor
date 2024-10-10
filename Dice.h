#pragma once
#include "Quad.h"
#include "Quad.h"
#include "Transform.h"
class Dice :
	public Quad
{

public:
	Dice() :Quad() {};
	~Dice();
	void InitVertexDate() override;
	void InitIndexDate() override;
};
