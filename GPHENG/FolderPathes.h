#pragma once
#include <string>

class FolderPathes
{
public:
	static std::wstring codeFolder(const wchar_t* filePath)
	{
		const wchar_t* projectFolderPath = L"C:\\Users\\rdlit\\GitRepos\\CPP_GphEngine_V1\\GPHENG\\";
		std::wstring s(projectFolderPath);
		s += std::wstring(filePath);
		return s;
	}

	static std::wstring shadersFolder(const wchar_t* filePath)
	{
		const wchar_t* projectFolderPath = L"C:\\Users\\rdlit\\GitRepos\\CPP_GphEngine_V1\\GPHENG\\Shaders\\";
		std::wstring s(projectFolderPath);
		s += std::wstring(filePath);
		return s;
	}

	static std::wstring assetsFolder(const wchar_t* filePath)
	{
		const wchar_t* projectFolderPath = L"C:\\Users\\rdlit\\GitRepos\\CPP_GphEngine_V1\\GPHENG\\Assets\\";
		std::wstring s(projectFolderPath);
		s += std::wstring(filePath);
		return s;
	}

	static std::wstring texturesFolder(const wchar_t* filePath)
	{
		std::wstring s(assetsFolder(L"Textures\\"));
		s += std::wstring(filePath);
		return s;
	}

	static std::wstring meshesFolder(const wchar_t* filePath)
	{
		std::wstring s(assetsFolder(L"Meshes\\"));
		s += std::wstring(filePath);
		return s;
	}
};