#pragma once

#include <memory>
#include "Math/Vector2.h"
#include "Renderer/Drawable.h"
#include "Core/Manager.h"

namespace meow {

	class Camera
	{
	public:
		static void setSize(Vector2i size);
		static void onUpdate(float time);

		// CREARTORS
		Camera();
		~Camera();

		void setPosition(Vector2i pos);
		Vector2i Screen2World(Vector2i pos);
		Vector2i World2Screen(Vector2i pos);
		Rect getArea();
		void update(float time);
		void follow();
		void moteTo(Vector2f pos, float time);
		void setWorld(Rect area);
		void shake();

	private:
		static void addWatch(Camera* cam);
		static void delWatch(Camera* cam);
		Vector2i m_Position = { 0 , 0 };
		std::optional<Rect> m_world;
		static Vector2i s_Size;
		static std::vector<Camera*> s_Cameras;
	};

}
