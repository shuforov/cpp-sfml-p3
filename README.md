# Shape Bouncer application

A simple C++ program using the SFML library to render shapes (circles and rectangles) with text labels.  
Shapes move around the window and bounce off the edges.

![Project3_oVaT0zGSSR](https://github.com/user-attachments/assets/dd29f430-106b-4bb9-9fe6-a9b3a2476ba5)

---

## Features

✅ Renders circles and rectangles  
✅ Displays text labels inside each shape  
✅ Shapes move with individual speeds  
✅ Bounce off the window edges  
✅ Configurable settings via a text file

---

## Configuration File

All shapes and settings are loaded from a text file.  

Each line in the file starts with a **keyword** that describes what it configures:

## Example of config file
Window 800 600 <br>
Font fonts/arial.ttf 16 255 255 255 <br>
Circle CircleOne 100 100 5 3 255 0 0 30 <br>
Rectangle RectOne 200 200 4 2 0 255 0 60 40 <br>

## Explanation of config file
Window [width] [Height] <br>
Font [path] [size] [RGB] <br>
Circle [name] [positionXY] [speedXY] [RGB] [size] <br>
Rectangle [name] [positionXY] [speedXY] [RGB] [size] <br>
