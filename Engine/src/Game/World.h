#pragma once

#include "Game/Terrain.h"

namespace meow {
	class World
	{
	public:
		World():grassTerrain_(),
			hillTerrain_(),
			riverTerrain_()
		{}

		Terrain& getTile(int x, int y);

	private:
		int WIDTH;
		int HEIGHT;
		Terrain* tiles;//WIDTH * HEIGHT

		// ÆäËû´úÂë¡­¡­

		Terrain grassTerrain_;
		Terrain hillTerrain_;
		Terrain riverTerrain_;
	};
}
