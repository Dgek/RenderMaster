#include "RenderDemo_pch.h"

#include "DemoStateFactory.h"
#include "States\TestState.h"

State* DemoStateFactory::VCreateState(StateId id) const
{
	switch (id)
	{
	case GS_BOXTEST:
		return new TestState{};
	};

	return StateFactory::VCreateState(id);
}