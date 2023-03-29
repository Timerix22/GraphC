# GraphC - visual programming tool
When i was working on my compiler project, i understood that AST processing code isn't clear even for me because of hundreds lines of condition branches. GraphC is GUI editor of condition trees. It generates optimized AST processing code in C. 
GUI is based on [Dear ImGui](https://github.com/ocornut/imgui) and [SDL](https://github.com/libsdl-org/sdlwiki)


## Installation
1. Clone the repository
```shell
git clone --recurse-submodules https://github.com/Timerix22/GraphC
```
2. Install **SDL2** from package manager or compile it from source (read [SDL docs](SDL/docs/README.md)).  
3. Compile the program
```shell
make build_exec
```
4. Copy program files from `bin/*` to any directory
