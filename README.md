# StarDonk

Free Windows VST3 hardbass / donk synthesizer made by StarSlav.

[![Download StarDonk](https://img.shields.io/badge/Download-StarDonk-yellow?style=for-the-badge)](https://github.com/starslavhardbass/StarDonk/releases/download/v1.0.1/StarDonk-v1.0.1-Windows-x64-VST3.zip)

**Windows 64-bit VST3**

**Current version: v1.0.1**

[View latest release](https://github.com/starslavhardbass/StarDonk/releases/latest)

![StarDonk](docs/stardonk-default.png)

## Demo

[![Watch StarDonk Demo](docs/stardonk-video.png)](https://www.youtube.com/watch?v=qcPkwo0MUIg)

## What's new in v1.0.1

- Added automatic donk root detection
- Added live Detected Root display
- Added Root Note control
- Added Match Root option
- Match Root instantly matches the current detected root
- Turning Match Root off returns Root Note to C
- Random Donk resets Root Note to C unless Match Root is enabled
- Added octave control for moving donks up or down while staying in the same key
- Added Freeze Octave option
- Random Donk resets octave to 0 unless Freeze Octave is enabled
- Added WAV export
- WAV exports use the current preset name, or a random StarDonk name
- Detected root is added to exported WAV filenames
- WAV exports now automatically trim unnecessary silence
- Reverb tails are preserved while they are still audible
- Improved FL Studio computer-keyboard support
- Added version display to the Help window
- Added manual GitHub update checker
- Improved Help window scrolling and layout
- Improved compatibility with older .donk presets
- Various fixes and UI improvements

## Features

- Classic donk / hardbass sound
- 8 main macro controls
- Advanced FM engine
- Random donk generator
- Reverb
- Sidechain / ducking
- Declicker
- Preset browser
- Minimalistic mode
- Experimental controls
- Custom face images

## Advanced Mode

![Advanced Mode](docs/stardonk-advanced.png)

Advanced Mode opens the deeper synth engine with FM operators, envelopes,
sub controls, filter resonance, distortion mix and more.

## Minimalistic Mode

![Minimalistic Mode](docs/stardonk-minimal.png)

A clean black-and-white interface with raw parameter values.

## Minimalistic + Advanced

![Minimalistic Advanced](docs/stardonk-minimal-advanced.png)

Advanced controls can also be used together with Minimalistic Mode.

## Presets

![Preset Browser](docs/stardonk-presets.png)

StarDonk supports `.donk` presets and preset folders.

You can create your own folders inside the preset directory and sort your donks however you want.

## Requirements

- Windows 64-bit
- VST3-compatible DAW

## Installation

Place:

`StarDonk.vst3`

in:

`C:\Program Files\Common Files\VST3`

Place presets in:

`Documents\StarDonk\Presets`

Then restart or rescan plugins in your DAW.

## Building from source

StarDonk is written in C++ using JUCE.

You will need:

- JUCE
- Visual Studio

Open:

`StarDonk.jucer`

Generate/open the Visual Studio project and build the VST3 target using:

`Release x64`

## License

StarDonk may be modified and redistributed free of charge.

Selling StarDonk or modified versions is not permitted.

Credit to StarSlav is appreciated, but not required.

See `LICENSE` for the full terms.

## StarSlav

YouTube: https://youtube.com/@starslav

Instagram: https://instagram.com/starslavhardbass
