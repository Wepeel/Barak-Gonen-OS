workspace "PrimeProcess"

    architecture "x86_64"
    configurations
    {
        "Debug",
        "Release"
    }

project "PrimeProcess"
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