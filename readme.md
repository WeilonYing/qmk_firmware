# Bad Apple!! but on a QMK keyboard
YouTube demo: https://www.youtube.com/watch?v=6KbRA2RjhgQ

If you're looking for the source code, check it out here: https://github.com/WeilonYing/qmk_firmware/commit/1888d3eab2a019734eba190837361ce37ff13432

## How this works
QMK supports custom animation cycles written in C. At a high level, this simply means that the keyboard can play the animation as long as we can convert it into something it can read. My process was as follows:
1. I took the bad apple video, downsampled and cropped it to 20x6 at 10fps using the ffmpeg tool.
2. Wrote a python script and used opencv to convert it into an array of 20x6 boolean matrices, and had it spit that out as predefined constant in a C header file. You can find the [script here](https://github.com/WeilonYing/qmk_firmware/commit/1888d3eab2a019734eba190837361ce37ff13432#diff-1d56b7a1b91f493e1ee5b6338842634b4a5afa0b9a85893f7a65c86e3de49390), and my [outputted header file here](https://github.com/WeilonYing/qmk_firmware/commit/1888d3eab2a019734eba190837361ce37ff13432#diff-7bdc675226110936f146df04388507d46f38c02a98abc3e53b4837b164fd759d).
3. Modified a pre-existing animation runner on the firmware to make the max counter the number of frames in the header file, then stuffed the whole thing into a 226kb compiled binary
4. Flashed the keyboard with the new compiled firmware 

The Massdrop CTRL keyboard has about 256kb of program memory, and this was its main limitation on the resolution and framerate of the video. The video frames could probably be compressed and optimised further, but you'd probably also need to implement a decompression algorithm in the firmware, and I didn't have time to do that.

Feel free to take this code and improve upon it or adapt it for your own use. But I do ask that you credit me if you do so.

*Original QMK documentation below*

---

# Quantum Mechanical Keyboard Firmware

[![Current Version](https://img.shields.io/github/tag/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/tags)
[![Discord](https://img.shields.io/discord/440868230475677696.svg)](https://discord.gg/Uq7gcHh)
[![Docs Status](https://img.shields.io/badge/docs-ready-orange.svg)](https://docs.qmk.fm)
[![GitHub contributors](https://img.shields.io/github/contributors/qmk/qmk_firmware.svg)](https://github.com/qmk/qmk_firmware/pulse/monthly)
[![GitHub forks](https://img.shields.io/github/forks/qmk/qmk_firmware.svg?style=social&label=Fork)](https://github.com/qmk/qmk_firmware/)

This is a keyboard firmware based on the [tmk\_keyboard firmware](https://github.com/tmk/tmk_keyboard) with some useful features for Atmel AVR and ARM controllers, and more specifically, the [OLKB product line](https://olkb.com), the [ErgoDox EZ](https://ergodox-ez.com) keyboard, and the [Clueboard product line](https://clueboard.co).

## Documentation

* [See the official documentation on docs.qmk.fm](https://docs.qmk.fm)

The docs are powered by [Docsify](https://docsify.js.org/) and hosted on [GitHub](/docs/). They are also viewable offline; see [Previewing the Documentation](https://docs.qmk.fm/#/contributing?id=previewing-the-documentation) for more details.

You can request changes by making a fork and opening a [pull request](https://github.com/qmk/qmk_firmware/pulls), or by clicking the "Edit this page" link at the bottom of any page.

## Supported Keyboards

* [Planck](/keyboards/planck/)
* [Preonic](/keyboards/preonic/)
* [ErgoDox EZ](/keyboards/ergodox_ez/)
* [Clueboard](/keyboards/clueboard/)
* [Cluepad](/keyboards/clueboard/17/)
* [Atreus](/keyboards/atreus/)

The project also includes community support for [lots of other keyboards](/keyboards/).

## Maintainers

QMK is developed and maintained by Jack Humbert of OLKB with contributions from the community, and of course, [Hasu](https://github.com/tmk). The OLKB product firmwares are maintained by [Jack Humbert](https://github.com/jackhumbert), the Ergodox EZ by [ZSA Technology Labs](https://github.com/zsa), the Clueboard by [Zach White](https://github.com/skullydazed), and the Atreus by [Phil Hagelberg](https://github.com/technomancy).

## Official Website

[qmk.fm](https://qmk.fm) is the official website of QMK, where you can find links to this page, the documentation, and the keyboards supported by QMK.
