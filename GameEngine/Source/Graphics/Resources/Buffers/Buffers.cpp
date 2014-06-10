#include "Core.h"

#include "Buffers.h"

Buffer::~Buffer()
{
	SAFE_RELEASE(m_pBuffer);
}