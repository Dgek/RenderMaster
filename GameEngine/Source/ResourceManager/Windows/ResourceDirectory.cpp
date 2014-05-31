#include "Core.h"

#include "ResourceDirectory.h"
#include "../Resource.h"

ResourceDir::ResourceDir(const wstring & resourceDir)
{
	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);

	m_directory = dir;
	int iLastSlashPos = m_directory.find_last_of(L"\\");
	//m_directory = m_directory.substr(0, iLastSlashPos);
	m_directory += L"\\Resources\\";
}

bool ResourceDir::VOpen()
{
	ReadResourceDirectory(L"*");
	return true;
}

void ResourceDir::ReadResourceDirectory(const wstring & pattern)
{
	HANDLE pHandle;
	WIN32_FIND_DATA data;

	wstring path = m_directory + pattern;
	pHandle = FindFirstFile(path.c_str(), &data);
	if (pHandle != INVALID_HANDLE_VALUE)
	{
		//iterate through all files left in directory
		while (FindNextFile(pHandle, &data))
		{
			if (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;

			wstring fileName = data.cFileName;
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (fileName != L".." && fileName != L".")
				{
					fileName = pattern.substr(0, pattern.length() - 1) + fileName + L"\\*";
					ReadResourceDirectory(fileName);
				}
			}
			else
			{
				fileName = pattern.substr(0, pattern.length() - 1) + fileName;
				wstring lower = fileName;
				//std::transform(lower.begin(), lower.end(), lower.begin(), std::tolower);
				//std::transform(lower.begin(), lower.end(), lower.begin(), (int(*)(int)) std::tolower<int>);
				wcscpy_s(&data.cFileName[0], MAX_PATH, lower.c_str());
				m_directoryContent[StringHelper::ws2s(lower)] = m_resourcesData.size();
				m_resourcesData.push_back(data);
			}
		}
	}

	FindClose(pHandle);
}

int ResourceDir::Find(const string & name)
{
	auto lower = name;
	//std::transform(lower.begin(), lower.end(), lower.begin(), (AINT32(*)(AINT32)) std::tolower<AINT32>);
	const auto it = m_directoryContent.find(lower);

	if (it != end(m_directoryContent))
	{
		return it->second;
	}

	//no such resource in the directory
	return -1;
}

int ResourceDir::VGetRawResourceSize(const Resource & resource)
{
	auto iNum = Find(resource.m_name.c_str());
	if (iNum == -1)
		return -1;

	return m_resourcesData[iNum].nFileSizeLow;
}

int ResourceDir::VGetRawResource(const Resource & resource, char* pBuffer)
{
	auto iNum = Find(resource.m_name.c_str());
	if (iNum == -1)
		return -1;

	auto fullFileSpec = StringHelper::ws2s(m_directory) + resource.m_name.c_str();
	FILE* pFile = fopen(fullFileSpec.c_str(), "rb");
	auto bytes = fread(pBuffer, 1, m_resourcesData[iNum].nFileSizeLow, pFile);
	fclose(pFile);

	return bytes;
}