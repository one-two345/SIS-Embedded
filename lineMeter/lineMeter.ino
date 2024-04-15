// C++ code
//
#include "LiquidCrystal_I2C.h"

int soil_moisture = 0;

float soil_moisture_percentage = 0;

LiquidCrystal_I2C lcd_1(32, 16, 2);

void setup()
{
  lcd_1.init();
  pinMode(A0, INPUT);

  lcd_1.setCursor(0, 0);
  lcd_1.backlight();
  lcd_1.display();
}

void loop()
{
  soil_moisture = analogRead(A0); 
  soil_moisture_percentage = ((soil_moisture / 876) * 100);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Smart Irrigation");
  lcd_1.setCursor(0, 1);
  lcd_1.print(soil_moisture_percentage);
  delay(10); // Delay a little bit to improve simulation performance
}
