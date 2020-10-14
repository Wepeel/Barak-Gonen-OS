workspace "DLL"

    architecture "x86_64"
    configurations
    {
        "Debug",
        "Release"
    }

project "DLL"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    files
    {
        "*.h",
        "*.cpp"
    }

    pchheader "pch.h"
    pchsource "pch.cpp"

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"