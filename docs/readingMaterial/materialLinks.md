# Reading Material links

## openGL:
- [learn openGL video-series](https://www.youtube.com/watch?v=XpBGwZNyUh0&list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-)
- [learn openGL website](https://learnopengl.com/Advanced-OpenGL/Geometry-Shader)
- [OpenGL Docs](https://docs.gl/)
- [opengltutorials] (https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/)

## glfw
-[glfw docs](https://www.glfw.org/docs/latest/)
-[glfw input handling](https://www.glfw.org/docs/latest/input_guide.html)

## Terrain Generation
- [Terrain Generation with perlin noise](https://www.redblobgames.com/maps/terrain-from-noise/)
- [Polygonal map generation](http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/)

- [3d map generation video-series](https://www.youtube.com/watch?v=U9q-jM3-Phc)
- [Game engine tutorials in java](https://www.youtube.com/watch?v=mnIQEQoHHCU&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=32)

- [Object placement using poisson disk sampling](http://devmag.org.za/2009/05/03/poisson-disk-sampling/)

## Skybox and cubemaps
- [Skybox](https://learnopengl.com/Advanced-OpenGL/Cubemaps)

## Quick debug GUI
- [imGui](https://github.com/ocornut/imgui/wiki/Getting-Started)

# PBR 
- [PBR THEORY](https://learnopengl.com/PBR/Lighting)
- [PBR CODE](https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/1.2.lighting_textured/1.2.pbr.fs)

## Vscode and general code
- [debug](https://www.youtube.com/watch?v=-tGSO5-eRRg)
- [debug cmake](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/debug-launch.md)
- [documenting better](https://developer.lsst.io/cpp/api-docs.html)
- triple /// will make fast doxygen style comments
- [enums not to behave as globals](https://stackoverflow.com/questions/4269365/do-enum-values-behave-like-global-variables) enum class has no implicit conversion which is annoying, but using enum as a struct member is fine!

- [Virtuals and pure Virtual classes](https://stackoverflow.com/questions/2652198/difference-between-a-virtual-function-and-a-pure-virtual-function) I found that in pure virtual i dont need the ovveride keyword and that [i should use const = 0; if they dont modify the class](https://stackoverflow.com/questions/21187965/c-code-explanation-for-method-prototype-with-const-0)