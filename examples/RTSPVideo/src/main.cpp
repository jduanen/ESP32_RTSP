/*****************************************************************************
 *
 * Xiao ESP32-S3 Sense RTSP Test Program
 *
 *****************************************************************************/
#include <Arduino.h>

#include <CRtspSession.h>
#include <OV2640.h>
#include <OV2640Streamer.h>
//#include <WebServer.h>
#include <WiFiClient.h>
#include <WiFiUtilities.h>

#include "myWiFi.h"

//// TODO fix the library properly
//// directly edit these files:
//// .pio/libdeps/seeed_xiao_esp32s3/Micro-RTSP/src/OV2640.h
//// .pio/libdeps/seeed_xiao_esp32s3/Micro-RTSP/src/OV2640.cpp
#include "xiaoSense.h"


const int portNum = 8554;

uint32_t MSEC_PER_FRAME = 100;        // 10 FPS

const char *ssid = WLAN_SSID;
const char *password = WLAN_PASS;

#define APP_NAME "MicroRTSPExample"
#define APP_VERSION "1.0.0"


OV2640 cam;
WiFiUtilities *wifi;
CStreamer *streamer;
CRtspSession *session;
WiFiClient client;          // FIXME, support multiple clients
WiFiServer rtspServer(portNum);


/*
void resetDevice(void) {
    delay(100);
    WiFi.disconnect();
    esp_restart();
}
*/

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    delay(1000);
    Serial.println(String(APP_NAME) + ": BEGIN");

    Serial.println("\n\n##################################");
    Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
    Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
    Serial.printf("ChipRevision %d, Cpu Freq %d, SDK Version %s\n", ESP.getChipRevision(), ESP.getCpuFreqMHz(), ESP.getSdkVersion());
    Serial.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());
    Serial.println("##################################\n\n");

    wifi = new WiFiUtilities(WIFI_STA, ssid, password, -1);

    cam.init(xiaoCam_config);

    rtspServer.begin();

    IPAddress ip = WiFi.localIP();
    String str = "RTSP Usage:\n";
    str += "    Image Stream: rtsp://";
    str += ip.toString();
    str += ":8554/mjpeg/1\n";
    Serial.println(str);

    Serial.println(String(APP_NAME) + ": READY");
}

void loop() {
    static uint32_t lastimage = millis();

    // FIXME support multiple simultaneous clients
    if (session) {
        session->handleRequests(0); // we don't use a timeout here,
        // instead we send only if we have new enough frames

        uint32_t now = millis();
        if((now > (lastimage + MSEC_PER_FRAME)) || (now < lastimage)) { // handle clock rollover
            session->broadcastCurrentFrame(now);
            lastimage = now;

            // check if we are overrunning our max frame rate
            now = millis();
            if(now > (lastimage + MSEC_PER_FRAME)) {
                printf("warning exceeding max frame rate of %d ms\n", (now - lastimage));
            }
        }

        if (session->m_stopped) {
            delete session;
            delete streamer;
            session = NULL;
            streamer = NULL;
            Serial.println("==> DELETED CAM STREAMER & SESSION");
        }
    } else {
        client = rtspServer.accept();
        if (client) {
            streamer = new OV2640Streamer(&client, cam);    // our streamer for UDP/TCP based RTP transport
            session = new CRtspSession(&client, streamer);  // our threads RTSP session and state
            Serial.println("==> CREATED CAM STREAMER & SESSION");
        }
    }
}
