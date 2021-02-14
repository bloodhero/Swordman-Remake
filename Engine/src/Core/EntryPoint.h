#pragma once

#include "Core/Application.h"
#include "Core/Manager.h"

// To be defined in CLIENT
meow::Scene* meow::createStartScene();

int main(int argc, char* const argv[])
{
	auto app = meow::Application::getApplication();

	app->runWithScene(meow::createStartScene());

	app->onRun();
}
