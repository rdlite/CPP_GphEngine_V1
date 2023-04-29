#pragma once
#include <unordered_map>
#include <string>
#include "Prerequisites.h"
#include "Resource.h"
#if __cplusplus <= 201402L 
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#endif
#if __cplusplus >= 201703L
#include <filesystem>
#endif

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	ResourcePtr createResourceFromFile(const wchar_t* filePath);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath) = 0;
private:
	std::unordered_map<std::wstring, ResourcePtr> m_mapResources;
};