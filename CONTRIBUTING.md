# Contributing


All contributions to this project are very welcome. 

A contribution can be many things: Opening an issue or a feature request (or a port request - have you seen some cool open source dsp algorithm that we should port? Let us know! ) or committing code or documentation. If you have a cool example that you would like to spread to the world, feel free to open up a pull request and add it to the relevant help file. The more examples the better!

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [Commit message style](#commit-message-style)
- [Adding a plugin](#adding-a-plugin)
  - [Including extra header/implementation files](#including-extra-headerimplementation-files)
- [Renaming a plugin](#renaming-a-plugin)
- [Creating a release](#creating-a-release)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Commit message style

We use [conventional commit style messages](https://www.conventionalcommits.org/en/v1.0.0/) for the commit messages. The reason for this is that it makes it easy for tools to automatically generate changelogs and other nice things. 

For example, let's say you added oversampling to *Rongs*, a commit message might look like: `feat(Rongs): Added oversampling` or a bug fix: `fix(Rongs): Fixed oversampling`. 

Documentation changes (either in the Readme or the schelp files) should look like this: `doc(Rongs): Changed example`.

If something is a breaking change, it may be suffixed with an exclamation mark: `feat(Rongs)!: Use imperial measurements for frequency parameter`. 

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

### Realtime safe memory

From the [SuperCollider documentation](http://doc.sccode.org/Guides/WritingUGens.html):

> Do not allocate memory from the OS via malloc / free or new/ delete. Instead you should use the real-time memory allocator via RTAlloc / RTFree.

This is super important when porting code from other environments that involve memory allocation/deallocation. For example reverbs, delay based stuff etc. It is definately a challenge to make the memory realtime safe but it is definately worth it because it makes the plugins really robust (and allows the user to instantiate the plugins while livecoding without experiencing dropouts, glitches or xruns caused by memory allocation).

## Renaming a plugin

If you regret the name of your plugin, the repo also includes a script for renaming plugins like so `rename_plugin <old_plugin_name> <new_plugin_name>`:

```bash
./scripts/rename_plugin SuperPlugin SuperCoolPlugin 
```

## Creating a release

We use [semantic versioning](https://semver.org/) for the version numbers. Once all commits are ready to go on the `main` branch, a release may be created automatically by creating a new tag for that version.

Example:

```bash
git tag v0.2.8
git push --tags
```

When pushing a new version tag, a [github action will run](https://madskjeldgaard.dk/posts/how-to-gh-action/) and compile plugins for all platforms, zip them up and create a release.
