#ifndef MATERIAL_H
#define MATERIAL_H

struct Component
{
	float r, g, b;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f)
	{
		Set(r, g, b);
	}
	Component& operator=(const Component& rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		return *this;
	}
	void Set(float r, float g, float b)
	{
		this->r = r; this->g = g; this->b = b;
	}
};
struct Material
{
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess;
	unsigned size;
	Material& operator=(const Material& rhs)
	{
		kAmbient = rhs.kAmbient;
		kDiffuse = rhs.kDiffuse;
		kSpecular = rhs.kSpecular;
		kShininess = rhs.kShininess;
		size = rhs.size;
		return *this;
	}
	Material() : kAmbient(0.1f, 0.1f, 0.1f), kDiffuse(0.6f, 0.6f, 0.6f), kSpecular(0.3f, 0.3f, 0.3f), kShininess(1.f), size(0)
	{
	}
};
#endif