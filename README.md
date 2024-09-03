![Banner](banner.jpg)

# LogiGates

## Brief description:
LogiGates is a simple logical chain simulator. It includes all the basic logic elements such as NOT, AND, OR, XOR, implication, and equivalent. In addition, it includes some additional elements, such as a 5-bit decimal to binary number converter, as well as a 5-bit binary number display. You can also use the lamp to check the presence of the signal. The workspace is based on the use of nodes that are connected sequentially to form a logical chain. You can save your workspace as a file and then upload it later, or send it to other person.

![Banner](demo.gif)

## Building

### GNU/Linux (Debian based distros)

1. Install SDL2 `sudo apt install libsdl2-dev`

2. Clone this repository
`git clone https://github.com/MHDtA-dev/LogiGates.git`

3. Go to the project directory
`cd LogiGates-master`

4. Create build directory and go to it
```
  mkdir build
  cd build
```

5. Run CMake `cmake ..`

6. Build using make or ninja (Depends on your generator)
`make` OR `ninja`

### MacOS

1. Install SDL2 with brew `brew install sdl2`

2. Clone this repository
`git clone https://github.com/MHDtA-dev/LogiGates.git`

3. Go to the project directory
`cd LogiGates-master`

4. Create build directory and go to it
```
  mkdir build
  cd build
```

5. Run CMake `cmake ..`

6. Build using make or ninja (Depends on your generator)
`make` OR `ninja`

### Windows

Bruh :)
