#ifndef SIMPLE_LEVEL_H
#define SIMPLE_LEVEL_H

#include "Level.h"

namespace BEngine
{
	//-----------------------------------------------------------------------
	//CSimpleLevel - non stream able
	//
	//Simple level class that just forces to load all entities once,
	//doesn't do any level management, used for tests.
	//-----------------------------------------------------------------------

	class CSimpleLevel : public ILevel
	{
	public:
		CSimpleLevel();
		~CSimpleLevel() = default;

		void loadLevel(const LevelUpdateInfo& lInfo) override final;
		void reload() override final;

		void update(const LevelUpdateInfo& lInfo) override final;
		void shutDown() override final;

	};
}

#endif //SIMPLE_LEVEL_H
