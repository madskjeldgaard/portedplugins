[![.github/workflows/cmake.yml](https://github.com/madskjeldgaard/mkplugins/actions/workflows/cmake.yml/badge.svg)](https://github.com/madskjeldgaard/mkplugins/actions/workflows/cmake.yml)

# MKPlugins

A collection of SuperCollider plugins by Mads Kjeldgaard.

Most of these are algorithms that I felt like exploring more in-depth at a C++ level to understand them better or simply found missing in the SuperCollider plugin landscape.


## Included plugins

See help files for more information on these, included links to sources/research where applicable:

**Chen**

Chen's chaotic double scroll attractor. Inspired by Bryan Head's implementation in the Mutable Instruments Stages alternative firmware.

**LPG**

A digital model of the Buchla Lowpass-Gate. Based on a paper/research by Julian Parker and Stefano D'Angelo.

**Rongs**

A modal synthesis voice based on the algorithms of Mutable Instruments' modules. It's sonic territory spans tired spring reverb, plastic tubes, metallic percussion and plucked strings.

### Installation
1. Download one of [the prebuilt releases](https://github.com/madskjeldgaard/mkplugins/releases). Choose the one that fits your operating system.
2. Unzip the contents in your extensions directory (If you don't know where this is, open up SuperCollider and evaluate `Platform.userExtensionDir` - the path will be posted in the post window).
3. Recompile class library.

### Building

### Requirements

- CMake >= 3.5
- SuperCollider source code


#### Preparations for a build
Clone the project:

    git clone https://github.com/madskjeldgaard/mkplugins
    cd mkplugins

This will get you the mkplugins.

Before continuing, you need two things: 

First, the [SuperCollider source code](github.com/supercollider/supercollider/) :

```bash
git clone https://github.com/supercollider/supercollider.git
```

Note where you placed this source code on your computer and copy the full path to it for use with CMake.

Secondly, you need the path to your SuperCollider extensions directory. This is where CMake will install the plugins.

You can get this by opening up SuperCollider and running this line of code

``` 
Platform.userExtensionDir
```

Note the path that it posts to your post window in SuperCollider, copy it and save it for the next part of the build process.


#### Actually Building
To build the plugins, run these commands. Note the two paths you need to put in to the CMake command, these are the ones you got from the preparatory steps above.

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE='Release' -DSC_PATH=/path/to/sc/source -DCMAKE_INSTALL_PREFIX=/path/to/extensions
cmake --build . --config Release
cmake --build . --config Release --target install
```
#### Building for Raspberry Pi

This repository includes a script for the Raspberry Pi users (Raspberry Pi OS based) that installs the prerequisites for building (cmake), downloads all of the source code needed and compiles and installs it. 

