# Changelog

## [0.3.0] - 2021-09-11

### Bug Fixes

- VadimFilter resonance range changed to 0.0 to 1.0.

### Documentation

- Add guidelines/info for contributions
- Add section on how to create a release
- Add section on memory to contribution guidelines
- Add TOC
- Add breaking change to commity style guide
- Fix examples so they use frequency for f0 param

### Features

- Add LockhartWavefolder
- Add NonlinearFilter
- Add VA1Pole, VADiodeFilter, VAKorg35, VALadder, VASEM12
- Optimize tan function in odin2 filters
- Rename the project from MKPlugins to PortedPlugins

### Build

- Streamline github actions
- Use git cliff in github actions
- Make add_plugin script append plugin name to overview help file
- Remove eigen as dependency
