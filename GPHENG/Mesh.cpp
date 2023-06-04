#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>
#include <iostream>

#include "GraphicsEngine.h"
#include "Math.h"

Mesh::Mesh(const wchar_t* fullPath) : Resource(fullPath)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(fullPath);

	std::string mtldir = inputfile.substr(0, inputfile.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str(), mtldir.c_str());

	if (!err.empty()) throw std::exception("Mesh not created successfully");

	if (!res) throw std::exception("Mesh not created successfully");

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	size_t sizeVertexIndexLists = 0;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		sizeVertexIndexLists += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(sizeVertexIndexLists);
	list_indices.reserve(sizeVertexIndexLists);

	m_materialSlots.resize(materials.size());

	size_t indexGlobalOffset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		m_materialSlots[m].startIndex = indexGlobalOffset;
		m_materialSlots[m].materialID = m;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				if (shapes[s].mesh.material_ids[f] != m) 
				{
					continue;
				}

				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

					VertexMesh vertex(Vector3(vx, vy, vz), Vector2(tx, ty), Vector3(nx, ny, nz));
					list_vertices.push_back(vertex);

					list_indices.push_back((unsigned int)indexGlobalOffset + v);
				}

				index_offset += num_face_verts;
				indexGlobalOffset += num_face_verts;
			}
		}

		m_materialSlots[m].numIndicies = indexGlobalOffset - m_materialSlots[m].startIndex;
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	m_vertexBuffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh),
		(UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);
	m_indexBuffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertexBuffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_indexBuffer;
}

const MaterialSlot& Mesh::getMaterialSlot(unsigned int slot)
{
	if (slot >= m_materialSlots.size()) return MaterialSlot();
	return m_materialSlots[slot];
}

size_t Mesh::getNumMaterialSlots()
{
	return m_materialSlots.size();
}
