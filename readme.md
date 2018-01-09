## How to use

### Windows

Use the ```VsDevCmd.bat``` with ```-arch=x64```.

    git clone --recursive -j8 https://github.com/marcgpuig/Andes.git
    cd Andes && mkdir build && cd build
    cmake -G "NMake Makefiles" ..
    nmake && .\Andes.exe

*-j8 will only work with version 1.9 or later of Git*

If you already cloned the repo:

    cd Andes
    git submodule update --init --recursive
    mkdir build && cd build
    cmake -G "NMake Makefiles" ..
    nmake && .\Andes.exe