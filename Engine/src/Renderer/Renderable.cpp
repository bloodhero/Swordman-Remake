#include "pch.h"
#include "Renderer/Renderable.h"


void meow::Renderable::draw(std::optional<Rect> dst)
{

	texture->setAlphaMod(alphaMod);
	texture->setColorMod(colorMod);
	texture->setBlendMode(blendMode);
	texture->draw(slice, dst);

	// reset state
	texture->setAlphaMod(std::nullopt);
	texture->setBlendMode(std::nullopt);
	texture->setColorMod(std::nullopt);
}
