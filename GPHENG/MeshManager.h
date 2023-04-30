#pragma once
#include "ResourceManager.h"
#include "Mesh.h"

class MeshManager: ResourceManager
{
public:
	MeshManager();
	~MeshManager();
	MeshPtr createMeshFromFile(const wchar_t* filePath);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath);
};