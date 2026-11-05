- [Report 1: Project initialization](#report-1-project-initialization)
- [Goals](#goals)
- [Future work](#future-work)
- [Application structure](#application-structure)
  - [Run the program](#run-the-program)
  - [External dependencies](#external-dependencies)
  - [Code structure](#code-structure)
  - [Rendering steps](#rendering-steps)
  - [GLB file parsing](#glb-file-parsing)
- [AI Notice](#ai-notice)
- [References](#references)

# Report 1: Project initialization

This report gives information about the goals of the first version of the GLB Viewer application.

# Goals

Goals established during the first workshop were not all met due to setbacks. The objective will be to catch up during the next part. <br>
The goals were the following: 

- [X] Initialize the project : Have a working cpp application that can deal with 
- [X] Setup a GUI framework to develop the software (ImGui) 
- [X] Display a 3D object inside a scene
- [X] Load a GLB file and parse it display its object inside the scene
- [ ] Apply the initial transformations present in the GLB file to loaded objects
- [ ] Apply transformation on the loaded objects
- [X] Zoom the camera
- [ ] Move the camera
- [ ] Rotate the camera

The delay was caused from various reasons:
- Difficulty to understand the GLTF file format
- Difficulty to render the scene (usage of texture was necessary)

# Future work

In addition to the goals not met during the last project version, the objective of this iteration will be to load textures from the GLB archive and apply them to the loaded objects. The priorities will be the following:
1. Apply the initial transformations present in the GLB file to loaded objects
2. Move the camera
3. Rotate the camera
4. Apply transformation on the loaded objects
5. Load texture from the GLB file inside the code
6. Apply textures to the loaded objects

# Application structure

As said before, the application was developped using C++ and OpenGL. This part is more technical and will describe the code and the structure in itself to help the user find relevant information.

## Run the program

The user must download OpenGL, at least version 3.3. The program was run inside Windows and with the `g++` / `gcc` compiler. The build process was not tested on other distribution or other computer than the mine. <br>
For Windows users, the file `app.exe` was made available inside the release `0.1`. It should work on every Windows computer but let me know if it's not the case. 

Some `.glb` file can be find online to test the software, like on [pixabay](https://pixabay.com/3d-models/search/glb/)

## External dependencies

This software is programmed in C++, using various libraries, like: 
- [ImGui](https://github.com/ocornut/imgui), used as the ui framework (to display buttons, text etc...)
- [GLFW](https://www.glfw.org/), to handle the window
- [Glad](https://glad.dav1d.de/), to interact with openGL functionalities
- [GLM](https://github.com/g-truc/glm), to make 3d math easier
- KHR
- [nlohmann](https://json.nlohmann.me/), to make easier json file loading and usage

## Code structure

The application were developped using the MVC (folders `models`, `views`, `controllers`) principle. 

The project does not setup a dependency injection framework. Knwowing that, each controller is made available using the Singleton pattern.

The following code files present in the `src` folders are dependencies and were not programmed by me:
- `imgui/*`, contains the code of the ImGui lib
- `glad.c`, contains the code of the Glab lib

Everything else in the src folder were programmed by me. 

## Rendering steps

1. For each `Node`, read its `Mesh`
2. For each `Mesh`, read its `MeshPart`
3. For each `MeshPart`, read its associed `VerticeBuffer` and its `IndiceBuffer`. Send them to open GL for rendering
4. Using the default shaders that can be found inside the `/resource/shaders` folder, render the objects.
5. Tell OpenGL to render the loaded scene inside a texture
6. Tell ImGui to render an image using the generated texture. 

> It was necessary to render inside a Texture. ImGui is not able to render directly OpenGL result inside its window. Manipulate the viewport to draw on top of ImGui is not recommended.

## GLB file parsing

The `json` part of the GLB file was parsed using the `nlohmann` framework. The following actions were necessary to parse the file:
1. Locate the buffers using information provided by the json
2. Parse the buffers to interpret binary data as understandable data like floats or integers
3. Store the buffers in memory
4. Load `MeshPart` and `Mesh` using the json data. Link them their buffer using the previously loaded data
5. Instanciate on demand `Node`, that are `Mesh` with applied transformation. 

> The application use Node data to render things to the scene.

# AI Notice

AI was not used during the development of this version of the GLB Viewer application. Only usage on the project was the generation of the `pres_1.pptx` document, used to present the first version of the project in front of the class.

# References

[Learn how to use OpenGL inside of a C++ application](https://learnopengl.com/)
[GLTF specification used to parse the GLB files](https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.pdf)
[Documentation ImGui](https://pixtur.github.io/mkdocs-for-imgui/site/api-imgui/ImGui--Dear-ImGui-end-user/)
[ImGui usage](https://pthom.github.io/imgui_explorer/)
[Help to implement some concept in the application](https://vcnotebook.gitlab.io/)