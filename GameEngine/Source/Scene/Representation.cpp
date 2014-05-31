#include "Core.h"

#include "Representation.h"

EntityRepresentation::EntityRepresentation(EntityRepresentationId id, const string & resourceMeshName)
	: m_id(id), m_lastUpdate(0.0)
{}