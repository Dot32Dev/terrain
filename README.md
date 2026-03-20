# Terrain
## Build from fresh Git clone instructions
To build, make a folder in the project directory named third_party, inside 
create a folder named GLFW, download GLFW binaries and copy your version of 
visual studio's folder's contents into your GLFW folder. Add its include 
directory to your project. Similarly with GLAD, generate a GLAD for 3.3 core 
profile, and copy its files into a GLAD subfolder of the third_party folder.
Addionally download and copy glm into third_party. Compile in Visual Studio 2022