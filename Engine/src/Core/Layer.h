#pragma once

#include <string>

#include "Core/Utils.h"

namespace meow {
	class Layer
	{
	public:
		// CREARTORS
		Layer(std::string_view name = "Layer"): m_Name(name) {}
		virtual ~Layer() = default;

		// ACCESSORS
		std::string_view getName() const { return m_Name; }

		// MANIPULATORS
		virtual void onAttach() = 0;
		virtual void onDetach() = 0;

		virtual void onDraw() = 0;
		virtual void onUpdate() = 0;
		virtual void onEvent() = 0;

	protected:
		std::string m_Name;
	};
}
