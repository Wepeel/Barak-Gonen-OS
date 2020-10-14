workspace "Ex5_3"

    architecture "x86_64"
    configurations
    {
        "Debug",
        "Release"
    }

project "Ex5_3"
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