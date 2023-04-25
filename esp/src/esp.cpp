#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "ArduinoJson.h"
#include "AsyncJson.h"

AsyncWebServer server(80);

const size_t MAX_TRAJ_LENGTH = 1024;
const size_t NUM_SERVOS = 16;

typedef struct
{
    float seconds;
    uint8_t positions[NUM_SERVOS];
} trajframe_t;
static trajframe_t traj[MAX_TRAJ_LENGTH];
static size_t traj_length = 1000;
static size_t traj_index = 0;
static float traj_starttime = 0;

const char *ssid = "CMU-DEVICE";
const char *password = "";

volatile bool modifying_traj = false;
volatile bool running_traj = false;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    // Serial.print("ESP Board MAC Address:  ");
    // Serial.println(WiFi.macAddress());
    // Serial.print("IP Address: ");
    // Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/plain", "Hello, world"); });

    server.on("/set-traj-len", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("length", true)) {
            String length = request->getParam("length", true)->value();
            int len = length.toInt();
            if (len >= MAX_TRAJ_LENGTH) {
                request->send(400, "text/plain", "Length too high.");
            } else {
                while (modifying_traj)
                    ;
                modifying_traj = true;
                traj_length = len;
                modifying_traj = false;
                request->send(200, "text/plain", "OK");
            }
        } else {
            request->send(400, "text/plain", "BAD REQUEST");
        }
    });

    server.on("/start", HTTP_POST, [](AsyncWebServerRequest *request) {
        while (modifying_traj)
            ;
        modifying_traj = true;
        traj_index = 0;
        traj_starttime = (float)micros() / 1000000.0;
        running_traj = true;
        modifying_traj = false;
        request->send(200, "text/plain", "OK");
    });

    server.on("/stop", HTTP_POST, [](AsyncWebServerRequest *request) {
        while (modifying_traj)
            ;
        modifying_traj = true;
        running_traj = false;
        modifying_traj = false;
        request->send(200, "text/plain", "OK");
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/set-traj", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("index", true) && request->hasParam("timestamp", true) && request->hasParam("positions", true)) {
            String index = request->getParam("index", true)->value();
            String timestamp = request->getParam("timestamp", true)->value();
            String positions = request->getParam("positions", true)->value();

            trajframe_t frame;
            frame.seconds = timestamp.toFloat();
            StaticJsonDocument<256> doc;
            deserializeJson(doc, positions);
            for (size_t i = 0; i < NUM_SERVOS; i++) {
                frame.positions[i] = doc[i];
            }
            int ind = index.toInt();
            if (ind >= MAX_TRAJ_LENGTH) {
                request->send(400, "text/plain", "Index too high.");
            } else {
                while (modifying_traj)
                    ;
                modifying_traj = true;
                traj[ind] = frame;
                modifying_traj = false;

                // Serial.println(ind);
                // Serial.println(frame.seconds);
                // for (int i = 0; i < NUM_SERVOS; i++) {
                //     Serial.print(frame.positions[i]);
                //     Serial.print(" ");
                // }
                // Serial.println();

                request->send(200, "text/plain", "OK");
            }
        } else {
            request->send(400, "text/plain", "BAD REQUEST");
        }
    });

    server.onNotFound(notFound);

    server.begin();

    traj_starttime = (float)micros() / 1000000.0;
}

void loop() {
    if (!running_traj) {
        return;
    }
    while (modifying_traj)
        ;
    modifying_traj = true;
    float current_time = (float)micros() / 1000000.0;
    if (current_time - traj_starttime > traj[traj_index].seconds) {
        // Serial.println(current_time);
        uint8_t buf[19];
        buf[0] = 0xC0;
        buf[1] = 0xFF;
        for (int i = 0; i < NUM_SERVOS; i++) {
            buf[2 + i] = traj[traj_index].positions[i];
        }
        buf[18] = 0xEE;
        Serial.write(buf, 19);

        traj_index++;
        if (traj_index >= traj_length) {
            traj_index = 0;
            traj_starttime = current_time;
        }
    }
    modifying_traj = false;
}