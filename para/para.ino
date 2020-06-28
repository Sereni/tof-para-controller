#include <Wire.h>
#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>

SFEVL53L1X distanceSensor;

#define NUMBER_OF_SENSORS 5
#define THRESHOLD 1650
bool debug = true;

// Mapping of sensors to joystick buttons
char buttons[NUMBER_OF_SENSORS] = {1, 2, 3, 4, 5};

void setup()
{
  Serial.begin(9600);
  if (debug) { Serial.println("Qwiic Mux Shield Read Example"); }
  Wire.begin();

  //Initialize all the sensors
  for (byte x = 0 ; x < NUMBER_OF_SENSORS ; x++)
  {
    enableMuxPort(x); //Tell mux to connect to port X
    distanceSensor.begin(); //Init the sensor connected to this port
//    distanceSensor.setTimingBudgetInMs(500);
    distanceSensor.setDistanceModeShort();
    distanceSensor.setTimingBudgetInMs(50);
    disableMuxPort(x);
    Joystick.button(x, 0);
  }

  if (debug) { Serial.println("Mux Shield online"); }
}

void loop()
{
  for (byte x = 0 ; x < NUMBER_OF_SENSORS ; x++)
  {
     enableMuxPort(x); //Tell mux to connect to this port, and this port only

    distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
    int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
    distanceSensor.clearInterrupt();
    distanceSensor.stopRanging();

    if (distance < THRESHOLD && distance > 0)
    {
      if (debug) { Serial.println(distance); }
      Joystick.button(buttons[x], 1);
    } else {
      Joystick.button(buttons[x], 0);
      }
     disableMuxPort(x); //Tell mux to disconnect from this port
  }
   delay(1); //Wait for next reading
}
