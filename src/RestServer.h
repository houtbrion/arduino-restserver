#ifndef restserver_h
#define restserver_h

#include "RestSettings.h"
#include "Log.h"

#ifdef USE_WIFI
#if defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_MKRVIDOR4000)
#include <WiFiNINA.h>
#endif /* UNO_WIFI etc. */
#include <WiFi.h>
#endif /* USE_WIFI */


#ifndef USE_WIFI
#ifdef USE_UIPETHERNET
#include <UIPEthernet.h>
#else /* USE_UIPETHERNET */
#include <Ethernet.h>
#endif /* USE_UIPETHERNET */
#endif /* USE_WIFI */

#ifdef ARDUINO_ARCH_SAMD
#include <avr/dtostrf.h>
#endif /* CPU_ARCH==SAMD_ARCH */

struct Routes {
  char * method;
  char * name;
  void (*callback)(char * params);
};

class RestServer {
public:
#ifdef USE_WIFI
  RestServer(WiFiServer& client);
#else /* USE_WIFI */
  RestServer(EthernetServer& client);
#endif /* USE_WIFI */
  
  void run();
  
  void addRoute(char * method, char * route, void (*f)(char *));
  
  void addData(char* name, String& value);
  void addData(char* name, uint16_t value);
  void addData(char* name, int value);
  void addData(char* name, float value);
  void addData(char* name, char* value);

private:
  Routes routes_[ROUTES_TOTAL];
  uint8_t routesIndex_;
  char buffer_[OUTPUT_BUFFER_SIZE];
  uint16_t bufferIndex_;

#ifdef USE_WIFI
  WiFiServer& server_;
  WiFiClient client_;
#else /* USE_WIFI */  
  EthernetServer& server_;
  EthernetClient client_;
#endif /* USE_WIFI */  

  void check();
  void reset();
  void addToBuffer(char * value);
  void send(uint8_t chunkSize, uint8_t delayTime);
};

#endif
