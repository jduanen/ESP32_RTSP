#include <Arduino.h>

#include <CRtspSession.h>
#include <OV2640.h>
#include <OV2640Streamer.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <WiFiUtilities.h>


#define VERBOSE         1
#define APP_NAME        "MicroRTSPExample"
#define APP_VERSION     "1.0.0"


extern WiFiUtilities *wifi;
extern OV2640 cam;


void resetDevice(void);
String usage(String str);

void initRTSPServer(void);
void runRTSPServer(void);
void stopRTSPServer(void);
