# GraphC - visual programming tool
When i was working on my compiler project, i understood that AST processing code isn't clear even for me because of hundreds lines of condition branches. GraphC is GUI editor of condition trees. It generates optimized AST processing code in C. 
GUI is based on [Dear ImGui](https://github.com/ocornut/imgui) and [SDL](https://github.com/libsdl-org/sdlwiki)


## Installation
1. Clone the repository
```shell
git clone --recurse-submodules https://github.com/Timerix22/GraphC
```
2. Install **SDL2** from package manager or compile it from source.  
    **If you are using msys, switch to mingw64 shell.**
    ```shell
    cd dependencies/SDL2
    ./configure
    make -j [number of cpu threads]
    ```
    Then you can install it systemwide: 
    ```shell
    make install
    ```
    or copy to ./libs: 
    ```shell
    mkdir -p ../../libs
    cp ./build/.libs/SDL2.dll ../../libs/
    ```
    If it didn't work, read [SDL docs](dependencies/SDL2/docs/README.md) and [INSTALL.txt](dependencies/SDL2/INSTALL.txt).  

3. Compile the program
```shell
make build_exec
```

4. Copy program files from `bin/*` to any directory
