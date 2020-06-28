# DIY Para-Para controller with Time of Flight sensors and Arduino

This is a short guide on building your own [Para-Para Paradise](https://en.wikipedia.org/wiki/Para_Para_Paradise) controller using an Arduino and [ToF sensors](https://en.wikipedia.org/wiki/Time-of-flight_camera).

![para-controller](https://user-images.githubusercontent.com/4284741/85947840-fc4c6080-b944-11ea-847f-0401745b6f3a.jpg)

* Cost: $150
* Time: a couple evenings
* Materials used:
  * 5x [VL53L1X](https://www.sparkfun.com/products/14722) 4 meter distance sensor
  * [Teensy 3.2](https://www.pjrc.com/store/teensy32_pins.html)
  * [Qwiic mux board](https://www.sparkfun.com/products/14685) to connect more than 1 sensor to the Teensy
  * [Regular jumper wires and qwiic wires](https://www.sparkfun.com/products/15081) since that's the connectors my breakout boards have
  
## Notes on materials and project limitations
  
**Disclaimer: this controller has poor range and field of view, see notes below.**

### Why these sensors?

I used this [distance sensor guide](https://www.sparkfun.com/distance_sensor_comparison_guide) to compare the characteristics of different sensors. I had four requirements:
  1. Range over 150 cm
  1. High update frequency, to work reasonably with timing windows
  1. A narrow-ish field of view, so that movement is only detected above the sensor
  1. $15 or so a piece (which ruled out all the cool lidars)

The VL53L1X have 400 cm range, 50 Hz update rate, and are $20 a piece. Points (1) and (2), however, turned out to be moot.
  * Although the sensor range is 4 metres, it cannot reliably detect small targets (like hands) at this distance. In practice, detection stopped working at 120 cm. Larger targets, such as a magazine, worked a little further, but nowhere close to 4 metres. This is why the sensors are lifted onto boxes in the picture.
  * Para-Para timing windows are huge. Parastar uses 300 ms windows for all judgments, so picking a sensor with 10 Hz update rate would have been sufficient.
  
### Which Arduino?

Any board that can act as a USB controller. Other than Teensy, that could be an Arduino Leonardo, or an Arduino Micro, or any other board built on an atmega32u4 controller. There are ways to make other controllers work as USB, but they looked too involved to me.

### Why the mux?

This was an easy way to connect multiple sensors to the same I2C bus. TL;DR all sensors of the same type have the same address on the bus, changing it is either a pain or impossible depending on the library, I tinkered and decided I'd rather throw $10 at it.

## Assembly

![para-assembly](https://user-images.githubusercontent.com/4284741/85947845-08d0b900-b945-11ea-98d6-174939a78fee.jpg)

1. Connect the Arduino to the main port on the mux
    * Find a cable with qwiic connector on one side, and 4 pins on the other
    * Plug the qwiic end into the MAIN port on the mux
    * Find the following pins on your Arduino: GND, 3.3V, SDA, SCL
    * Connect the 4 pins on the cable to the Arduino as follows:
      * Black = GND
      * Red = 3.3V
      * Blue = SDA
      * Yellow = SCL
1. Connect the sensors to ports 0-4 on the mux. Use a qwiic-qwiic cable if they are long enough. Otherwise you may need to use two qwiic to 4 pin cables and connect them with jumper wires in between. When doing so, trace the wire color to make sure the wires do not get mixed up from end to end.
1. Plug the Arduino into your PC with a USB cable

## Code

To program the controller, you will need the Arduino IDE, and the Teensyduino if using a Teensy.

The sketch uses a simple algorithm:
1. In a loop, poll each sensor for a reading
1. If the reading is lower than a set threshold, send a joystick button press
1. Else, send an off event to the button

Upload the [sketch](https://github.com/Sereni/tof-para-controller/blob/master/para/para.ino) to your board. Stepmania should now recognize the controller as a joystick, and allow you to map the buttons in the usual way.
