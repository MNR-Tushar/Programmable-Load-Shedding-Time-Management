#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Keypad.h>
// Initialize LCD and RTC
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust I2C address if needed
RTC_DS1307 rtc;
// Keypad setup
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {6, 7, 8, 9};
byte colPins[COLS] = {2, 3, 4};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// LEDs
const int redLED = 10;
const int greenLED = 11;
// Time variables
int startHour = 0, startMinute = 0, endHour = 0, endMinute = 0;
void setup() {
  // Initialize components
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  lcd.init();  // Initialize LCD
  lcd.backlight();  // Turn on backlight
  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1); // Halt execution if RTC fails
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
  lcd.print("  Programmable");
  delay(2000);
  lcd.clear();
  lcd.print(" Load Shedding ");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Time Management");
  delay(2000);
}
void loop() {
  // Get load-shedding times
  lcd.clear();
  lcd.print("Set Start Time:");
  delay(2000);
  lcd.setCursor(0, 1);
  startHour = getTimeInput("Hour");
  startMinute = getTimeInput("Minute");
  lcd.clear();
  lcd.print("Set End Time:");
  delay(2000);
  lcd.setCursor(0, 1);
  endHour = getTimeInput("Hour");
  endMinute = getTimeInput("Minute");
  // Monitor time and control LEDs
  while (true) {
    DateTime now = rtc.now();
    int currentHour = now.hour();
    int currentMinute = now.minute();
    lcd.clear();
    lcd.print("Time: ");
    lcd.print(now.hour());
    lcd.print(":");
    lcd.print(now.minute());
    if ((currentHour > startHour || (currentHour == startHour && currentMinute >= startMinute)) &&
        (currentHour < endHour || (currentHour == endHour && currentMinute <= endMinute))) {
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Power on AB");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print("Load Shedding XY");
      delay(1500);
    }
    else {
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Power on XY");
      delay(1500);
      lcd.setCursor(0, 1);
      lcd.print("Load Shedding AB");
      delay(1500); 
    }
    delay(1000); // Refresh display every second
  }
}
// Function to get input for time
int getTimeInput(String type) {
  lcd.clear();
  lcd.print(type + ":");
  lcd.setCursor(0, 1);
 String input = "";
  char key;
  while (true) {
    key = keypad.getKey();
    if (key) {
      if (key == '#') break; // Confirm input
      if (key == '*') {
        input = ""; // Clear input
        lcd.setCursor(0, 1);
        lcd.print("                "); // Clear line
        lcd.setCursor(0, 1);
      }
else {
        input += key;
        lcd.print(key);
      }
    }
  }
  return input.toInt();
}
