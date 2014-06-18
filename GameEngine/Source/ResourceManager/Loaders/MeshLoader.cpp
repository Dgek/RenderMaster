#include "Core.h"

#include "MeshLoader.h"

#include "../../Graphics/Resources/Buffers/BufferParams.h"
#include "../../Graphics/Resources/GraphicsResource.h"

#include "../../Math/float2.h"
#include "../../Math/float3.h"

#include "../../Renderer/Meshes/IndexedMesh.h"

string MeshResourceExtraData::VToString()
{
	return "MeshResourceExtraData";
}

bool MeshLoader::VLoadResource(char* pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	auto pExtra = make_shared<MeshResourceExtraData>();
	pHandle->SetExtra(pExtra);

	FILE *file;
	fopen_s(&file, pRawBuffer, "rb");
	if (!file)
		return false;

	// check idString
	char idString[5];
	memset(idString, 0, 5);
	fread(idString, sizeof(char), 4, file);
	if (strcmp(idString, "DMSH") != 0)
	{
		fclose(file);
		return false;
	}

	// check version
	int version;
	fread(&version, sizeof(int), 1, file);
	if (version != 1)
	{
		fclose(file);
		return false;
	}

	// get number of vertices
	int numVertices;
	fread(&numVertices, sizeof(int), 1, file);
	if (numVertices < 3)
	{
		fclose(file);
		return false;
	}

	struct GEOMETRY_VERTEX
	{
		float3 position;
		float2 texCoords;
		float3 normal;
		Vec tangent;
	};
	// load vertices
	GEOMETRY_VERTEX *vertices = new GEOMETRY_VERTEX[numVertices];
	if (!vertices)
	{
		fclose(file);
		return false;
	}
	fread(vertices, sizeof(GEOMETRY_VERTEX), numVertices, file);

	//create vertex buffer
	float3* pos = new float3[numVertices];
	float2* texCoords = new float2[numVertices];
	float3* normals = new float3[numVertices];
	Vec* tangents = new Vec[numVertices];

	for (auto i = 0; i < numVertices; ++i)
	{
		pos[i] = vertices[i].position;
		texCoords[i] = vertices[i].texCoords;
		normals[i] = vertices[i].normal;
		tangents[i] = vertices[i].tangent;
	}

	BufferParams params;
	params.FillVertexBufferParams(sizeof(float3), numVertices, true, false, false, false);

	SubresourceData data;
	data.SetData(pos);

	if (!pExtra->m_pVertices->Create(params, &data, numVertices, sizeof(float3)))
	{
		assert(0 && "Error creating vertex buffer!");
	}

	params.FillVertexBufferParams(sizeof(float2), numVertices, true, false, false, false);
	data.SetData(texCoords);
	if (!pExtra->m_pTexCoords->Create(params, &data, numVertices, sizeof(float2)))
	{
		assert(0 && "Error creating vertex buffer!");
	}

	params.FillVertexBufferParams(sizeof(float3), numVertices, true, false, false, false);
	data.SetData(normals);
	if (!pExtra->m_pNormals->Create(params, &data, numVertices, sizeof(float3)))
	{
		assert(0 && "Error creating vertex buffer!");
	}

	params.FillVertexBufferParams(sizeof(Vec), numVertices, true, false, false, false);
	data.SetData(tangents);
	if (!pExtra->m_pTangents->Create(params, &data, numVertices, sizeof(Vec)))
	{
		assert(0 && "Error creating vertex buffer!");
	}

	SAFE_DELETE_ARRAY(tangents);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(texCoords);
	SAFE_DELETE_ARRAY(pos);
	SAFE_DELETE_ARRAY(vertices);

	// get number of indices
	int numIndices;
	fread(&numIndices, sizeof(int), 1, file);
	if (numIndices<3)
	{
		fclose(file);
		return false;
	}

	// load indices 
	int *indices = new int[numIndices];
	if (!indices)
	{
		fclose(file);
		return false;
	}
	fread(indices, sizeof(int), numIndices, file);

	params.FillIndexBufferParams(sizeof(int), numIndices, true, false, false, false);
	data.SetData(indices);
	pExtra->m_pIndexBuffer->Create(params, &data, numIndices, sizeof(int));

	SAFE_DELETE_ARRAY(indices);

	// get number of sub-meshes
	int numSubMeshes;
	fread(&numSubMeshes, sizeof(int), 1, file);
	if (numSubMeshes<1)
	{
		fclose(file);
		return false;
	}

	//load and create submeshes
	pExtra->m_pMeshes.reserve(numSubMeshes);
	pExtra->m_uNumMeshes = numSubMeshes;
	for (int i = 0; i < numSubMeshes; ++i)
	{
		auto pMesh = make_shared<IndexedMesh>();
		char materialName[256];
		fread(materialName, sizeof(char), 256, file);
		pMesh->SetMaterial(materialName);

		fread(&pMesh->m_firstIndex, sizeof(int), 1, file);
		fread(&pMesh->m_numIndices, sizeof(int), 1, file);
	}

	return true;
}