#include "ResourceManager.h"
#include "iostream"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

ResourcePtr ResourceManager::createResourceFromFile(const wchar_t* filePath)
{
#if (_MSC_VER >= 1900 && _MSC_VER <= 1916)  || ( _MSC_VER >= 1920 && __cplusplus <= 201402L) 
	std::wstring full_path = std::experimental::filesystem::absolute(filePath);
#endif

#if _MSC_VER >= 1920 && __cplusplus > 201402L 
	std::wstring full_path = std::filesystem::absolute(file_path);
#endif

	auto it = m_mapResources.find(full_path);

	if (it != m_mapResources.end()) 
	{
		return it->second;
	}

	Resource* rawRes = this->createResourceFromFileConcrete(full_path.c_str());

	if (rawRes)
	{
		ResourcePtr res(rawRes);
		m_mapResources[full_path] = res;
		return res;
	}

	return nullptr;
}