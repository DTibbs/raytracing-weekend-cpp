raytracing-weekend-cpp
======

Port of my Rust port of the book [Ray Tracing in One Weekend](https://github.com/petershirley/raytracinginoneweekend) back to C++.

The output of the program is a PPM formatted image echo'ed to stdout via `println!`. To get a file, just pipe stdout to file. Gimp can open PPM result.

Building
=======
Build solution in Visual Studio (2019 Community Edition tested). .exe will be built to x64/Debug or x64/Release depending on which you have selected in the IDE.

Running
=======
`x64/<Debug|Release>/raytracing-weekend-cpp.exe > test.ppm`