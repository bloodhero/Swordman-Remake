#include "Core/Scene.h"

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

}
