/*
 AutoType file content read from SD card
 
 This example shows how to read content from a file and use the keyboard emulator to write that data.

 Board used: http://osoyoo.com/2018/06/25/osoyoo-pro-micro-board/
 
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 16
 ** MISO - pin 14
 ** CLK - pin 15
 ** CS - pin 10
  
 * Button used to control when to start the project
 ** Button - pin 9 as input pulled HIGH

 created   Aug 2018
 by Chris Stepanek

 This example code is in the public domain.

 */
#include <SD.h>
#include <SPI.h>
#include <HID.h>
#include <Keyboard.h>

const int BUTTON_PIN = 9; 
const int CS_PIN = 10;
const String fileName = "script.txt";

File file;

void setup() {
  //Make pin 9 an input and turn on the
  //pullup resistor so it goes HIGH unless
  //connected to ground.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Open serial communication for debug messages
  Serial.begin(9600);
  while (!Serial) {
    // Wait for serial port to connect
  }
  Serial.println("Waiting for button press...");
}

void loop() {
  while(digitalRead(BUTTON_PIN)) {
    // Do nothing while the pin is HIGH 
  }
  delay(100); // debounce delay
  doWork();
}

void doWork() 
{
  while (!InitializeSD()) {
    // Wait for SD card to initialize
  }
  
  if (SD.exists(fileName))
  {
    Serial.print(fileName);
    Serial.println(" exists");
    file = SD.open(fileName);
    if (file)
    {
      Serial.println("File opened");
      Serial.println("Begin keyboard emulation");
      Keyboard.begin();
      Serial.print("Writting file content... ");
      while (file.available())
      {
        Keyboard.write(file.read());
        delay(20);
      }
      Serial.println("complete");
      file.close();
      Serial.println("File closed");
      Keyboard.end();
      Serial.println("End keyboard emulation");
    }
  }
  else {
    Serial.print(fileName);
    Serial.println(" does not exist!");
    Serial.print("Please insert a SD card that contains ");
    Serial.println(fileName);
  }
  Serial.println("Waiting for button press...");
}

bool InitializeSD()
{
  Serial.print("Initializing SD card...");
  if (!SD.begin(CS_PIN)) {
    Serial.println(" Initialization failed!");
    Serial.println("Please insert SD card then press the button to continue.");
    while(digitalRead(BUTTON_PIN)) {
      // Wait for button press
    }
    delay(100); // debounce delay
    InitializeSD();
  }
  Serial.println("Initialization complete.");
  return true;
}

