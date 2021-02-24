#pragma once

#include <string>
#include <vector>
#include "Core/Utils.h"

namespace meow {
	class LayerStack;
	class Scene
	{
	public:
		// CREARTORS
		Scene();
		virtual ~Scene();

		// ACCESSORS

		// MANIPULATORS
		virtual void onCreate() = 0;
		virtual void onDestroy() = 0;
		virtual void onPause() = 0;
		virtual void onResume() = 0;
		virtual void onNuklear() = 0;

		void onDraw();
		void onUpdate(float time);
		LayerStack* getLayerStack();

	private:
		LayerStack* m_LayerStack;
	};

	class SceneStack
	{
	public:
		SceneStack() = default;
		~SceneStack();
		void runWithScene(Scene* scene);
		void replaceScene(Scene* scene);
		void pushScene(Scene* scene);
		void popScene();
		Scene* getCurrentScene();
	private:
		std::vector<Scene*> m_Scenes;
	};
}
