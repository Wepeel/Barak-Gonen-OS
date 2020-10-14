workspace "Ex4_5"

    architecture "x86_64"
    configurations
    {
        "Debug",
        "Release"
    }

project "Ex4_5"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    files
    {
        "*.h",
        "*.cpp"
    }

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"