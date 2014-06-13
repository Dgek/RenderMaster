#include "Core.h"

#include "MeshLoader.h"

string MeshLoader::VGetPattern()
{}

bool MeshLoader::VUseRawFile()
{}

bool MeshLoader::VNeedFile()
{}

int MeshLoader::VGetLoadedResourceSize(char* pRawBuffer, unsigned int uRawSize)
{}

bool VLoadResource(char* pRawBuffer, unsigned int uRawSize, std::shared_ptr<ResHandle> pHandle)
{}