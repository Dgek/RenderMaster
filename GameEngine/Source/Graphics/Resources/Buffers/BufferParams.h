#pragma once

struct BufferParams : public D3D11_BUFFER_DESC
{
public:
	__forceinline bool FillVertexBufferParams(unsigned int vertexSize, unsigned int count,
		bool gpuWrite, bool cpuRead, bool cpuWrite, bool stream);

	__forceinline bool FillIndexBufferParams(unsigned int indexSize, unsigned int count,
		bool gpuWrite, bool cpuRead, bool cpuWrite, bool stream);

	__forceinline bool FillConstantBufferParams(unsigned int size,
		bool gpuWrite, bool cpuRead, bool cpuWrite);

	__forceinline bool FillStructredBufferParams(unsigned int structsize, unsigned int count, bool CPUWritable,
		bool GPUWritable);

	/** ================
	==	Mutators  ==
	================
	**/
	__forceinline void SetSize(unsigned int size);
};

__forceinline bool BufferParams::FillVertexBufferParams(unsigned int vertexSize, unsigned int count,
	bool gpuWrite, bool cpuRead, bool cpuWrite, bool stream)
{
	ZeroMemory(this, sizeof(D3D11_BUFFER_DESC));
	ByteWidth = vertexSize * count;

	if (cpuRead && cpuWrite && gpuWrite)
	{
		Usage = D3D11_USAGE_STAGING;
		CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}
	else if (gpuWrite && !cpuRead && !cpuWrite)
	{
		Usage = D3D11_USAGE_DEFAULT;
	}
	else if (cpuWrite && !cpuRead && !gpuWrite)
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
	}

	if (stream)
		BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	else
		BindFlags = D3D11_BIND_VERTEX_BUFFER;

	return true;
}

__forceinline bool BufferParams::FillIndexBufferParams(unsigned int indexSize, unsigned int count,
	bool gpuWrite, bool cpuRead, bool cpuWrite, bool stream)
{
	ZeroMemory(this, sizeof(D3D11_BUFFER_DESC));
	ByteWidth = indexSize * count;

	if (cpuRead && cpuWrite && gpuWrite)
	{
		Usage = D3D11_USAGE_STAGING;
		CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}
	else if (gpuWrite && !cpuRead && !cpuWrite)
	{
		Usage = D3D11_USAGE_DEFAULT;
	}
	else if (cpuWrite && !cpuRead && !gpuWrite)
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
	}

	if (stream)
		BindFlags = D3D11_BIND_INDEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	else
		BindFlags = D3D11_BIND_INDEX_BUFFER;

	return true;
}

__forceinline bool BufferParams::FillConstantBufferParams(unsigned int size,
	bool gpuWrite, bool cpuRead, bool cpuWrite)
{
	ZeroMemory(this, sizeof(D3D11_BUFFER_DESC));
	ByteWidth = size;

	if (cpuRead && cpuWrite && gpuWrite)
	{
		Usage = D3D11_USAGE_STAGING;
		CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}
	else if (gpuWrite && !cpuRead && !cpuWrite)
	{
		Usage = D3D11_USAGE_DEFAULT;
	}
	else if (cpuWrite && !cpuRead && !gpuWrite)
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
	}

	BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	return true;
}

__forceinline bool BufferParams::FillStructredBufferParams(unsigned int structsize, unsigned int count, bool CPUWritable,
	bool GPUWritable)
{
	ZeroMemory(this, sizeof(D3D11_BUFFER_DESC));
	ByteWidth = structsize * count;
	MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	StructureByteStride = structsize;

	if (!CPUWritable && !GPUWritable)
	{
		BindFlags = D3D11_BIND_SHADER_RESOURCE;
		Usage = D3D11_USAGE_IMMUTABLE;
		CPUAccessFlags = 0;
	}
	else if (CPUWritable && !GPUWritable)
	{
		BindFlags = D3D11_BIND_SHADER_RESOURCE;
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (!CPUWritable && GPUWritable)
	{
		BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		Usage = D3D11_USAGE_DEFAULT;
		CPUAccessFlags = 0;
	}
	else if (CPUWritable && GPUWritable)
	{
		assert(0 && "Error: Resource cannot be simultanuously writable by CPU and GPU");
	}

	return true;
}

__forceinline void BufferParams::SetSize(unsigned int size)
{
	ByteWidth = size;
}