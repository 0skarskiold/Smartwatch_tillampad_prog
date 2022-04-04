//Fixa så att man kan byta mellan de olika funktionerna
//Easter eggs: Garmin i början, varning vid hög puls
//kan printa allt på en sträng
//kan fixa så att allt är typ centrerat


//Importerar bibliotek
#include <RtcDS3231.h>
#include <Wire.h>
#include "U8glib.h"
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   



U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);



// Skapar en RTC-modul
RtcDS3231<TwoWire> rtcModule(Wire);


// Creates an instance of the PulseSensorPlayground object called "pulseSensor"
PulseSensorPlayground pulseSensor;


// Skapar variabler. Behöver de ha const framför sig?
int hours;
int minutes;
int seconds;
int temp;
const int buttonPin = 2;
int buttonState = 0;
int buttonvar = 1;
const int SCREEN_WIDTH = 128;
int savedBPM = 0;

//Variablerna från pulsprogrammet
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore.
const int PulseWire = 0;
const int LED13 = 13;


void setup() {
  // Begin serial
  rtcModule.Begin();
  // Update RTC module time to compilation time
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  rtcModule.SetDateTime(compiled);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);         // Set's up Serial Communication at certain speed.

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);

  pulseSensor.setThreshold(Threshold);

  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }
}


void loop(void) {
  buttonState = digitalRead(buttonPin);

  //Serial.println(buttonvar);
  //  Om man trycker på knappen byter buttonState mellan true och false. Delay är där för att man inte ska kunna trycka flera gånger när man bara vill trycka en gång.
  if (buttonState == HIGH && buttonvar != 3) {
    buttonvar += 1;
    delay (100);
  }
  else if (buttonState == HIGH && buttonvar == 3) {
    buttonvar = 1;
    delay (100);
  }

  if (buttonvar == 3) {
    // Refresh picture
    oled.firstPage();
    do {
      drawpulse();
    } while (oled.nextPage());

  }

  if (buttonvar == 2) {
    oled.firstPage();
    do {
      drawtemp();
    } while (oled.nextPage());

  }

  //Om buttonvar är false kommer tiden visas
  if (buttonvar == 1) {
    // Refresh picture
    oled.firstPage();
    do {
      drawtime();
    } while (oled.nextPage());
  }

}


//Funktionen innhåller koden som visar tiden
void drawtime(void) {

  RtcDateTime now = rtcModule.GetDateTime();



  hours = now.Hour();
  minutes = now.Minute();
  seconds = now.Second();


  if (hours < 10) { //Om timmar < 10 kommer en 0:a visas framför siffran
    oled.drawStr(39, 37, "0");
    oled.drawStr(49, 37, (String(hours)).c_str());
    oled.drawStr(59, 37, (":"));
  }

  else {
    oled.drawStr(39, 37, (String(hours)).c_str());
    oled.drawStr(59, 37, (":"));
  }

  if (minutes < 10) { // Om minuterna är mindre än 10 kommer en nolla läggas till.
    oled.drawStr(69, 37, "0");
    oled.drawStr(78, 37, (String(minutes)).c_str());
  }
  else {
    oled.drawStr(69, 37, (String(minutes)).c_str());
  }


  // Set font to Helvetica size 10
  oled.setFont(u8g_font_helvB10);
}






//Här är funktionen som skriver ut temperaturen
void drawtemp(void) {
  RtcTemperature rtcTemp = rtcModule.GetTemperature();
  temp = rtcTemp.AsFloatDegC();

  oled.drawStr(57, 37, (String(temp)).c_str());
  oled.drawStr(77, 37, "C");

}






// Här är funktionen som skriver ut pulsen
void drawpulse(void) {

  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
  // "myBPM" hold this BPM value now.


  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
    Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");                        // Print phrase "BPM: "
    Serial.println(myBPM);                        // Print the value inside of myBPM.

  }
  if (myBPM != 0) {
    int textPosition = (SCREEN_WIDTH - strlen((String(myBPM).c_str()))) / 2;
    oled.drawStr(textPosition, 37, String(myBPM).c_str());
    savedBPM = myBPM;
  }
  else {
    int textPosition = (SCREEN_WIDTH - strlen((String(savedBPM).c_str()))) / 2;
    oled.drawStr(textPosition, 38, String(savedBPM).c_str());
  }
  delay(20);                    // considered best practice in a simple sketch.
}