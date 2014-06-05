#include "RenderDemo_pch.h"

#include "DemoStateFactory.h"

State * DemoStateFactory::VCreateState(int id) const
{
	switch (id)
	{
	case GS_BOXTEST:
		return new BoxTestState{};
	};

	return StateFactory::VCreateState(id);
}