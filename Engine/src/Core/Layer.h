#pragma once

#include <string>
#include <vector>
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

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}
