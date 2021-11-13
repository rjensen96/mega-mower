/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

//https://randomnerdtutorials.com/esp32-websocket-server-arduino/

#pragma once

// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer
#include <AsyncTCP.h> // https://github.com/me-no-dev/AsyncTCP
#include "motor-driver.hpp"

// Replace with your network credentials
const char* ssid = "Refuge";
const char* password = "For the birds!";

bool ledState = 0;
const int ledPin = 2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void notifyClients(String message) {
  ws.textAll(message);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == 0x2) {
    /* BINARY MESSAGES */
    // these are numbers 0 - 8 inclusive. 
    // represents the direction the vehicle should be going.
    // todo: consider skipping long chains of commands. if length is huge, then there's a backlog and we shouldn't execute backlogs.
    // also, at some point we should set motor state to zero if it's been a long time since the last websocket command.
    for(int i = 0; i < len; i++) {
      Serial.print(data[i]);
      Serial.println();

      uint8_t newDirection = data[i];
      handleDriveCommand(newDirection);
      
    }
  } else if (info->final && info->index == 0 && info->len == len && info->opcode == 0x1) { 
      /* TEXT MESSAGES */
      Serial.println((char*)data); // currently no purpose for text messages.
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.begin();
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
}
