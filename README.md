# Description pg3d
 3d triangle-projection grahics made in C/C++ with WinAPI
 
## Controls

esc - read config and reset
z - flip between projection modes
x - flip between wireframe and surface
v - flip between colorblending methods

Up Left Right Down arrows - move camera around the body center
i - move closer (does not redraw on itself, use for example i and then Left)
k - move further (same)

1 - load cube
2 - load sphere 1
3 - load sphere 3
4 - load sphere 5

## Notes
zbuffer was not used
Absolute movement was partially implemented, but not linked to lighting ,thus looks wrong (QWERASDF - for movement)


## building

- C++ Compiler: C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin\g++.exe
- Command: g++.exe "C:\Users\NIK\Documents\_STU\_sem4\pg3d\main.cpp" -o "C:\Users\NIK\Documents\_STU\_sem4\pg3d\main.exe" -g3 -I"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\include" -I"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\x86_64-w64-mingw32\include" -I"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\lib\gcc\x86_64-w64-mingw32\9.2.0\include" -I"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\lib\gcc\x86_64-w64-mingw32\9.2.0\include\c++" -L"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\lib" -L"C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\x86_64-w64-mingw32\lib" -static-libgcc -lgdi32 -lcomdlg32 -g3

Compilation results...

- Errors: 0
- Warnings: 0
- Output Filename: C:\Users\NIK\Documents\_STU\_sem4\pg3d\main.exe
- Output Size: 5,04834747314453 MiB
- Compilation Time: 3,36s