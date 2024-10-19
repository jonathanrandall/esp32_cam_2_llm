#ifndef OLED_STUFF_
#define OLED_STUFF_


//i2c for pwm driver
#include <Wire.h>
//oled
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define SDA_PIN 14
#define SCL_PIN 2

// Initialize the PCA9685
TwoWire I2CPWM = TwoWire(0);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2CPWM, -1);

void init_oled(){
  I2CPWM.begin(SDA_PIN, SCL_PIN, 100000);
  
  //oled
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3C for 128x64
  I2CPWM.begin(SDA_PIN, SCL_PIN, 100000);
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
  display.setRotation(0);
  display.setTextColor(WHITE);
  //to invert use display.setTextColor(BLACK,WHITE);
  display.setTextSize(2);
  display.setCursor(10, 10);
}

void oled_write(String str){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 10);
  display.println(str);
  display.display();
}

#endif