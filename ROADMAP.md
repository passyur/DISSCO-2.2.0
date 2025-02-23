Systems Needs
-------------
Of extreme importance:
- [x] Transition from `premake` to `CMake`
- [ ] Port from GTK+2.X to Qt6.4

Of roughly equal importance:
- [ ] macOS support
    - [ ] CMOD
        - [x] Compiles
        - [ ] Works
    - [ ] LASS
        - [x] Compiles
        - [ ] Works
    - [ ] LASSIE
        - [x] Compiles
        - [ ] Works
    - [x] Build Documentation

- [ ] Windows support
    - [ ] CMOD
      - [ ] Compiles
      - [ ] Works
    - [ ] LASS
      - [ ] Compiles
      - [ ] Works
    - [ ] LASSIE
      - [ ] Compiles
      - [ ] Works
    - [x] Build Documentation

- [ ] Linux Support
  - [x] CMOD
  - [x] LASS
  - [ ] LASSIE
    - [x] Compiles
    - [ ] Works
  - [x] Build Documentation

Other important matters:
- [ ] Satisfactory Documentation (contribute using the project wiki)
  - [ ] Translating the files in Documents to a wiki format
  - [ ] Extending documentation where necessary/desired
- [ ] Add Dockerfile and Apptainer definition
- [ ] Replace all '.h' extensions with '.hpp' extensions, particularly when the file uses C++ features
- [ ] Tidy up the project
  - [ ] Remove references to deprecated functions, replace if necessary
  - [ ] Make code modular while preserving functionality
  - [ ] Standardize style
  - [ ] Compactify file structure (to prevent undue sprawl)

My wish list to Santa this year, or new years' resolutions:
- [ ] Transition from XML to, e.g., JSON
- [ ] Cross-platform parallelization
  - [ ] CUDA
  - [ ] Accelerate/Metal
  <!-- - [ ] OpenMP? -->
- [ ] Better memory management
