#pragma once

#include "Core/Application.h"
#include "Core/Manager.h"

// To be defined in CLIENT
meow::Bootstrap* meow::createBootstrap();

int main(int argc, char* const argv[])
{
	auto app = meow::Application::getApplication();
	auto manager = meow::Manager::getManager();
	auto booter = meow::createBootstrap();

	app->onConfig(booter);

	manager->runWithScene(booter->firstScene());

	app->onRun();
}
