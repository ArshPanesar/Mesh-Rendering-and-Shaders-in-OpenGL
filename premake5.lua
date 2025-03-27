workspace "Shaders"
    configurations { "Debug", "Release" }

project "Shaders"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    
    toolset "clang"

    -- Using -w Option to Supress all Warnings
    buildoptions { "-fcolor-diagnostics", "-fansi-escape-codes", "-w" }

    files { "src/**.h", "src/**.cpp" }

    includedirs { "include" }

    libdirs { "libraries" }

    -- Had to rename libglfw3dll.a to liblibglfw3dll.a in libraries directory to avoid some linking error
    links { "libglfw3dll", "glew32", "opengl32" } -- "gdi32"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"