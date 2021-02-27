project "SFML-Graphics"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/build/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/objdir")

	files
	{
		"**.hpp",
		"**.cpp"
	}

	-- �궨��
	defines
	{
		"SFML_STATIC"
	}

	-- ���Ӱ���Ŀ¼
	includedirs
	{
		"include",
		"src",
		"%{IncludeDir.freetype}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.stb_image}"
	}

	-- ���ӿ�Ŀ¼
	libdirs
	{
		"%{LibDir.freetype}",
		"%{LibDir.SDL2}",
		"Glad"
	}

	-- ���ӵĿ�
	links
	{

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
