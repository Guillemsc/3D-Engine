# SuSto Engine
[![Build status](https://ci.appveyor.com/api/projects/status/m3lluga340u8kfuk/branch/master?svg=true)](https://ci.appveyor.com/project/Guillemsc/3d-engine/branch/master)

3D Engine made by Guillem Sunyer Caldú and Simón Stoyanov Beltrán for the 3D engines subject during the 3rd year of Game Design and Game Development degree at CITM.

Link to the Github Repository ![https://github.com/Guillemsc/3D-Engine](https://github.com/Guillemsc/3D-Engine)

Link to the **Guillem Sunyer**'s Github page ![https://github.com/Guillemsc](https://github.com/Guillemsc/)

Link to the **Simón Stoyanov**'s Github page ![https://github.com/SimonStoyanov](https://github.com/SimonStoyanov)


## Game Engine Instructions
### Main Menu Bar
* **File**
  * _**Open in Explorer**_: Opens the container of the Engine.
  * _**Quit (Alt + F4)**_: Exits the application.
* **Game Object**
  * _**Create Empty**_: Creates an empty GameObject and puts it into the hierarchy.
* **Component**
It will only show the options if a GameObject of the hierarchy is selected.
  * _**Add Mesh**_: Adds a component mesh to the selected GameObject.
  * _**Add Texture**_: Adds a component texture to the selected GameObject.
* **Window**
  * _**Game**_
  * _**Hierarchy**_
  * _**Inspector**_
  * _**Console**_
  * _**Configuration (C)**_
  * _**Profiler (P)**_
  * _**Hardware**_
* **Help**
  * _**About SuSto Engine**_
* **Layouts**
  * _**Current Layout**_: Shows the layout that is using the engine at the moment.
  * _**List of Layouts**_: A list of layouts that can be assigned when clicking the _Set_ Button or be deleted with the _X_ Button.
  * _**Input Layout**_: By clicking the button _Save Layout_ and assigning a name at the text input, a layout will be added to the list of layouts above.
* **Debug**
  * _**Engine Test**_: Opens the engine test window.
  * _**Test Window**_: Opens the imgui test window.
* **FPS**: Show the current framerate.

### Game
* **Camera Movement**
  * _**Right Click + W, A, S, D**_: Move **forward**, **Left**, **backward** or **right**, respectively.
  * _**Right Click + Z, X**_: Move **up** or **down**, respectively
  * _**Mouse Scroll**_: Move the camera forward or backwards.
  * _**Alt + Left Click + Mouse Movement**_: Camera orbits the object.
  * _**Right Click + Mouse Movement**_: Camera moves.
  * _**F**_: Centers the view to the mesh.
* **Other functionalities**
  * _**Drag and Drop**_: if an fbx is dropped into the engine, it will be rendered. A PNG texture can be dropped to.
 
### Hierarchy


## License
MIT License

Copyright (c) 2017 Guillem Sunyer Caldú and Simon Stoyanov Beltran

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
