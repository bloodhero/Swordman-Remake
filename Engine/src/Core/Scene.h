#pragma once

#include <string>

#include "Core/Utils.h"

namespace meow {
	class Scene
	{
	public:
		// CREARTORS
		Scene(std::string_view name = "Scene");
		virtual ~Scene();

		// ACCESSORS

		// MANIPULATORS
		virtual void onPause() = 0;
		virtual void onResume() = 0;

		void onEvent();
		void onDraw();
		void onUpdate(float time);

	private:
		struct Impl;
		Ref<Impl> p_Impl;
	};
}
