#include "wifi_config.h"
#include <WiFi.h>
//#include <ezTime.h>
#include "TimeDB.h"
#include "WavPlayer.h"
#include "VolumeSelector.h"
#include "TimeDB.h"
#include "Configurer.h"
#include "sound/WavData.h"

WavPlayer player;
WiFiClient wifiClient;
String uniqueId;
//Timezone LocalTime;
//#define LOCAL_TIMEZONE "America/Denver"

int time_hour;
int time_minute;

const int STATUS_LED = 10;
const int BUTTON_PIN = 16;
const int VOLUME_RESISTOR_PIN = 12;
const int VOLUME_NORESISTOR_PIN = 13;
const int SPEAK_TIMEOUT = 5000;

int touchThreshold = 60; // copper plate sensitivity level

// TODO: 
// update IDE... need spiffs
// look at the DAC audio guy's article
// SPIFFS: ESP32 ... easy to upload all the .wav files this way.

enum SpeechState {
  CONFIGURING,
  WAITING,
  SPEAK_HOUR,
  SPEAK_MINUTE
};

SpeechState speechstate;

TimeDB timeDB;
Configurer configurer;

// SETUP and LOOP are the two required functions in Arduino

void setup() {
  digitalWrite(STATUS_LED, 1);    // Inverted for the built-in light
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
    
  Serial.begin(115200);
  delay(5000); // This is necessary for the ESP32 to get serial messages
  Serial.println("..... STARTING .....");

  // If there's no configuration or the button is pressed on bootup, go into config mode
  String ssid, password;
  if(!configurer.GetConfig(ssid, password)) {
    Serial.println("***************** CONFIGURE ******************");
    configurer.setup();
    speechstate = CONFIGURING;
  }
  else {
    WiFi.begin(cfg.ssid, cfg.password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }
    Serial.println("Wifi Connected");

//  waitForSync();  // from ezTime, means wait for NTP to get date and time.
//  Serial.println("synced");
//  LocalTime.setLocation(LOCAL_TIMEZONE);
    timeDB.SetTimezone("America/Denver");
    timeDB.UpdateTime();

    VolumeSelector vs(VOLUME_RESISTOR_PIN, VOLUME_NORESISTOR_PIN);
    vs.SetLevel(VolumeSelector::EXTRA_QUIET);

    speechstate = WAITING;
  }
  Serial.println("Done with setup()");
}

void loop() {

  switch (speechstate)
  {
  case CONFIGURING:
    configurer.loop();
    break;
  case SPEAK_HOUR:
    speechstate = SpeakHour();
    break;
  case SPEAK_MINUTE:
    speechstate = SpeakMinute();
    break;
  case WAITING:
  default:
    speechstate = DoWaiting();
    break;
  }
}

SpeechState SpeakHour() {
  SpeechState rv = SPEAK_HOUR;

  if (!player.Update()) { 
    // this handles the change from hour to minute. No longer playing the hour.
    Serial.println("minute " + (String)time_minute);
    player.StartPlaying(minuteWav[time_minute / 5]);
    Serial.println("HOUR->MINUTE");
    rv = SPEAK_MINUTE;
  }
  return rv;
}

SpeechState SpeakMinute() {
  SpeechState rv = SPEAK_MINUTE;

    if (!player.Update()) {
      rv = WAITING;
      Serial.println("MINUTE->WAITING");
    }
    return rv;
}

bool IsTriggered() {
  bool rv = false;
  uint32_t currTime = millis();
  static uint32_t prevTime = 0;

  static int touchFrames;
  int touchTriggerFrames = 10;
  int currentValue = touchRead(T0);

//  Serial.println(currentValue);
  
  if(currentValue < touchThreshold) {
    touchFrames++;
  } 
  else {
    touchFrames = 0;
  }
  
  if(touchFrames > touchTriggerFrames){ 
    if(currTime - prevTime >= SPEAK_TIMEOUT) {
      prevTime = currTime;
      rv = true;
    }
  }
  
  return rv;
}

SpeechState DoWaiting() {
  SpeechState rv = WAITING;
  
  if(IsTriggered()) {

    // this handles the change from hour to minute. We are done playing the hour.  Now changing states to the SPEAK_MINUTE state.
//    time_hour = LocalTime.hour() % 12;
//    time_minute = LocalTime.minute() / 5 * 5;
    timeDB.UpdateTime();
    time_hour = timeDB.GetHour();
    time_minute = timeDB.GetMinute();  
    Serial.println("Got time: hour " + (String)time_hour + " and minute " + (String)time_minute);
    
    Serial.println("hour " + (String)time_hour);
    player.StartPlaying(hourWav[time_hour]);
    Serial.println("WAITING->HOUR");
    rv = SPEAK_HOUR;
  }
  return rv;
}
