#pragma once

#include <memory>
#include "Math/Vector2.h"
#include "Renderer/Drawable.h"

namespace meow {

	class Camera
	{
	public:
		Camera(int width, int height) :
			m_Size({ width, height }),
			m_Position({ 0,0 })
		{}
		~Camera() = default;

		void setPosition(Vector2i pos) { m_Position = pos; }
		Vector2i Screen2World(Vector2i pos) { return pos + m_Position - m_Size / 2.0; }
		Vector2i World2Screen(Vector2i pos) { return pos - m_Position + m_Size / 2.0; }
		Rect getArea()
		{
			auto real_position = m_Position - m_Size / 2.0f;
			return {
				real_position.x,
				real_position.y,
				m_Size.x,
				m_Size.y
			};
		}

	private:
		Vector2i m_Position;
		Vector2i m_Size;
	};


}
