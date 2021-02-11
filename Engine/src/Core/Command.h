#pragma once

#include "Game/GameActor.h"

namespace meow {
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void execute(GameActor& actor) = 0;
		virtual void undo() = 0;
	};

	class CommandList
	{
	public:
		void addCommand(Command*);
		bool undo();
		bool redo();
	};
}
