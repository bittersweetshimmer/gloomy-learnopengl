# gloomy-learnopengl
LearnOpenGL tutorials made with [`gloomy`](https://github.com/bittersweetshimmer/gloomy).

##### Build
```sh
> git submodule update --init --recursive # pull the dependencies
> mkdir build
> cd build
> cmake .. -D GLOOMY_DEBUG:BOOL=TRUE # error checking on each OpenGL call
> make -j8 # or `msbuild gloomy_learnopengl.sln /m` if you're using Visual Studio 
```

### 3rd party code
- [GLFW](https://www.glfw.org/) - OpenGL library - `zlib/libpng`
- [glm](https://glm.g-truc.net/0.9.9/index.html) - Linear algebra - `MIT`
- [stb](https://github.com/nothings/stb) - Utilities - Public domain / `MIT`
- [OBJ-Loader](https://github.com/Bly7/OBJ-Loader) - Loading .obj files (sorry [assimp](https://github.com/assimp/assimp), you seem too bloated to me) - `MIT`

### Acknowledgments
- [LearnOpenGL](https://learnopengl.com/) - great tutorials on modern OpenGL.
