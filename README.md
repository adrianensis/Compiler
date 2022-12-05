# README
---
## Compiler

This project is actually a **transpiler**, I just stared it as a compiler study/practice. Here I attempt to translate from a **simplified form of C++** to modern C++.

> (I should think in a better name for my simplified C++ version)

This was a couple of months side project, feel free to learn from it, but please don't expect a stable tool. There's half done stuff and buggy features here and there.

Still, I managed to port some classes from my game engine math library to **simplified C++**, which was way more than expected when I started this. Compilers are intimidating, but once you get to have a basic one working, it feels very powerful.

## Features

- No pointers or references
- Define the memory space for each class (stack or heap) by using **stack** keyword before class definition: `stack class Line: public Shape`
- Single file (no declaration/implementation separation)
- Modules!
- Automatic includes and circular dependency
- It relies on C++ linters and Language Servers for coloring/autocompletion
  - Some errors may be spotted (related with includes etc)
- It relies on some silly macros (see `code/core/FakeMacros.hpp`) to fake some keywords

## Experimental

  - Access to C++ std stuff

## Bugs

- Recursive Scope search must be improved 

## TODO

- Access to STL containers
- Reflection (it should come almost for free)
- Lambdas syntax