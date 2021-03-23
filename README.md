[![.github/workflows/cmake.yml](https://github.com/madskjeldgaard/mkplugins/actions/workflows/cmake.yml/badge.svg)](https://github.com/madskjeldgaard/mkplugins/actions/workflows/cmake.yml)

# MKPlugins

A collection of SuperCollider plugins by Mads Kjeldgaard.

Most of these are algorithms that I felt like exploring more in-depth at a C++ level to understand them better or simply found missing in the SuperCollider plugin landscape.

## Included plugins

See the overview helpfile [MKPlugins](/plugins/HelpSource/Overview/MKPlugins.schelp) for more information which plugins are included.

## Installation

**Instructions for most systems**:
1. Download one of [the prebuilt releases](https://github.com/madskjeldgaard/mkplugins/releases). Choose the one that fits your operating system.
2. Unzip the contents in your extensions directory (If you don't know where this is, open up SuperCollider and evaluate `Platform.userExtensionDir` - the path will be posted in the post window).
3. Recompile class library.

**Arch Linux**:

`yay -S supercollider-mkplugins`

**RaspberryPi OS**:

See the accompanying script for downloading, building and installing the plugins.

### Building

### Requirements

- CMake >= 3.5
- SuperCollider source code


#### Preparations for a build
Clone the project:

	git clone https://github.com/madskjeldgaard/mkplugins
	cd mkplugins
	git submodule update --init --recursive

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


# Credits
Most of these plugins build on code, research and ideas of others, including:

- [Mutable Instruments / E. Pichenettes](https://mutable-instruments.net/)
- Julian Parker and Stefano D'angelo (LPG research)
- [ElectroSmith's DaisySP](https://github.com/electro-smith/DaisySP) DSP library
- Bryan Head (Chen attractor)
- [Jatin Chowdhury](https://https://ccrma.stanford.edu/~jatin/)


A massive thank you to them for sharing their knowledge and ideas and making new sounds possible! 

You can find more information about sources of information and code in the help files for the plugins.
