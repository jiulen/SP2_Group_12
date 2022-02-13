#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h"

struct Light
{
	Position position;
	Color color;
	float power;
	float kC, kL, kQ;
	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};
	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
	Light(): position(0.f, 20.f, 0.f), color(1.f,1.f,1.f), power(1.f), kC(1.f), kL(0.1f), kQ(0.001f), type(LIGHT_POINT), spotDirection(0.f, 1.f, 0.f), cosCutoff(cos(Math::DegreeToRadian(45))), cosInner(cos(Math::DegreeToRadian(30))), exponent(3.f)
	{
	}

};
#endif