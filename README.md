[![.github/workflows/cmake.yml](https://github.com/madskjeldgaard/mkplugins/actions/workflows/cmake.yml/badge.svg)](https://github.com/madskjeldgaard/mkplugins/actions/workflows/cmake.yml)

# MKPlugins

A collection of plugins for the [SuperCollider sound environment](https://supercollider.github.io/).

## Included plugins

See the `MKPlugins.schelp` help file for an overview inside of SuperCollider and the respective help files for each plugin for more information about the particularities of each plugin.

- **AnalogBassDrum** - Virtual analog 808 bass drum model. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **AnalogPhaserMod** - The modulation section of a phaser based on/inspired by classic krautrock phasers. Ported from [Jatin Chowdhury's ChowDSP-VCV-rack project](https://github.com/jatinchowdhury18/ChowDSP-VCV).
- **AnalogPhaser** - Virtual analog feedback phaser based on/inspired by classic krautrock phasers. Ported from [Jatin Chowdhury's ChowDSP-VCV-rack project](https://github.com/jatinchowdhury18/ChowDSP-VCV).
- **AnalogSnareDrum** - Virtual analog 808 snare drum model. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **AnalogTape** - Virtual analog tape model with variable oversampling and anti aliasing. Ported from [Jatin Chowdhury's ChowDSP-VCV-rack project](https://github.com/jatinchowdhury18/ChowDSP-VCV).
- **AnalogVintageDistortion** - A virtual analog vintage EQ and distortion by Jatin Chowdhury. Features oversampling. Ported from [Jatin Chowdhury's ChowDSP-VCV-rack project](https://github.com/jatinchowdhury18/ChowDSP-VCV).
- **AnalogFoldOsc** - Buchla type of Oscillator with built in wavefolder circuit. Based on research by Fabián Esqueda, Henri Pöntynen, Vesa Välimäki and Julian D. Parker.
- **BLOsc** - Band limited oscillator. Original Author(s): Paul Batchelor, saw2 Faust by Julius Smith. Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **Chen** - Chen's chaotic double scroll attractor, based on code from [Bryan Head's alternative Mutable Instruments firmware](https://github.com/qiemem/eurorack/releases/tag/v1.1.0).
- **DCompressor** - Compressor from [DaisySP DSP library](https://github.com/electro-smith/DaisySP), ported from faust originally by shensley andAvAars originally.
- **HarmonicOsc** - 16 voice harmonic oscillator. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **LPG** - Virtual analog Buchla Lowpass-gate based on the work of Julian Parker and Stefano D'angelo.
- **NeoFormant** - Formant oscillator with aliasing-free phase reset. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **NeoVarSawOsc** - Variable saw oscillator. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **OscBank** - A mixture of 7 sawtooth and square waveforms in the style of divide-down organs. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **PhasorModal** - Ported from [Jatin Chowdhury's ChowDSP-VCV-rack project](https://github.com/jatinchowdhury18/ChowDSP-VCV).
- **Resonator** - A resonant body simulation (originally found in the Mutable Instruments Rings synthesizer module). Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **Rongs** - A modal synthesis voice. This is a remix of the Mutable Instruments Rings algorithm allowing unrealistic sounds of sloppy gongs and 100km long strings. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack).
See the overview helpfile [MKPlugins](/plugins/HelpSource/Overview/MKPlugins.schelp) for more information which plugins are included.
- **StringVoice** - Extended Karplus-Strong. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **VadimFilter** - Virtual analog low pass, high pass and band pass filters based on the [ground breaking work of Vadim Zavalishin](https://www.native-instruments.com/fileadmin/ni_media/downloads/pdf/VAFilterDesign_1.1.1.pdf). Ported from  [folderkit](https://codeberg.org/nonmateria/folderkit) 
- **VarShapeOsc** - Variable Waveshape Oscillator. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **VosimOsc** - Vosim oscillator (Two sinewaves multiplied by and sync'ed to a carrier). Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 
- **ZOsc** - Sinewave multiplied by and sync'ed to a carrier. Original code by [Èmilie Gillet / Mutable Instruments](https://github.com/pichenettes/eurorack). Revisited by Ben Sergentanis for the [DaisySP DSP library](https://github.com/electro-smith/DaisySP). 

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


# Contributing
Contributions are welcome! 

If you experience any problems, post it as an issue or if you have the skills to fix it yourself you may open up a PR with a suggested change. 

If you see any problems in the documentation, feel free to do the same here (please submit a PR if you can - otherwise feel free to post an issue). 

See [CONTRIBUTING.md](CONTRIBUTING.md) for more information on how to contribute.

# Credits
Thank you to [Notam](https://notam.no) for the support. 

Most of these plugins build on code, research and ideas of others, including:

- [Mutable Instruments / E. Pichenettes](https://mutable-instruments.net/)
- Julian Parker and Stefano D'angelo (LPG research)
- [ElectroSmith's DaisySP](https://github.com/electro-smith/DaisySP) DSP library
- Bryan Head (Chen attractor)
- [Jatin Chowdhury](https://https://ccrma.stanford.edu/~jatin/)


A massive thank you to them and everyone who contributed to the sub projects this relies on for sharing knowledge and ideas and making new sounds possible! 

You can find more information about sources of information and code in the help files for the plugins.
