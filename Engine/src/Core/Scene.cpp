#include "Core/Scene.h"
#include "Core/Layer.h"
#include "Events/Event.h"

namespace meow {
	SceneStack::~SceneStack()
	{
		for (auto s : m_Scenes)
		{
			delete s;
		}
	}

	void SceneStack::runWithScene(Scene* scene)
	{
		if (m_Scenes.size() == 0)
		{
			m_Scenes.push_back(scene);
		}

	}

	void SceneStack::replaceScene(Scene* scene)
	{
		// cleanup the current state
		if (!m_Scenes.empty())
		{
			delete m_Scenes.back();
			m_Scenes.pop_back();
		}

		// store and init the new state
		m_Scenes.push_back(scene);
	}

	void SceneStack::pushScene(Scene* scene)
	{
		// pause current state
		if (!m_Scenes.empty())
		{
			m_Scenes.back()->onPause();
		}

		// store and init the new state
		m_Scenes.push_back(scene);
	}

	void SceneStack::popScene()
	{
		// cleanup the current state
		if (!m_Scenes.empty())
		{
			delete m_Scenes.back();
			m_Scenes.pop_back();
		}

		// resume previous state
		if (!m_Scenes.empty())
		{
			m_Scenes.back()->onResume();
		}
	}

	Scene* SceneStack::getCurrentScene()
	{
		return m_Scenes.back();
	}


	Scene::Scene() :
		m_LayerStack(new LayerStack())
	{
	}


	Scene::~Scene()
	{
		delete m_LayerStack;
	}

	void Scene::onDraw()
	{
		for (auto it = m_LayerStack->begin(); it != m_LayerStack->end(); ++it)
		{
			(*it)->onDraw();
		}
	}


	void Scene::onUpdate(float time)
	{
		for (Layer* layer : *m_LayerStack)
			layer->onUpdate(time);
	}


	meow::LayerStack* Scene::getLayerStack()
	{
		return m_LayerStack;
	}

}
