# GLB Viewer

The objective of this application is to have a software able to open, visualize and manipulate .glb documents (an open source standard to share 3d scenes).

This software is programmed in C++, using various libraries, like: 
- [ImGui](https://github.com/ocornut/imgui), used as the ui framework (to display buttons, text etc...)
- [GLFW](https://www.glfw.org/), to handle the window
- [Glad](https://glad.dav1d.de/), to interact with openGL functionalities
- [GLM](https://github.com/g-truc/glm), to make 3d math easier
- KHR
- [nlohmann](https://json.nlohmann.me/), to make easier json file loading and usage

The implementation is developped and tested using Window + g++. Information about how to run the project is located in the doc directory: `/docs/dev/run.md`.

The project uses OpenGL 3.3. The developper should have OpenGL >= 3.3 setup on his system.