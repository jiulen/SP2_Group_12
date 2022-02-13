#include "MeshBuilder.h"
#include "Vertex.h"
#include <GL\glew.h>
#include <vector>
#include "MyMath.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	v.pos.Set(-lengthX / 2, 0.f, 0.f);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, 0.f, 0.f);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, -lengthY / 2, 0.f);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, lengthY / 2, 0.f);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, 0.f, -lengthZ / 2);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, 0.f, lengthZ / 2);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);
	
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_LINES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.f);		v.color = color;	v.normal.Set(0, 0, 1);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.f);	v.color = color;	v.normal.Set(0, 0, 1);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.f);	v.color = color;	v.normal.Set(0, 0, 1);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.f);	v.color = color;	v.normal.Set(0, 0, 1);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateRepeatingQuad(const std::string& meshName, Color color, float repeats, float lengthX, float lengthY)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.f);		v.color = color;	v.normal.Set(0, 0, 1);	v.texCoord.Set(repeats, repeats); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.f);	v.color = color;	v.normal.Set(0, 0, 1);	v.texCoord.Set(0, repeats); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.f);	v.color = color;	v.normal.Set(0, 0, 1);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.f);	v.color = color;	v.normal.Set(0, 0, 1);	v.texCoord.Set(repeats, 0); vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	//Left
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(-1, 0, 0);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color; v.normal.Set(-1, 0, 0);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(-1, 0, 0);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(-1, 0, 0);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(-1, 0, 0);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(-1, 0, 0);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	//Back
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);		v.color = color; v.normal.Set(0, 0, -1);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, 0, -1);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, 0, -1);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);		v.color = color; v.normal.Set(0, 0, -1);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, 0, -1);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, 0, -1);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	//Bottom
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, -1, 0);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, -1, 0);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, -1, 0);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, -1, 0);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color; v.normal.Set(0, -1, 0);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, -1, 0);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	//Front
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, 0, 1);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color; v.normal.Set(0, 0, 1);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, 0, 1);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, 0, 1);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, 0, 1);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, 0, 1);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	//Right
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(1, 0, 0);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(1, 0, 0);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);		v.color = color; v.normal.Set(1, 0, 0);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(1, 0, 0);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(1, 0, 0); v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(1, 0, 0);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	//Top
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, 1, 0);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);		v.color = color; v.normal.Set(0, 1, 0);	v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, 1, 0);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, 1, 0);	v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color; v.normal.Set(0, 1, 0);	v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);		v.color = color; v.normal.Set(0, 1, 0);	v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
		
	for (unsigned i = 0; i < 36; ++i)
	{
		index_buffer_data.push_back(i);
	}

	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	
	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string& meshName, Color color, unsigned noOfSlices, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float radianPerSlice = Math::TWO_PI / noOfSlices;
	v.pos.Set(0, 0, 0); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float y = 0;
		float z = radius * sinf(theta);

		v.pos.Set(x, y, z); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(slice + 1);
		index_buffer_data.push_back(0);
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string& meshName, Color color, unsigned noOfSlices, float innerRadius, float outerRadius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float radianPerSlice = Math::TWO_PI / noOfSlices;
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = outerRadius * cosf(theta);
		float y = 0;
		float z = outerRadius * sinf(theta);
		v.pos.Set(x, y, z); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
		float X = innerRadius * cosf(theta);
		float Y = 0;
		float Z = innerRadius * sinf(theta);
		v.pos.Set(X, Y, Z); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(slice * 2);
		index_buffer_data.push_back(slice * 2 + 1);
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color color, unsigned noOfStacks, unsigned noOfSlices, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float radianPerStack = Math::PI / noOfStacks;
	float radianPerSlice = Math::TWO_PI / noOfSlices;
	for (unsigned stack = 0; stack < noOfStacks + 1; ++stack) {
		float phi = -Math::HALF_PI + stack * radianPerStack;
		for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
			float theta = slice * radianPerSlice;
			float x = cosf(phi) * cosf(theta);
			float y = sinf(phi);
			float z = cosf(phi) * sinf(theta);

			v.pos.Set(radius * x, radius * y, radius * z); v.color = color; v.normal.Set(x, y, z); v.texCoord.Set(slice, stack); vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < noOfStacks; ++stack) {
		for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
			index_buffer_data.push_back(stack * (noOfSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (noOfSlices + 1) + slice);
		}
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateHemisphere(const std::string& meshName, Color color, unsigned noOfStacks, unsigned noOfSlices, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float radianPerStack = Math::HALF_PI / noOfStacks;
	float radianPerSlice = Math::TWO_PI / noOfSlices;
	unsigned startIndex = 0;
	v.pos.Set(0, 0, 0); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v); //bottom circle
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		v.pos.Set(x, 0, z); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex);
		index_buffer_data.push_back(startIndex + slice + 1);
	}
	startIndex = vertex_buffer_data.size();
	for (unsigned stack = 0; stack < noOfStacks + 1; ++stack) {
		float phi = stack * radianPerStack;
		for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
			float theta = slice * radianPerSlice;
			float x = cosf(phi) * cosf(theta);
			float y = sinf(phi);
			float z = cosf(phi) * sinf(theta);

			v.pos.Set(radius * x, radius * y, radius * z); v.color = color; v.normal.Set(x, y, z);  vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < noOfStacks; ++stack) {
		for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
			index_buffer_data.push_back(startIndex + stack * (noOfSlices + 1) + slice);
			index_buffer_data.push_back(startIndex + (stack + 1) * (noOfSlices + 1) + slice);
		}
	}
	
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, unsigned noOfSlices, float height, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float radianPerSlice = Math::TWO_PI / noOfSlices;
	unsigned startIndex = 0;
	v.pos.Set(0, -height / 2, 0); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v); //bottom circle
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float y = -height * 0.5f;
		float z = radius * sinf(theta);

		v.pos.Set(x, y, z); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex);
		index_buffer_data.push_back(startIndex + slice + 1);
	}
	startIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) { //curved
		float theta = slice * radianPerSlice;
		float x = cosf(theta);
		float z = sinf(theta);

		v.pos.Set(radius * x, -height * 0.5f, radius * z); v.color = color; v.normal.Set(x, 0, z); vertex_buffer_data.push_back(v);
		v.pos.Set(radius * x, height * 0.5f, radius * z); v.color = color; v.normal.Set(x, 0, z); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex + 2 * slice);
		index_buffer_data.push_back(startIndex + 2 * slice + 1);
	}
	startIndex = vertex_buffer_data.size();
	v.pos.Set(0, height * 0.5f, 0); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v); //top circle
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float y = height * 0.5f;
		float z = radius * sinf(theta);

		v.pos.Set(x, y, z); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex + slice + 1);
		index_buffer_data.push_back(startIndex);
	}
	
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string& meshName, Color color, unsigned noOfSlices, float height, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float radianPerSlice = Math::TWO_PI / noOfSlices;
	unsigned startIndex = 0;
	v.pos.Set(0, -height * 0.5f, 0); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v); //bottom circle
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float y = -height * 0.5f;
		float z = radius * sinf(theta);

		v.pos.Set(x, y, z); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex);
		index_buffer_data.push_back(startIndex + slice + 1);
	}
	startIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = cosf(theta);
		float y = -height * 0.5f;
		float z = sinf(theta);

		v.pos.Set(radius * x, y, radius * z); v.color = color; v.normal.Set(height * x, radius, height * z); v.normal.Normalize(); vertex_buffer_data.push_back(v);
		v.pos.Set(0, height * 0.5f, 0); v.color = color; v.normal.Set(height * x, radius, height * z); v.normal.Normalize(); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex + 2 * slice);
		index_buffer_data.push_back(startIndex + 2 * slice + 1);
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateTruncatedCone(const std::string& meshName, Color color, unsigned noOfSlices, float height, float smallRadius, float bigRadius) { //CHECK
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float radianPerSlice = Math::TWO_PI / noOfSlices;
	unsigned startIndex = 0;
	v.pos.Set(0, -height * 0.5f, 0); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v); //bottom circle
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = bigRadius * cosf(theta);
		float y = -height * 0.5f;
		float z = bigRadius * sinf(theta);

		v.pos.Set(x, y, z); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex);
		index_buffer_data.push_back(startIndex + slice + 1);
	}
	//
	startIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) { //curved part
		float theta = slice * radianPerSlice;
		float x = cosf(theta);
		float z = sinf(theta);

		v.pos.Set(bigRadius * x, -height * 0.5f, bigRadius * z); v.color = color; v.normal.Set(height * x, bigRadius, height * z); v.normal.Normalize(); vertex_buffer_data.push_back(v);
		v.pos.Set(smallRadius * x, height * 0.5f, smallRadius * z); v.color = color; v.normal.Set(height * x, smallRadius, height * z); v.normal.Normalize(); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex + 2 * slice);
		index_buffer_data.push_back(startIndex + 2 * slice + 1);
	}
	//
	startIndex = vertex_buffer_data.size();
	v.pos.Set(0, height * 0.5f, 0); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v); //top circle
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		float theta = slice * radianPerSlice;
		float x = smallRadius * cosf(theta);
		float y = height * 0.5f;
		float z = smallRadius * sinf(theta);

		v.pos.Set(x, y, z); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
		index_buffer_data.push_back(startIndex + slice + 1);
		index_buffer_data.push_back(startIndex);
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string& meshName, Color color, unsigned noOfStacks, unsigned noOfSlices, float innerRadius, float outerRadius) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float radianPerStack = Math::TWO_PI / noOfStacks; float radianPerSlice = Math::TWO_PI / noOfSlices;
	float x1, z1;
	float x2, y2, z2;
	for (unsigned stack = 0; stack < noOfStacks + 1; stack++) {
		for (unsigned slice = 0; slice < noOfSlices + 1; slice++) {
			z1 = outerRadius * cosf(stack * radianPerStack);
			x1 = outerRadius * sinf(stack * radianPerStack);
			z2 = (outerRadius + innerRadius * cosf(slice * radianPerSlice)) * cosf(stack * radianPerStack);
			y2 = innerRadius * sinf(slice * radianPerSlice);
			x2 = (outerRadius + innerRadius * cosf(slice * radianPerSlice)) * sinf(stack * radianPerStack);

			v.pos.Set(x2, y2, z2); v.color = color; v.normal.Set(x2 - x1, y2, z2 - z1); v.normal.Normalize(); vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < noOfStacks; ++stack) {
		for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
			index_buffer_data.push_back(stack * (noOfSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (noOfSlices + 1) + slice);
		}
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}
Mesh* MeshBuilder::GenerateQuarterTorus(const std::string& meshName, Color color, unsigned noOfStacks, unsigned noOfSlices, float innerRadius, float outerRadius) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float radianPerStack = Math::HALF_PI / noOfStacks; float radianPerSlice = Math::TWO_PI / noOfSlices;
	float x1, z1;
	float x2, y2, z2;
	for (unsigned stack = 0; stack < noOfStacks + 1; stack++) {
		for (unsigned slice = 0; slice < noOfSlices + 1; slice++) {
			z1 = outerRadius * cosf(stack * radianPerStack);
			x1 = outerRadius * sinf(stack * radianPerStack);
			z2 = (outerRadius + innerRadius * cosf(slice * radianPerSlice)) * cosf(stack * radianPerStack);
			y2 = innerRadius * sinf(slice * radianPerSlice);
			x2 = (outerRadius + innerRadius * cosf(slice * radianPerSlice)) * sinf(stack * radianPerStack);

			v.pos.Set(x2, y2, z2); v.color = color; v.normal.Set(x2 - x1, y2, z2 - z1); v.normal.Normalize(); vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < noOfStacks; ++stack) {
		for (unsigned slice = 0; slice < noOfSlices + 1; ++slice) {
			index_buffer_data.push_back(stack * (noOfSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (noOfSlices + 1) + slice);
		}
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string& meshName, const std::string& file_path)
{
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if(!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	//Create the mesh and call glBindBuffer, glBufferData
	//Use triangle list and remember to set index size

	//test
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}
Mesh* MeshBuilder::GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path)
{
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	std::vector<Material> materials;
	bool success = LoadOBJMTL(file_path.c_str(), mtl_path.c_str(),
		vertices, uvs, normals, materials);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data,
		vertex_buffer_data);
	Mesh* mesh = new Mesh(meshName);
	for (Material& material : materials)
		mesh->materials.push_back(material);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	unsigned offset = 0;
	for (unsigned row = 0; row < numRow; ++row)
	{
		for (unsigned col = 0; col < numCol; ++col)
		{
			v.pos.Set(0.5f, 0.5f, 0.f);		v.normal.Set(0, 0, 1);	v.texCoord.Set((col + 1) * width, (numRow - row) * height); vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, 0.5f, 0.f);	v.normal.Set(0, 0, 1);	v.texCoord.Set(col * width, (numRow - row) * height); vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, -0.5f, 0.f);	v.normal.Set(0, 0, 1);	v.texCoord.Set(col * width, (numRow - 1 - row) * height); vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, -0.5f, 0.f);	v.normal.Set(0, 0, 1);	v.texCoord.Set((col + 1) * width, (numRow - 1 - row) * height); vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}
