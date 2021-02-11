#pragma once

namespace meow {
	class Terrain
	{
	public:
		Terrain() = default;
		int getMovementCost() const;
		bool isWater() const;
		void getTexture() const;
	};
}
