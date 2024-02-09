# hiasobi
- particle simulation program
- uses raylib for rendering
- not finished

# Build instructions
## Linux
- Clone the repository: `git clone https://github.com/sekomidev/hiasobi`
- Enter the repo directory: `cd hiasobi`
- Install premake5 with your package manager:
  - `sudo pacman -S premake` on Arch
  - `sudo apt-get install premake` on Debian-based systems
  - `sudo dnf install -y premake` on Fedora
- Install dependencies for raylib with your package manager:
  - `sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama`
- Generate a makefile: `premake5 gmake2`
- Build the application: `make config=release_x64 -j 4`
- Run it with `_bin/Release/hiasobi`

## Windows
fuck if i know
