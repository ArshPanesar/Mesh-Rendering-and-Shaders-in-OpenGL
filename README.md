
# Shading with OpenGL

This project was done as part of an assignment in a Computer Graphics course.

## Controls

- **W**: Translate in the +Y Direction

- **X**: Translate in the -Y Direction

- **D**: Translate in the +X Direction

- **A**: Translate in the -X Direction

- **E**: Translate in the -Z Direction

- **Q**: Translate in the +Z Direction

- **Left Arrow**: Scale Down by 1%

- **Right Arrow**: Scale Up by 1%

- **Hold Right Click and Move Mouse**: Change LookAt Direction of Camera.

## Platform and Libraries Used

I have used the Windows operating system to build this project, alongwith the following technologies:

- **Libraries for OpenGL:** GLFW and GLEW were used to run the sample program provided in the assignment.

- **MinGW:** MinGW binaries were installed to run the project. The project uses the Clang++ compiler along with the lldb linker.

- **Premake5:** I used premake as a build system for my project since it was easier to build the project with multiple source files and headers.

- **Make:** Premake acts as a makefile generator, and the project can be compiled by using the make package.

- **ImGui:** ImGui is a fast and bloat-free library that is used to quickly build user interfaces and render them at runtime. ImGui is used in this project to provide the user an easy way to (1) view and change parameters for perspective projection, (2) applying different shading techniques, (3) additional UI for interacting with light and meshes in the scene. Section 3 provides a detailed description on navigating these menus.

## Compiling Instructions

To compile the project:

- Open the project folder in Visual Studio Code and press F5. This will run the entire compilation process and executes the binary generated. The tasks.json file contains two tasks: 
    1. Run premake5 with the gmake argument to generate a makefile for the project.
    2. Compile the Project using the make command. 
    3. Then, launch.json project simply launches the binary generated after compilation. 
    
    
[Make](https://gnuwin32.sourceforge.net/packages/make.htm) and [Premake5](https://premake.github.io/) are necessary to compile the project. You can try compiling with just Make, but if that does not work, running Premake first may be necessary. 

The C++ compiler and linker can be changed to use **g++** and **gdb** linker instead, by editing these files: premake.lua, tasks.json and launch.json.  
