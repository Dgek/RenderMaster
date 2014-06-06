#include "Core.h"

#include "StateFactory.h"

State * StateFactory::VCreateState(StateId id) const
{
	switch (id)
	{
	};

	//the state couldn't be created
	return nullptr;
}