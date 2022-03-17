//Fixa så att mintuövergången blir bra 
//Fixa så att man kan byta mellan de olika 
//Fixa UI bugg där 05 går ihop

#include <RtcDS3231.h>
#include <Wire.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);



// Create new RTC module
RtcDS3231<TwoWire> rtcModule(Wire);


// Create variables
int hours;
int minutes;
int seconds;
int temp;
const int buttonPin = 2;
int buttonState = 0;
bool buttonvar = 0;


void setup() {
  // Begin serial
  Serial.begin(9600);
  rtcModule.Begin();
  // Update RTC module time to compilation time
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  rtcModule.SetDateTime(compiled);
  pinMode(buttonPin, INPUT);
}


void loop(void) {
  buttonState = digitalRead(buttonPin);

  //  if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    buttonvar = !buttonvar;
    delay (100);
  }

  Serial.print(buttonvar);


  if (buttonvar == 1) {
    // Draw2:    // Refresh picture
    oled.firstPage();
    do {
      drawtemp();
    } while (oled.nextPage());

  }
  if (buttonvar == 0) {
    // Refresh picture
    oled.firstPage();
    do {
      drawtime();
    } while (oled.nextPage());

  } 

}

void drawtime(void) {

  RtcDateTime now = rtcModule.GetDateTime();



  hours = now.Hour();
  minutes = now.Minute();
  seconds = now.Second();
  if (hours < 10) {
    oled.drawStr(39, 37, "0");
    oled.drawStr(49, 37, (String(hours)).c_str());
    oled.drawStr(59, 37, (":"));
    if (minutes < 10) {
      oled.drawStr(69, 37, "0");
      oled.drawStr(78, 37, (String(minutes)).c_str());
    }
    else {
      oled.drawStr(69, 37, (String(minutes)).c_str());
    }
  }

  else {
    oled.drawStr(39, 37, (String(hours)).c_str());
    oled.drawStr(59, 37, (":"));
    if (minutes < 10) {
      oled.drawStr(69, 37, "0");
      oled.drawStr(75, 37, (String(minutes)).c_str());
    }
    else {
      oled.drawStr(69, 37, (String(minutes)).c_str());
    }

  }



  // Set font to Helvetica size 10
  oled.setFont(u8g_font_helvB10);
}

void drawtemp(void) {
  RtcTemperature rtcTemp = rtcModule.GetTemperature();
  temp = rtcTemp.AsFloatDegC();

  oled.drawStr(57, 37, (String(temp)).c_str());
  oled.drawStr(77, 37, "C");

}