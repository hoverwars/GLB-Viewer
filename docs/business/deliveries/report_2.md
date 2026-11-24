- [Report 2: Camera and object transformations](#report-2-camera-and-object-transformations)
- [Goals](#goals)
- [Future work](#future-work)
- [Application structure](#application-structure)
  - [Run the program](#run-the-program)
  - [External dependencies](#external-dependencies)
  - [Code structure](#code-structure)
  - [Camera](#camera)
  - [Applying the initial transformations](#applying-the-initial-transformations)
  - [Object selection and visibility](#object-selection-and-visibility)
  - [Transforming the selected objects](#transforming-the-selected-objects)
  - [User interface improvements](#user-interface-improvements)
- [AI Notice](#ai-notice)
- [References](#references)

# Report 2: Camera and object transformations

This report gives information about the goals of the second version of the GLB Viewer application. It follows the [first report](report_1.md), which describes the base architecture (MVC, GLB parsing, rendering inside a texture), still valid for this version.

# Goals

The goals of this iteration were the ones listed at the end of the first report:

- [X] Apply the initial transformations present in the GLB file to loaded objects
- [X] Move the camera
- [X] Rotate the camera
- [X] Apply transformation on the loaded objects
- [ ] Load texture from the GLB file inside the code
- [ ] Apply textures to the loaded objects

The first four goals, that were late during the first delivery, are now all met. The camera can be moved and rotated, and the objects can be moved, rotated and scaled, either with the mouse or with numeric fields.

The textures were not started. The time was spent on the camera and on the transformations, which required more work than expected, mainly because of the changes of coordinate system and of the interactions between the nodes of a hierarchy (see below).

Some work that was not planned was also done to make the application easier to use: buttons to reset the camera, to display or hide the grid, the lines and the loaded objects, an icon pack, and a way to select an object by clicking on it inside the left menu.

# Future work

The objective of the last iteration will be to finish the rendering part. The priorities will be the following:
1. Load texture from the GLB file inside the code
2. Apply textures to the loaded objects
3. Let the user choose the perspective (perspective or orthographic projection)
4. Handle the light and the normals of the objects

# Application structure

This part is more technical and describes the changes made since the first version. The global structure of the application did not change: C++ and OpenGL, MVC, singleton controllers.

## Run the program

The user must have OpenGL, at least version 3.3. The program is run on Windows with the `g++` / `gcc` compiler, using the command `make` (see `/docs/dev/run.md`). The build process was not tested on other distribution or other computer than the mine.

Some `.glb` file can be found online to test the software, like on [pixabay](https://pixabay.com/3d-models/search/glb/)

## External dependencies

The dependencies are the same as the ones of the first version (ImGui, GLFW, Glad, GLM, KHR, nlohmann). The only addition is an icon pack, [Lucide](https://lucide.dev/), loaded as a font inside ImGui (`resource/fonts/lucide.ttf` and `include/imgui/IconsLucide.h`). It is used to display the icons of the buttons.

## Code structure

The following files were added or heavily modified during this iteration:
- `models/camera.*`, a new class dedicated to the camera. It was extracted from the `Scene` class, that was becoming too big
- `models/glb/node.*`, now exposes the transformations of a node (getters and setters), its father and its selection / visibility state
- `controllers/nodeHandler.*`, now able to give the nodes that are currently selected
- `views/scene/scene.*`, displays the toolbar and handles the mouse interactions
- `views/menus/leftMenu.*`, displays the list of the loaded nodes and the transformation inspector
- `init.*` and `utils/darkStyle.*`, setup of the icon pack and of the style of the selected nodes

## Camera

The camera orbits around a target point. Its position is not stored directly, it is computed every frame from the following values:
- the `target`, the point looked at
- the `distance` between the camera and its target
- the `yaw` and `pitch` angles (the pitch is limited to ±89° to avoid flipping the camera)

The camera is controlled with the mouse, when the cursor is above the scene:

| Action | Result |
| --- | --- |
| Mouse wheel | Zoom (the distance is multiplied by 0.9 for each notch, so it feels the same at any distance). The distance is limited between 1 and 200 |
| Middle button drag | Rotate the camera around its target |
| Shift + middle button drag | Move the target along the right and up axes of the camera |

When the camera is moved, the target follows the cursor exactly. For that, the distance covered by a pixel on the screen at the depth of the target is computed using the field of view of the camera (45°).

A drag starts only if the cursor is above the scene, but it continues even if the cursor leaves it.

The view matrix is computed using `glm::lookAt`, and the projection with `glm::perspective`. The camera also keeps its `right` and `up` axes, to be used by the object transformations.

> glTF files are Y-up, but the application uses Z-up (like in Blender). A rotation of 90° around the X axis is applied to the loaded models so they stand on the grid.

## Applying the initial transformations

A GLB file stores the translation, the rotation (as a quaternion) and the scale of every node. The `Node` class now applies them when rendering: the matrix of a node is the matrix of its father multiplied by its own translation, rotation and scale. The children are therefore moved with their father.

The first goal of the first report was reached this way, and it is also the base of the transformations made by the user: transforming a node only means changing the values stored inside the `Node`.

## Object selection and visibility

The left menu displays the nodes as a tree. For each node, the user can:
- click on its name to select it. The selected nodes are highlighted in the menu
- click on an icon to hide / display it. Hiding a node also hides its children

The `NodeHandler` is able to return the *selected roots*: the selected nodes whose father is not selected. As a father moves its children, transforming a child at the same time would apply the transformation twice. Only the roots are transformed.

## Transforming the selected objects

The transformations are done in two ways.

**With the mouse.** The toolbar of the scene contains four modes: selection, move, scale and rotate. In the three last modes, dragging with the left button transforms all the selected objects:

| Mode | Drag | Keys |
| --- | --- | --- |
| Move | In the plane of the camera. The object follows the cursor, whatever its distance to the camera | `Shift`: vertical movement only |
| Scale | Up / down, the object grows / shrinks (limited to a factor 2 per frame) | |
| Rotate | Left / right turns around the vertical axis, up / down turns around the horizontal axis of the screen | |

In each mode, holding `X`, `Y` or `Z` limits the transformation to this axis of the world.

The mouse movements are expressed in the world coordinates (camera axes, Z-up), but the transformations are stored inside the nodes in the space of their father. The movement is therefore converted using the inverse of the matrix of the father of the node. The rotations are combined using quaternions, and the scale constrained to an axis is applied on the axis of the node that is the closest of the one requested.

**With the numeric inspector.** At the bottom of the left menu, the position, the rotation (in degrees) and the scale of the first selected node can be edited using drag fields. It allows precise values that are hard to get with the mouse.

## User interface improvements

- Button to reset the camera, highlighted when the camera is already centered
- Display / hide the grid and the lines of the objects
- Icons instead of text for the buttons
- Dark style for the selected nodes

# AI Notice

AI was used during the development of this version of the GLB Viewer application. The transformation of the objects (mouse interactions inside `Scene::handleTransformInput`, the numeric inspector inside `LeftMenu::renderTransformInspector`, and the associated getters / setters in `Camera`, `Node` and `NodeHandler`) were generated using Claude Code, in case I did not have the time to finish this objective by myself (commit `b9598cc`).

The camera, the selection, the visibility, the toolbar and the initial transformations were programmed without AI.

AI was also used to generate the `report_2.md` and the `pres_2.pptx` documents. The slideshow uses a template from [Slidesgo](https://slidesgo.com/).

# References

[Learn how to use OpenGL inside of a C++ application](https://learnopengl.com/)
[GLTF specification used to parse the GLB files](https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.pdf)
[Documentation ImGui](https://pixtur.github.io/mkdocs-for-imgui/site/api-imgui/ImGui--Dear-ImGui-end-user/)
[ImGui usage](https://pthom.github.io/imgui_explorer/)
[Lucide icons](https://lucide.dev/)
[Help to implement some concept in the application](https://vcnotebook.gitlab.io/)
