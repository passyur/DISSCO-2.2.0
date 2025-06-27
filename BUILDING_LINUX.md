Building on Linux 
=================
(WIP)
Preliminary Requirements
--------------------------

The following are *necessary* to compile CMOD and LASS:

- A C++11-supporting compiler (g++, clang),
- A C compiler (gcc ...),
- cmake >= 3.5,
- libsndfile >= 1.0,
- libxerces-c >= 3.2, and
<!-- - muparser >= 2.X -->

<!-- Keeping for reference, but not relevant to current development -->
<!-- To compile with LASSIE, the following couple are *necessary* inclusions:

- GTK+ 2.4 < 3.24 (developers: also should be updated!) and
- GTKmm-2.4 >= 1.5. -->

To compile with LASSIE, the following is *necessary*:

- Qt >= 6.4

Recommended
-----------

LASSIE will want to open up a terminal, either gnome-terminal or xterm, to provide an interactive view of CMOD running your program. One of these is likely installed already if you're using a desktop environment. Otherwise, install one or the other, or both. *Note that LASSIE is an X application.*

Installing requirements and recommendations on:
-----------------------------------------------

### Debian-likes
Install the following:

    sudo apt install build-essential premake4 libsndfile1 libsndfile1-dev libxerces-c3.2 libxerces-c-dev libmuparser2v5 libmuparser-dev xterm

`build-essential` includes `g++` and `gcc`. If you prefer another C/C++ compiler, you can install separately.

<!-- TODO: RHEL, maybe -->

Installing DISSCO
-----------------
Just `git clone` this repo; explicitly:

    git clone https://github.com/passyur/DISSCO-2.2.0.git

Building
--------
In the project's root directory (by default: `path/to/DISSCO-X.X.X`), run the following:

    mkdir build && cd build

Then, in `build`, do

    cmake ..

to generate the build files and

    cmake --build .

to build.

By running this command in `build`, one generates a so-called *out-of-source* (OOS) build. The alternative, an in-source build, is heavily discouraged (including [by the CMake maintainers](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Getting%20Started.html#directory-structure)), and the root `CMakeLists.txt` reflects this distaste. The rationale is that OOS builds minimize clutter and collect all build files in one directory, whereas in-source builds put build files virtually everywhere. (This is bad.)

From `build`, you can clean `build` using `cmake --build . --target clean`. Alternatively, you can do `rm -r build` from outside of `build`.