# SFML Collision Demo
This is a shape collision demo using C++ and SFML library which can be configured by config file which will be read and run to a window.
In the config file, it can be specified with following items.
1. Window (window width) (window height) (frame rate limit)
2. Font (font path) (font size) (r) (g) (b)
3. Circle (shape name) (position x) (position y) (speed x) (speed y) (r) (g) (b) (radius)
4. Rectangle (shape name) (position x) (position y) (speed x) (speed y) (r) (g) (b) (width) (height)

It is recommended to specify the Window and Font before any shapes to prevent errors (shape needs font properties to set its name on it)