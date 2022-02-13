#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX = 1.f, float lengthY = 1.f);
	static Mesh* GenerateRepeatingQuad(const std::string& meshName, Color color, float repeats = 1.f, float lengthX = 1.f, float lengthY = 1.f);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX = 1.f, float lengthY = 1.f, float lengthZ = 1.f);
	static Mesh* GenerateCircle(const std::string& meshName, Color color, unsigned noOfSlices = 10, float radius = 1.f);
	static Mesh* GenerateRing(const std::string& meshName, Color color, unsigned noOfSlices = 10, float innerRadius = 0.3f, float outerRadius = 0.5f);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned noOfStacks = 10, unsigned noOfSlices = 10, float radius = 0.5f);
	static Mesh* GenerateHemisphere(const std::string& meshName, Color color, unsigned noOfStacks = 10, unsigned noOfSlices = 10, float radius = 0.5f);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, unsigned noOfSlices = 10, float height = 1.f, float radius = 0.5f);
	static Mesh* GenerateCone(const std::string& meshName, Color color, unsigned noOfSlices = 10, float height = 1.f, float radius = 0.5f);
	static Mesh* GenerateTruncatedCone(const std::string& meshName, Color color, unsigned noOfSlices = 10, float height = 1.f, float smallRadius = 0.3f, float bigRadius = 0.5f);
	static Mesh* GenerateTorus(const std::string& meshName, Color color, unsigned noOfStacks = 10, unsigned noOfSlices = 10, float innerRadius = 0.3f, float outerRadius = 0.5f);
	static Mesh* GenerateQuarterTorus(const std::string& meshName, Color color, unsigned noOfStacks = 10, unsigned noOfSlices = 10, float innerRadius = 0.3f, float outerRadius = 0.5f);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
};

#endif