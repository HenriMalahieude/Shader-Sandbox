# AI Coding Assistant Guide — Shader-Sandbox

Purpose
- Help AI agents become productive quickly in this repo: a collection of single-file raylib demo programs and GPU/CPU shaders.

Big picture
- Project is a set of small, standalone C++ demos (top-level .cpp files) that use raylib for windowing/GL.
- Shaders are in `_builds/shaders/` (GPU `.vs`/`.fs`); images in `_builds/imgs/`.
- Some demos use `rlights.h`, which provides a small lighting helper library—only one compilation unit (`RLIGHTS_IMPLEMENTATION`).

Build & run (concrete)
- Builds use the `_builds/Makefile`. Common usage from repo root:

```bash
cd _builds
make scan         # builds scan.exe by compiling ../scan.cpp
# or from root:
make -C _builds scan
# or just make most recent
make 
```

- If running on windows, use `mingw32-make` instead of `make`.

- Important flags & env:
  - Compiler: `g++` (Makefile uses `CC = g++`).
  - `RAYLIB_SRC` must point to your local raylib source if not installed (default `../../raylib/src/`).
  - Link flags include `-lopengl32 -lgdi32 -lwinmm` on Windows.
  - Binaries are produced in `_builds/` (Makefile compiles `../*.cpp` into executables in this directory).

Conventions & patterns
- Each demo is usually a single top-level `.cpp` (e.g., `scan.cpp`, `warp.cpp`, `solar.cpp`). Make targets map to these files (see `_builds/Makefile`).
- Some targets deviate from name-to-file mapping (example: `solar_tex` target builds `../solar.cpp` to `solar_tex.exe`). Check `_builds/Makefile` when adding or editing targets.
- CPU-side visual tricks are sometimes called "Shaders", with quotes, in README — those are not GPU shaders (README note).
- Shader uniform naming: `rlights.h` expects shader uniforms like `lights[0].enabled`, `lights[i].position` — follow those names if using the helper.

What to inspect first (quick links)
- Lighting helper: `rlights.h` — how lights are created and shader locations retrieved.
- Build script: `_builds/Makefile` — canonical build targets and `RAYLIB_SRC` usage.
- Example demos: `basic_lighting.cpp`, `scan.cpp`, `stars_by_instancing.cpp`, `camera_glitch.cpp`.
- GPU shader examples: `shaders/phong/lighting.vs`, `shaders/phong/lighting.fs`, `shaders/sun/sun.vs`.

When modifying or adding a demo
- Add a top-level `.cpp` file and add a corresponding target to `_builds/Makefile` (follow existing targets for naming and flags).
- Ensure only one file defines `RLIGHTS_IMPLEMENTATION` if you include `rlights.h` implementation.
- Keep resource paths in mind: binaries are built in `_builds/`, so running them from that directory may affect relative paths to `shaders/` and `_imgs/`.

Debugging & testing
- Makefile builds with debug flags: `-g3 -Og` (see `ALL_FLAGS`). Use `gdb` or a Windows debugger to inspect executables under `_builds/`.

Integration & dependencies
- Primary external dependency: raylib. Either install raylib or set `RAYLIB_SRC` to the raylib source directory. However, this should already be done.

If this file already exists
- Merge gently: preserve any project-specific notes already present and update the Build & run and Conventions sections above.
