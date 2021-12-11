# OceanTracer
Tiny ocean raymarcher running on the cpu. 

## Building

OceanTracer uses [TBB](https://github.com/oneapi-src/oneTBB) to manager parallelism and [GLEW](http://glew.sourceforge.net/) to load OpenGL functions.
Both can easily be built using [Vcpkg](https://github.com/microsoft/vcpkg).
```bat
vcpkg install tbb:x64-windows
vcpkg install glew:x64-windows
```
You can build the project with [CMake](https://cmake.org/), you will need to have [Vcpkg](https://github.com/microsoft/vcpkg) installed on your computer.

```bat
git clone https://github.com/romainaugier/SphereTracer.git
cd SphereTracer
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/toolchain/vcpkg.cmake 
cmake --build ./build --config Release
./build/src/Release/OceanTracer.exe
```

It has been tested and runs fine on Windows 10 and Ubuntu 20.04.
