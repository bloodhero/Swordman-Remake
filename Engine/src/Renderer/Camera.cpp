#include "pch.h"
#include "Renderer/Camera.h"

namespace meow {


	void Camera::setSize(Vector2i size)
	{
		s_Size = size;
	}

	void Camera::setPosition(Vector2i pos)
	{
		m_Position = pos;
	}

	meow::Vector2i Camera::Screen2World(Vector2i pos)
	{
		return pos + m_Position - s_Size / 2.0;
	}

	meow::Vector2i Camera::World2Screen(Vector2i pos)
	{
		return pos - m_Position + s_Size / 2.0;
	}

	meow::Rect Camera::getArea()
	{
		auto real_position = m_Position - s_Size / 2.0f;
		return {
			real_position.x,
			real_position.y,
			s_Size.x,
			s_Size.y
		};
	}


	void Camera::update(float time)
	{

	}

	void Camera::onUpdate(float time)
	{
		for (auto c : s_Cameras)
		{
			c->update(time);
		}
	}


	Camera::Camera()
	{
		addWatch(this);
	}


	Camera::~Camera()
	{
		delWatch(this);
	}

	void Camera::addWatch(Camera* cam)
	{
		s_Cameras.push_back(cam);
	}


	void Camera::delWatch(Camera* cam)
	{
		auto it = std::find(s_Cameras.begin(), s_Cameras.end(), cam);
		if (it != s_Cameras.end())
		{
			s_Cameras.erase(it);
		}
	}

	void Camera::follow()
	{

	}


	void Camera::moteTo(Vector2f pos, float time)
	{

	}


	void Camera::setWorld(Rect area)
	{

	}


	void Camera::shake()
	{

	}


	Vector2i Camera::s_Size;

	std::vector<Camera*> Camera::s_Cameras;

}
