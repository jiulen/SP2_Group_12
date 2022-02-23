#include "Rain.h"
Rain::Rain(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}
Rain::~Rain()
{
}
float Rain::getpos(int a)
{
	if (a == 1)
		return x;
	else if (a == 2)
		return y;
	else if (a == 3)
		return z;
}
void Rain::setpos(int a, float b)
{
	if (a == 1)
		x = b;
	else if (a == 2)
		y = b;
	else if (a == 3)
		z = b;
}
