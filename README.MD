[![Publish Docker Image](https://github.com/wiiu-env/WiiUModuleSystem/actions/workflows/push_image.yml/badge.svg)](https://github.com/wiiu-env/WiiUModuleSystem/actions/workflows/push_image.yml)

# Wii U Module System

This lib is required to build Modules to be loaded with the [Wii U Module System Loader](https://github.com/wiiu-env/WUMSLoader).

## Usage

A module needs to implements at least the following macros.
```C++
#include <wums.h>

WUMS_MODULE_EXPORT_NAME("homebrew_modulename");

WUMS_INITIALIZE() {
    /** THIS CODE WILL BE RUN ONCE **/ 
}
```

The `WUMS_MODULE_EXPORT_NAME` needs to be a globally unique name across all loaded modules. `WUMS_INITIALIZE` defines the code that will be run after the module was loaded.
The module will have full permission to the sd card via `fs:/vol/external01`.

### Optional macros/hooks
Other optional macros/hooks are support, here some examples:

```C++
WUMS_APPLICATION_STARTS() {
    /** Is called when a new application was started. **/
}

WUMS_APPLICATION_REQUESTS_EXIT() {
    /** Is called when a new application is going to be closed. **/
}
```

This list is incomplete, see `hooks.h` for all hooks and `meta.h` for all macros.

### Export functions

Modules can be used to export functions for other modules or "normal" applications.

```C++
#include <wums.h>

void MyCustomFunction() {
    /** Some special code that should be useable for everyone */
}

WUMS_EXPORT_FUNCTION(MyCustomFunction);
```

This function can then be used like they would be inside a .rpl, where the name of the "rpl" is the one defined in `WUMS_MODULE_EXPORT_NAME`.

Example using the Cafe OS OSDynLoad API:
```C++
if (OSDynLoad_Acquire("homebrew_modulename", &sModuleHandle) != OS_DYNLOAD_OK) {
    OSFatal("OSDynLoad_Acquire failed.");
}

void (*sMyCustomFunction)() = NULL;

if (OSDynLoad_FindExport(sModuleHandle, FALSE, "MyCustomFunction", (void**) &sMyCustomFunction) != OS_DYNLOAD_OK) {
    OSFatal("OSDynLoad_FindExport failed.");
}
```

## Use this lib in Dockerfiles.
A prebuilt version of this lib can found on dockerhub. To use it for your projects, add this to your Dockerfile.
```Dockerfile
[...]
COPY --from=wiiuenv/wiiumodulesystem:[tag] /artifacts $DEVKITPRO
[...]
```
Replace [tag] with a tag you want to use, a list of tags can be found [here](https://hub.docker.com/r/wiiuenv/wiiumodulesystem/tags). 
It's highly recommended to pin the version to the **latest date** instead of using `latest`.

## Format the code via docker
```bash
docker run --rm -v ${PWD}:/src wiiuenv/clang-format:13.0.0-2 -r ./include ./libraries ./example/example_module/source -i
```
