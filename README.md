# C++ Generic Firmware Loader

This Libary will load your Firmware to any Device supported by the 
[C++ Generic Bootloader](https://github.com/SetZero/cpp-firmware-loader) via Serial.

## Dependencies:

**Required:**
* [Asio](https://think-async.com/Asio/)
* [Poco](https://pocoproject.org/)
* [Clara](https://github.com/catchorg/Clara)
* [Intel Hex Class](https://github.com/codinghead/Intel-HEX-Class)

**Optional:**
* [Catch2](https://github.com/catchorg/Catch2) (for testing)

## Recommended Installation:

It is reccomended to use [vcpkg](https://github.com/microsoft/vcpkg/) or [Conan](https://conan.io/) for most of the dependencies.
We will use **vcpkg** for this example.

```
# vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

# Clara
./vcpkg install clara

# Poco
./vcpkg install poco

# Asio
./vcpkg install asio

# Catch (Optional)
./vcpkg install catch
```

Aftr this append 
```
-DCMAKE_TOOLCHAIN_FILE=**path/to/vcpkg.cmake**
```
to your cmake command line arguments
