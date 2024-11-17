# LifeSim

## Dalton Pierce - November 2024

---

# Milestones

- [x] v0.1 - Get build process working
- [x] v0.2 - Clean up & abstract demo code
- [x] v0.3 - Draw World to viewport
- [x] v0.4 - Draw Meep to viewport
- [x] v0.5 - Move Meep around viewport
- [x] v0.6 - Draw multiple Meeps to viewport
- [x] v0.7 - Meeps interact with World & other Meeps
- [ ] v0.8 - GUI customizes World & Meep settings on-the-fly
- [ ] v0.9 - Meeps carry out behavior autonomously
- [ ] v1.0 - World runs smoothly, Meeps carry out their respective behavior & interact with World and fellow Meeps in simple ways

## Submodule Setup

1. git submodule add -b docking https://github.com/ocornut/imgui.git dependencies/imgui
2. git submodule add https://github.com/microsoft/vcpkg.git dependencies/vcpkg
3. git submodule update --init --recursive

---

# Libraries

- [OpenGL Math Library](https://github.com/g-truc/glm)
- [SFML (Threading, Window Management, Graphics, Audio, & Networking)](https://github.com/SFML/SFML)

---

# References

* [Basic physics interactions](https://www.youtube.com/watch?v=-_IspRG548E)
* [GJK Algorithm for collision detection](https://www.youtube.com/watch?v=MDusDn8oTSE)
* [FetchContent Docs](https://cmake.org/cmake/help/latest/module/FetchContent.html)
* [Build Custom Instrumentation](https://youtu.be/5R6ug5NMC1M?si=8sjy3Rx-31Ci19hC&t=584)
* [STB Library](https://github.com/nothings/stb/tree/master)

# Inspiration

* https://github.com/Bogdanctx/Radar-Contact/tree/main
* https://github.com/St0wy/StowyPhysicsEngine
* https://github.com/ocornut/imgui

---

# Getting Started

1. Run setup script: `./setup.sh` _(Unix)_ or `./setup.bat` _(Windows)_
