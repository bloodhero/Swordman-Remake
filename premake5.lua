include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Swordman"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["entt"] = "%{wks.location}/Engine/vendor/entt/single_include"
IncludeDir["ffmpeg"] = "%{wks.location}/Engine/vendor/ffmpeg-4.3.1-win64/include"
IncludeDir["json"] = "%{wks.location}/Engine/vendor/json/single_include"
IncludeDir["Nuklear"] = "%{wks.location}/Engine/vendor/Nuklear"
IncludeDir["openal"] = "%{wks.location}/Engine/vendor/openal-soft-1.21.0-bin/include"
IncludeDir["SDL2"] = "%{wks.location}/Engine/vendor/SDL2-2.0.14/include"
IncludeDir["spdlog"] = "%{wks.location}/Engine/vendor/spdlog/include"
IncludeDir["stb_image"] = "%{wks.location}/Engine/vendor/stb"

LibDir = {}
LibDir["SDL2"] = "%{wks.location}/Engine/vendor/SDL2-2.0.14/lib/x64"
LibDir["ffmpeg"] = "%{wks.location}/Engine/vendor/ffmpeg-4.3.1-win64/lib"
LibDir["openal"] = "%{wks.location}/Engine/vendor/openal-soft-1.21.0-bin/libs/Win64"

group "Dependencies"
	include "vendor/premake"
group ""

include "Engine"
include "Sandbox"
