#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
using namespace sf;
namespace meow {
	class Renderer :public RenderTarget
	{
	public:
		sf::Vector2u getSize() const override;
		Renderer();
		~Renderer();
		bool setActive(bool active = true) override;
		void display();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
