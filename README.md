
# Shading with OpenGL

This project was done as part of an assignment in a Computer Graphics course.

## Images

The Scene

![default-view](https://github.com/user-attachments/assets/30610be6-ebb8-4a88-a1e5-954623616383)

Perspective Projection

![pers-proj](https://github.com/user-attachments/assets/7ca5e2b1-c3be-4dcd-a625-0759e21fe625)

---

Visualizing the Depth Buffer

Before

![before-depth-shader](https://github.com/user-attachments/assets/d0ac88c8-df24-46f9-90d6-a01c2dcf6b6d)

After

![after-depth-shader](https://github.com/user-attachments/assets/f78f2044-3233-4861-a6d1-f9bd930aa586)

---

Shaders

Gouraud Shaders

![gouraud](https://github.com/user-attachments/assets/b78011b5-a770-4146-b2ac-836b00fa5e0d)

Phong Shaders

![phong](https://github.com/user-attachments/assets/134962fd-f0cc-48e7-a827-ff7f8a2c1652)

Flat Shaders

![flat](https://github.com/user-attachments/assets/dd7f0e03-dd92-4822-8406-1f9620625933)


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
