# Via

This library contains the cross-platform implementations of the Via modules. It can serve as a resource for anyone who wishes to hack their modules, make custom VCVRack plugins, or make derivative works for other platforms. Starling welcomes pull requests from anyone who wants to contribute their work to the project, but we recommend touching base with us before any major development because the current API is very much unstable.

## Building

There are currently 2 build targets, the STM32F373 MCU used on the hardware module and the VCVRack plugin build environment. Each build target has extra platform specific driver code and build instructions.

[Hardware Module](https://github.com/starlingcode/via_hardware_executables)

[Rack Plugin](https://github.com/starlingcode/via-for-rack)

If we get some time, we want to target other platforms like teensy or axoloti.

## Development Goals

We can already think of a few things that would make the library easier to work with. We welcome additional suggestions in the issue tracker

1. Timer handling classes that remove hardware specific code from the algorithm implementation instead of the current inline typedefs, likewise with any other typedefs in the module algorithm implementation.
1. Replace "magic numbers" with human readable constant definitions.
1. Abstract fixed point math operations into templated functions to allow for single or double precision implementations of the algorithms.
1. Use more standard practice polymorphism and take advantage of templating where possible.
1. Break apart the "synthesis" objects into simpler building blocks.
1. Efficient bandlimited interpolation or decimatation.
1. A real time autocorrelation algorithm.

## Documentation

[Preliminary documentation](https://starling.space/website/static/img/via_library_documentation/index.html)

We will try to improve the documentation as we improve the library itself.
