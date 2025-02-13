Building on macOS 
=================
(WIP, not yet building without some hacking)

What is marked as not yet supported (denoted by ❌ and understood as referring to the entire line/paragraph) will be supported, soon.

We intend to offer static binaries at some point.

Preliminary Requirements
--------------------------

The following are *necessary* to compile CMOD and LASS:

- A C++11-supporting compiler (g++, clang++),
- A C compiler (gcc, clang),
- cmake >= 3.25,
- libsndfile >= 1.0,
- libxerces-c >= 3.2, and
<!-- - muparser >= 2.X -->

To compile with LASSIE, the following is *necessary*:

- Qt >= 6.4

Installing requirements and recommendations:
--------------------------------------------

<!-- *For all methods*, it's worth keeping in mind that we statically link `muparser`, meaning you don't need to worry about installing it. Please report any issues related to muparser during compilation. -->

### brew
`brew`, or homebrew, is something like a package manager for macOS. It's widely used and tends to have better maintained packages than the equivalent ports on MacPorts. (As of 11/13/24: It has a newer stable version of `libxerces-c` and an older stable version of Qt6.) Read more and acquire it at [https://brew.sh](https://brew.sh). Simply:

    $ brew install libsndfile xerces-c qt@6

<!-- TODO: RHEL, maybe -->

### port
`port`, or MacPorts, is something like BSD ports for macOS. It's widely used, but less so than Homebrew. Read more and acquire it at [https://macports.org](https://macports.org). Simply, with `sudo`/root:

    # port install libsndfile xercesc3 qt6

### (❌) Other accommodations
CMake automatically finds the relevant packages installed on the build system. If you have your own installations of the above libraries you'd like to compile against, you may ensure they're included and linked at compile-time by passing the following arguments (when `cmake`ing):

    -DLIBSNDFILE_INCLUDE='/path/to/libsndfile/include' -DLIBSNDFILE_LIB='/path/to/libsndfile/lib'

or

    -DLIBXERCESC_INCLUDE='/path/to/xerces-c/.../include' -DLIBXERCESC_LIB='/path/to/xerces-c/.../lib'

For your personal edification: for ARM Macs, `brew` will install in `/opt/homebrew/Cellar/[your stuff here]` and symlink to `/opt/homebrew/opt/[your stuff here]`, and for Intel/x86 Macs, `brew` will install in `/usr/local/bin/[your stuff here]`.

MacPorts will install it in `/opt/local/include/[your includes here]` and `/opt/local/lib/[your libs here]`.

Installing DISSCO
-----------------
Just `git clone` this repo; explicitly:

    git clone https://github.com/passyur/DISSCO-2.2.0.git

Building
--------
From the project's root directory (by default: `path/to/DISSCO-X.X.X`), run the following:

    mkdir build && cd build

Then, in `build`, do

    cmake ..

to generate the build files and

    cmake --build .

to build.

By running this command in `build`, one generates a so-called *out-of-source* (OOS) build. The alternative, an in-source build, is heavily discouraged (including [by the CMake maintainers](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Getting%20Started.html#directory-structure)), and the root `CMakeLists.txt` reflects this distaste. The rationale is that OOS builds minimize clutter and collect all build files in one directory, whereas in-source builds put build files virtually everywhere. (This is bad.)

From `build`, you can clean `build` using `cmake --build . --target clean`. Alternatively, you can do `rm -r build` from outside of `build`.