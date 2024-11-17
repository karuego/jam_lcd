#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

#define RTC_SDA  D2 // GPIO4
#define RTC_SCL  D1 // GPIO5
// #define RTC_ADDR 0x68
#define wire_rtc() Wire.begin(RTC_SDA, RTC_SCL);

#define LCD_SDA  D6
#define LCD_SCL  D7
#define LCD_ADDR 0x27
#define wire_lcd() Wire.begin(LCD_SDA, LCD_SCL);

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

RTC_DS3231 rtc;
RTC_Millis rtc_millis;

DateTime next;

void setup() {
  Serial.begin(115200);
  
  //#ifndef ESP8266
  //  while (!Serial); // wait for serial port to connect. Needed for native USB
  //#endif

  wire_lcd();
  lcd.init();
  lcd.backlight();
  lcd.print("Hello, World");
  delay(2300);
  lcd.clear();

  while (!mulai_rtc()) {
    wire_lcd();

    lcd.setCursor(0, 0);
    lcd.print("Error ::");
    lcd.setCursor(0, 1);
    lcd.print("    No RTC");

    delay(500);
  }

  wire_lcd();
  lcd.clear();
}

void loop() {
  if (rtc_millis.now() >= next)
    sinkron_rtc();

  DateTime now = rtc_millis.now();

  wire_lcd();
  lcd.setCursor(4, 0);
  // lcd.print("Waktu: ");
  if (now.hour() < 10) lcd.print('0');
  lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10) lcd.print('0');
  lcd.print(now.minute());
  lcd.print(':');
  if (now.second() < 10) lcd.print('0');
  lcd.print(now.second());

  lcd.setCursor(3, 1);
  // lcd.print("Tanggal: ");
  if (now.day() < 10) lcd.print('0');
  lcd.print(now.day());
  lcd.print('/');
  if (now.month() < 10) lcd.print('0');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());

  // Menampilkan waktu di serial monitor
  if (Serial) {
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

    Serial.print("Temperature: ");
    Serial.print(rtc.getTemperature());
    Serial.println(" C");
  }

  delay(1000);
}

bool mulai_rtc() {
  wire_rtc();

  if (!rtc.begin()) {
      return false;
  }

  rtc.disable32K();
  // rtc.disableAlarm(0);
  rtc.writeSqwPinMode(DS3231_OFF);
  
  return true;
}

void sinkron_rtc() {
  if (mulai_rtc()) {
    rtc_millis.adjust(rtc.now());
  }

  next = rtc_millis.now() + TimeSpan(0, 0, 5, 0);
}
