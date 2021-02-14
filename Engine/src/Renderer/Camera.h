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

		void setPosition(Vector2f pos) { m_Position = pos; }
		Vector2f Screen2World(Vector2f pos) { return pos + m_Position - m_Size / 2.0; }
		Vector2f World2Screen(Vector2f pos) { return pos - m_Position + m_Size / 2.0; }
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
		Vector2f m_Position;
		Vector2f m_Size;
	};


}
