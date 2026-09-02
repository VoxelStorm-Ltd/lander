# Building and running

Configure, build, and run a 64-bit Release build with:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_32BIT=OFF
cmake --build build --parallel
./build/Lander
```

To switch the same build directory to Debug mode, reconfigure it with `CMAKE_BUILD_TYPE=Debug` and rebuild:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_32BIT=OFF
cmake --build build --parallel
./build/Lander
```

`CMAKE_BUILD_TYPE` accepts `Debug` or `Release` and defaults to `Release` when omitted. `BUILD_32BIT` selects the target architecture: it defaults to the host architecture, `ON` enables `-m32` and the `lib/<platform>32` libraries, and `OFF` enables `-m64` and the `lib/<platform>64` libraries. For example, build 32-bit Release with:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_32BIT=ON
cmake --build build --parallel
./build/Lander
```

On Windows, run `build/Lander.exe` instead.
