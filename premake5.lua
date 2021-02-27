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
IncludeDir["cute_headers"] = "%{wks.location}/Engine/vendor/cute_headers"
IncludeDir["entt"] = "%{wks.location}/Engine/vendor/entt/single_include"
IncludeDir["ffmpeg"] = "%{wks.location}/Engine/vendor/ffmpeg-4.3.1-win64/include"
IncludeDir["freetype"] = "%{wks.location}/Engine/vendor/freetype-windows-binaries-2.10.4/include"
IncludeDir["glad"] = "%{wks.location}/Engine/vendor/glad/include"
IncludeDir["json"] = "%{wks.location}/Engine/vendor/json/single_include"
IncludeDir["Nuklear"] = "%{wks.location}/Engine/vendor/Nuklear"
IncludeDir["openal"] = "%{wks.location}/Engine/vendor/openal-soft-1.21.0-bin/include"
IncludeDir["SDL2"] = "%{wks.location}/Engine/vendor/SDL2-2.0.14/include"
IncludeDir["sfml_graphics"] = "%{wks.location}/Engine/vendor/SFML-Graphics/include"
IncludeDir["sol2"] = "%{wks.location}/Engine/vendor/sol2/single/include"
IncludeDir["spdlog"] = "%{wks.location}/Engine/vendor/spdlog/include"
IncludeDir["stb_image"] = "%{wks.location}/Engine/vendor/stb"

LibDir = {}
LibDir["SDL2"] = "%{wks.location}/Engine/vendor/SDL2-2.0.14/lib/x64"
LibDir["ffmpeg"] = "%{wks.location}/Engine/vendor/ffmpeg-4.3.1-win64/lib"
LibDir["openal"] = "%{wks.location}/Engine/vendor/openal-soft-1.21.0-bin/libs/Win64"
LibDir["freetype"] = "%{wks.location}/Engine/vendor/freetype-windows-binaries-2.10.4/Win64"

group "Dependencies"
	include "vendor/premake"
	include "Engine/vendor/SFML-Graphics"
	include "Engine/vendor/glad"
group ""

include "Engine"
include "Sandbox"
