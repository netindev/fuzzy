# fuzzy
lightweight app runner based on GTK

compile:
```
g++ -Os -s -fno-rtti -o program main.cpp window/window.cpp internal/commands.cpp `pkg-config --cflags --libs gtk+-3.0`
```
