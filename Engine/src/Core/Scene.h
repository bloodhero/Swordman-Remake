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
		Scene(std::string_view name = "Scene");
		virtual ~Scene();

		// ACCESSORS

		// MANIPULATORS
		virtual void onPause() = 0;
		virtual void onResume() = 0;

		void onEvent();
		void onDraw();
		void onUpdate(float time);

		LayerStack* getLayerStack();
	private:
		std::string m_Name;
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
