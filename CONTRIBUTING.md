# Contributing

All contributions to this project are very welcome. 

A contribution can be many things: Opening an issue or a feature request (or a port request - have you seen some cool open source dsp algorithm that we should port? Let us know! ) or committing code or documentation. If you have a cool example that you would like to spread to the world, feel free to open up a pull request and add it to the relevant help file. The more examples the better!

## Commit message style

We use [conventional commit style messages](https://www.conventionalcommits.org/en/v1.0.0/) for the commit messages. The reason for this is that it makes it easy for tools to automatically generate changelogs and other nice things. 

For example, let's say you added oversampling to *Rongs*, a commit message might look like: `feat(Rongs): Added oversampling` or a bug fix: `fix(Rongs): Fixed oversampling`. 

Documentation changes (either in the Readme or the schelp files) should look like this: `doc(Rongs): Changed example`.

## Adding a plugin

First, make sure you are on a new branch:

```bash
git checkout -b topic/SuperPlugin
```
Adding a new plugin is fairly simple: This repository comes with a bash script for adding a plugin called [add_plugin](scripts/add_plugin). 

It's really simple to use. From the root of the repo folder, run:

```bash
./scripts/add_plugin SuperPlugin
```

This will create a folder in `plugins` called `SuperPlugin` containing C++ files, an SC file and a help file. The script will also add the plugin to [CMakeLists.txt](CMakeLists.txt) so the compiler will pick it up properly. 

The only thing you need to manually do is to open the overview help file [plugins/HelpSource/Overview/MKPlugins.schelp](plugins/HelpSource/Overview/MKPlugins.schelp) and add it to the list (TODO: this should be automated).

### Including extra header/implementation files
Note: If your plugin uses extra header files/C++ files, you need to add these to the CMakelists entry for your plugin. As an example, here is the LockhartWavefolder plugin's cpp files entry. `LockhartCell.cpp` and `LockhartCell.hpp` are used in the main plugin files `LockhartWavefolder.cpp` and `LockhartWavefolder.hpp`: 

```CMake
# LockhartWavefolder
set(LockhartWavefolder_cpp_files
	plugins/LockhartWavefolder/LockhartCell.cpp
	plugins/LockhartWavefolder/LockhartCell.hpp
	plugins/LockhartWavefolder/LockhartWavefolder.cpp
	plugins/LockhartWavefolder/LockhartWavefolder.hpp
	)
```
For more information on how to write plugins, see [this tutorial on the SCSynth forum](https://scsynth.org/t/tutorial-supercollider-server-plugins-in-c/3449).

## Renaming a plugin

If you regret the name of your plugin, the repo also includes a script for renaming plugins like so `rename_plugin <old_plugin_name> <new_plugin_name>`:

```bash
./scripts/rename_plugin SuperPlugin SuperCoolPlugin 
```

