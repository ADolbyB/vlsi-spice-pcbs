# H-bridge Power Motor Controller:

The idea for this controller can be found here: [DIY High Current Motor Driver](https://www.instructables.com/DIY-High-Current-Motor-Driver-h-bridge/). 
I will be modifying and updating this circuit and controller for a different project. The original project author's code is written in Arduino `.ino` format and this should be updated to "Real C/C++".
I am also anticipating that equivalent parts may need to be substituted.

Ideally, I would like to have a similar motor speed controller written in FreeRTOS using the ESP32 Platform based on this design.

## Original [Design Specs](https://www.instructables.com/DIY-High-Current-Motor-Driver-h-bridge/):

- 24v Minimum.
- BiDirectional Motor Control.
- PWM control.
- Scalable high current capable (~100Amps).
- Minimal COmponents.
- 5V Step-Down for Logic.
- Battery Voltage Sense.
- Arduino Nano Controller (This will be updated with a more modern solution).
    - Needs access to inputs for specific uses: 
        - Throttle Trim Top & Bottom.
        - Right and Left direction.
        - Enable / Disable.
        - 1 extra input.
- Access to unused GPIO pins for outputs (LED's, etc.).

## How to open in EasyEDA:

At editor, Click the document icon on the topbar, via "Document" > "Open" > "EasyEDA Source", select json file, then open in the editor.

The free version of EasyEDA allows for local project file storage and is more than good enough for a project build like this.