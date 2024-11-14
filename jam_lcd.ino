#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define RTC_SDA D2 // GPIO4
#define RTC_SCL D1 // GPIO5
#define LCD_SDA D6
#define LCD_SCL D7

#define ADDR_RTC 0x68
#define ADDR_LCD 0x27 //0x27

LiquidCrystal_I2C lcd(ADDR_LCD, 16, 2);
RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  
  // Wire.begin();
  Wire.begin(RTC_SDA, RTC_SCL);
  if (!rtc.begin()) {
		Serial.println("RTC tidak terdeteksi");
		while (1);
	}

  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();
  lcd.print("Hello, World");
  delay(2500);
  lcd.clear();
}

void loop() {
  Wire.begin(RTC_SDA, RTC_SCL);
  DateTime now = rtc.now();

  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.setCursor(4, 0);
  // lcd.print("Waktu: ");
  if (now.hour() < 10)
  lcd.print('0');
  lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10)
  lcd.print('0');
  lcd.print(now.minute());
  lcd.print(':');
  if (now.second() < 10)
  lcd.print('0');
  lcd.print(now.second());

  lcd.setCursor(3, 1);
  // lcd.print("Tanggal: ");
  if (now.day() < 10)
  lcd.print('0');
  lcd.print(now.day());
  lcd.print('/');
  if (now.month() < 10)
  lcd.print('0');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());

  // Menampilkan waktu di serial monitor
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}
