/*
 *
 * @file vars.h
 * 
 * by Ross iam@rosscoe.com
 */

#ifndef VARS_H
#define VARS_H

#include <WiFi.h>

#ifdef EEZ
#ifdef RES8048
#include "EEZ/8048/screens.h"
#endif
#ifdef RES8048_LS
#include "EEZ/8048_landscape/screens.h"
#endif
#ifdef RES4827
#include "EEZ/4827/screens.h"
#endif

#endif


//  char* dd_locos;
const char * throttleName[4] = {
  "Throttle A",
  "Throttle B",
  "Throttle C",
  "Throttle D"
};

char locoName[NUM_LOCOS][NAME_LEN +1];
char locoAddress[NUM_LOCOS][ADDR_LEN +1];
uint32_t savedSpeed[NUM_LOCOS];
uint32_t locoSpeed[NUM_LOCOS];
uint8_t locoDir[NUM_LOCOS];
char funcName[NUM_LOCOS][NUM_FUNC_SLOTS][FNAME_LEN +1];
char funcNumber[NUM_LOCOS][NUM_FUNC_SLOTS][FNUM_LEN+1];          // 4 characters to store "255"
uint8_t funcState[NUM_LOCOS][NUM_FUNC_SLOTS];
uint8_t funcOption[NUM_LOCOS][NUM_FUNC_SLOTS];
//uint8_t resumeOnGo = 0;
//uint8_t runState = 1;                                           //1 = Running, 0 = Stoppe




#if defined ESP32DIS02170A || defined ESP32DIS08070H || defined ESP32DIS06043H
uint16_t map_xlate[] = {0, 3, 6, 9, 12, 1, 4, 7, 10, 13};
uint16_t func_xlate[] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};
uint16_t abs_xlate[] = {0, 5, 1, 6, 2, 7, 3, 8, 4, 9};
const char * btnMap_functions[] = {
                          " ", " ", "\n",
                          " ", " ", "\n",
                          " ", " ", "\n",
                          " ", " ", "\n",
                          " ", " ", NULL
                          };
#else
uint16_t map_xlate[] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10};  // this is the order in the map on screen, bottom row first 5 is missing as it's the "\n"
uint16_t func_xlate[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};  // this is the order that the functions slots will be in the map
uint16_t abs_xlate[] =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  const char * btnMap_functions[] = {
    " ", " ", " ", " ", " ", "\n",
    " ", " ", " ", " ", " ", NULL
  };
#endif



uint32_t activeIndex = 0;
uint32_t activeSlot[THROTTLE_COUNT] = {0};
uint8_t activeThrottle = 0;
uint8_t activeLocoID = 255;
uint8_t CallingPage = 0;
uint32_t activeLoco[THROTTLE_COUNT][NUM_LOCO_SLOTS] = {{0},{0}};
uint8_t thNum = 0;
uint16_t locoCount = 0;
uint16_t accCount = 0;
uint16_t routeCount = 0;
uint8_t locosDirty = 0;
uint8_t functionsDirty = 0;
uint8_t throttlesDirty = 0;
uint8_t accessoriesDirty = 0;
uint8_t credentialsDirty = 0;
uint8_t routesDirty = 0;

uint8_t selectedIDs[THROTTLE_COUNT][NUM_LOCO_SLOTS];


uint8_t thresholdSpeed = 15;
uint8_t REAccAmount = 1;

uint8_t brightness = 250;
uint8_t timeout = 20;

unsigned long lastButtonPress = 0;
uint8_t aFlag = 0;
uint8_t bFlag = 0;
int encoderPos = 0;
uint8_t oldEncPos = 0;
uint8_t oldSpeed = 0;
uint8_t reading = 0;
uint8_t old_pos = 0;  //encoderPos;
//boolean buttonState = 0;

uint8_t trackSel = 99; 
uint8_t replyExpected = 0;
  
int long interruptTime = 0;
int long elapsedTime = 0;
int long lastTime = 0;


//String savedActiveAddress;
//uint16_t savedActiveSpeed;
//uint8_t savedActiveDir;
uint16_t guestSpeed = 0;
uint8_t guestDir = 1;          // Default Guest Direction

uint8_t rosterMode = NORMAL_MODE;

uint16_t editingID = 0;
uint8_t functionEditSlot = 0;


enum ScreensEnum callingPage;



WiFiClient client;

//LGFX gfx;
//LGFX *gfx = new(LGFX);



char commandString[MAX_COMMAND_LENGTH+1];

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis = millis();

bool lastA = false;
unsigned long number = 0;
unsigned int lowest = 0;
unsigned int highest = 126; //126;

uint8_t directionFlag = 0;

const uint8_t tCount = 4;

static int foundNetworks = 0;
unsigned long networkTimeout = 10 * 1000;
String ssidList;

struct NwCred
{
  uint16_t id;
  String ssid;
  String password;
  String ipAddress;
  String nwPort;
};

NwCred netwks[NUM_NWKS];

TaskHandle_t ntScanTaskHandler, ntConnectTaskHandler;

String foundWiFiList[NUM_NWKS];


typedef enum {
  NO_NETWORK,
  NETWORK_SEARCHING,
  NETWORK_CONNECTED_POPUP,
  NETWORK_CONNECTED,
  NETWORK_CONNECT_FAILED
} Network_Status_t;

Network_Status_t networkStatus = NO_NETWORK;

/*
 * The Menu_Items need to be in order as they are in the EEZ button matrix.
 */

#if !defined ESP32DIS02170A_LS && !defined ESP32DIS08070H_LS
typedef enum {
  CONFIG,
  ROSTER,
  ACC,
  PROGRAM,
  WIFI,
  ROUTES,
  SAVE,
  RESTORE
 }Menu_Items;
#else
typedef enum {
  ROSTER = 0,
  ACC = 1,
  ROUTES = 2,
  PROGRAM = 3,
  CONFIG = 4,
  WIFI = 5,
 }Menu_Items;

 //  SAVE,
//  RESTORE

 #endif

DCCEXProtocol dccexProtocol;


#endif //VARS_H
