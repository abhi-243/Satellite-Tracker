#include "AccelStepper.h"
#include "QMC5883LCompass.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver
#define dirPin1 16
#define stepPin1 17
#define dirPin2 18
#define stepPin2 19
#define motorInterfaceType 1

//Encoder Pins
#define CLK1 33
#define DT1 32
#define SW1 35

#define CLK2 34
#define DT2 39
#define SW2 36

#define potPin 27

// Create instances of the AccelStepper class for each motor:
AccelStepper azStepper = AccelStepper(motorInterfaceType, stepPin1, dirPin1);
AccelStepper elStepper = AccelStepper(motorInterfaceType, stepPin2, dirPin2);

// Initialize the LCD, assuming it's a 16x2 LCD with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Magnetometer object
QMC5883LCompass compass;

//variables for printing
float azimuth = 0.0;
float elevation = 0.0;
String inputString = ""; // To store the input string


//logic variables
float azTarget = 0.0;
float elTarget = 0.0;
float azCurrent = 0.0;
float elCurrent = 0.0;
int azPosition = 0;
int elPosition = 0;
int azPre = -1;
int elPre = -1;
int az = 0;
int el = 0;

//more logic variables
bool azStop = false;
bool elStop = false;
int azDeadZone = 5;
int elDeadZone = 10;

//elevation pot
int potValue = 0;


void setup() {
  Serial.begin(9600);

  
  // Set the maximum speed and acceleration for both steppers:
  azStepper.setMaxSpeed(200);
  azStepper.setAcceleration(70);
  elStepper.setMaxSpeed(200);
  elStepper.setAcceleration(70);

  //begin the compass
  compass.init();
  
  //compass calibrations
  compass.setCalibrationOffsets(1850.00, -1740.00, -1297.00);
  compass.setCalibrationScales(0.92, 0.98, 1.12);

  // Initialize the LCD
  lcd.init();
  lcd.clear();
  lcd.backlight();

  // Clear LCD and set up initial display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ac:-----At:-----");
  lcd.setCursor(0, 1); 
  lcd.print("Ec: ----Et: ----");
}

void loop() {
  // Display the values on the LCD
  displayValues();

  // Check if data is available on Serial
  if (Serial.available() > 0) {
    // Read the incoming string until the newline character
    inputString = Serial.readStringUntil('\n');

    // Parse the azimuth and elevation from the string
    parseAZEL(inputString);
  }

  //
  potValue = analogRead(potPin);
  el = map(potValue, 1637, 3570, 0, 90);
  el = constrain(el, 0, 90);

  if(abs(el - elPre) >= 2) {
    elPre = el;
    elCurrent = el;
  }

  elPosition = (elTarget - elCurrent) * 200;
  if(elTarget <= 0) {
    elTarget = 0;
  }


  compass.read();

  az = compass.getAzimuth();

  if(abs(az - azPre) >= 3) {
    azPre = az;
    azCurrent = az;
  }

  if (azCurrent < 0) {
    azCurrent += 360;
  }


  if(elCurrent == elTarget) {
    elStepper.stop();
    elStop = true;
  } else if(abs(elCurrent - elTarget)>=elDeadZone) {
    elStop = false;
    if(elCurrent < elTarget) {
      elStepper.setSpeed(300);
    } else if(elCurrent > elTarget) {
      elStepper.setSpeed(-300);
    }
    elStepper.moveTo(elPosition);
    elStepper.run();
  }


  azPosition = (azTarget - azCurrent) * 200;

  if(azCurrent == azTarget) {
    azStepper.stop();
    azStop = true;
  } else if(abs(azCurrent - azTarget)>=azDeadZone) {
    azStop = false;
    if(azCurrent < azTarget) {
      azStepper.setSpeed(300);
    } else if(azCurrent > azTarget) {
      azStepper.setSpeed(-300);
    }
    azStepper.moveTo(azPosition);
    azStepper.run();
  }

}



void parseAZEL(String data) {
  int azIndex = data.indexOf("AZ: ");
  int elIndex = data.indexOf(", EL: ");

  if (azIndex != -1 && elIndex != -1) {
    // Extract the azimuth and elevation substrings
    String azString = data.substring(azIndex + 4, elIndex);
    String elString = data.substring(elIndex + 6);

    // Convert the strings to floats
    azimuth = azString.toFloat();
    elevation = elString.toFloat();
    azTarget = constrain(azimuth, 0.0, 359.0);
    elTarget = elevation;
  }
}


// Function to display azimuth and elevation on the LCD
void displayValues() {
  // Correct cursor positions in displayValues()
  lcd.setCursor(3, 0);  // Current azimuth at (3, 0)
  lcd.print("     ");
  lcd.setCursor(3, 0);
  lcd.print(azCurrent, 1);

  lcd.setCursor(3, 1);  // Current elevation at (3, 1)
  lcd.print("     ");
  lcd.setCursor(3, 1);
  lcd.print(elCurrent, 1);

  lcd.setCursor(11, 0); // Target azimuth at (11, 0)
  lcd.print("     ");
  lcd.setCursor(11, 0);
  lcd.print(azTarget, 1);

  lcd.setCursor(11, 1); // Target elevation at (11, 1)
  lcd.print("     ");
  lcd.setCursor(11, 1);
  lcd.print(elTarget, 1);

}