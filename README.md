# biogui
_Basic HMI framework for a TFT LCD touchscreen_

This is a framework for building Human-Machine Interfaces (HMIs) for primary application in research devices at the Institute of Biology of the University of Brasília. It was made as part of a master's degree course in biomedical engineering.

This is not meant to be user-friendly nor powerful enough for widespread commercial use.

## Widgets

The building blocks of screen interactions. Each element shown on screen is a widget.

### Slider

A slider bar for setting some parameter's value from a given range.

### Button

A simple text button with visual feedback on touch and which invokes a callback function. This function must be implemented by the user and passed to the Button constructor via a void function pointer.

### Label

A widget for displaying text on the screen. 

### Timer

Just like your microwave oven, many research devices benefit greatly from implementing a timer. So I went ahead and did it.

## Screens

A screen in the context of this framework is to be thought of as a set of widgets that fill some purpose, e.g. _the configuration screen on a 3d printer_, or _the running screen with countdown timer and wash cycle info on a smart washing machine_. 

Navigating between screens is made easy with this framework.

## Router

The router is responsible for _routing screen navigation_. It automatically updates all the widgets on a screen and handles seamless screen transition routines.

cpplint --recursive --filter=-whitespace/tab,-whitespace/braces,-whitespace/comments,-whitespace/indent,-whitespace/line_length,-whitespace/parens ./*