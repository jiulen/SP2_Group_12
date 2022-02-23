#pragma once
class Rain
{
private:
	float x, y, z;
public:
	Rain(float a, float b, float c);
	~Rain();
	float getpos(int a);
	void setpos(int a, float b);
};

