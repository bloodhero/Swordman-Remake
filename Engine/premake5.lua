project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/build/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/objdir")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	-- 宏定义
	defines
	{
		"ENGINE"
	}

	-- 附加包含目录
	includedirs
	{
		"src",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ffmpeg}",
		"%{IncludeDir.json}",
		"%{IncludeDir.Nuklear}",
		"%{IncludeDir.openal}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb_image}"
	}

	-- 附加库目录
	libdirs
	{
		"%{LibDir.SDL2}",
		"%{LibDir.ffmpeg}",
		"%{LibDir.openal}"
	}

	-- 链接的库
	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"avcodec.lib",
		"avformat.lib",
		"avutil.lib",
		"swscale.lib",
		"swresample.lib",
		"OpenAL32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DIST"
		runtime "Release"
		optimize "on"
