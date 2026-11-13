// Meegeleverd. Deze tab niet wijzigen.
// Wifi en MQTT naar EMQX. De sketch leest de sensor en maakt het bericht.
//
// De LED-matrix op de Uno R4 WiFi toont de toestand:
//   boogjes        geen wifi
//   wolk           wifi wel, broker niet
//   vink           verbonden
//   uitroepteken   mqtt.fout(), bijvoorbeeld geen BMP280
// mqtt.wisFout() haalt het uitroepteken weg. Daarna komt de verbinding terug.

#pragma once

#include <WiFiS3.h>
#include <ArduinoMqttClient.h>
#include <Arduino_LED_Matrix.h>

#include "secrets.h"

// 1 = led aan. Rij voor rij, 12 kolommen.
static const uint8_t BEELD_WIFI[8][12] = {
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
};

static const uint8_t BEELD_BROKER[8][12] = {
    {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
};

static const uint8_t BEELD_OK[8][12] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
};

static const uint8_t BEELD_FOUT[8][12] = {
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
};

class MqttVerbinding {
 public:
  MqttVerbinding() : mqtt(wifi) {}

  void begin() {
    matrix.begin();
    toonStatus();
    verbindWifi();
    verbindMqtt();
  }

  void houdOpen() {
    if (WiFi.status() != WL_CONNECTED) {
      verbindWifi();
    }
    if (!mqtt.connected()) {
      verbindMqtt();
    }
    mqtt.poll();
    toonStatus();
  }

  // Zet een uitroepteken op de matrix. De verbinding blijft op de achtergrond.
  void fout() {
    eigenFout = true;
    toonStatus();
  }

  // Haal het uitroepteken weg. De matrix toont weer wifi, broker of vink.
  void wisFout() {
    eigenFout = false;
    getoond = Niets;
    toonStatus();
  }

  bool verstuur(const char *bericht) {
    if (!mqtt.connected()) {
      return false;
    }

    mqtt.beginMessage(MQTT_TOPIC);
    mqtt.print(bericht);
    mqtt.endMessage();
    return true;
  }

 private:
  enum Toestand { Niets, WifiWeg, BrokerWeg, Verbonden, EigenFout };

  WiFiClient wifi;
  MqttClient mqtt;
  ArduinoLEDMatrix matrix;
  bool eigenFout = false;
  Toestand getoond = Niets;

  void verbindWifi() {
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("WiFi-module van de Uno R4 niet gevonden. Update de firmware via het Arduino-bordmenu.");
      toonStatus();
      return;
    }

    Serial.print("Verbinden met WiFi ");
    Serial.println(WIFI_SSID);
    toonStatus();

    while (WiFi.begin(WIFI_SSID, WIFI_WACHTWOORD) != WL_CONNECTED) {
      Serial.println("WiFi lukt nog niet, nieuwe poging...");
      toonStatus();
      delay(2000);
    }

    Serial.print("WiFi verbonden, IP-adres van dit bord: ");
    Serial.println(WiFi.localIP());
    toonStatus();
  }

  void verbindMqtt() {
    mqtt.setId(MQTT_CLIENT_ID);
    mqtt.setUsernamePassword(MQTT_GEBRUIKER, MQTT_WACHTWOORD);

    Serial.print("Verbinden met EMQX op ");
    Serial.print(MQTT_BROKER);
    Serial.print(":");
    Serial.println(MQTT_POORT);
    toonStatus();

    if (!mqtt.connect(MQTT_BROKER, MQTT_POORT)) {
      Serial.print("MQTT-verbinding mislukt, foutcode ");
      Serial.println(mqtt.connectError());
      toonStatus();
      delay(2000);
      return;
    }

    Serial.println("MQTT verbonden");
    toonStatus();
  }

  void toonStatus() {
    Toestand nu = Verbonden;
    if (eigenFout) {
      nu = EigenFout;
    } else if (WiFi.status() != WL_CONNECTED) {
      nu = WifiWeg;
    } else if (!mqtt.connected()) {
      nu = BrokerWeg;
    }

    if (nu == getoond) {
      return;
    }
    getoond = nu;

    switch (nu) {
      case WifiWeg:
        teken(BEELD_WIFI);
        break;
      case BrokerWeg:
        teken(BEELD_BROKER);
        break;
      case EigenFout:
        teken(BEELD_FOUT);
        break;
      default:
        teken(BEELD_OK);
        break;
    }
  }

  void teken(const uint8_t beeld[8][12]) {
    uint8_t kopie[8][12];
    memcpy(kopie, beeld, sizeof(kopie));
    matrix.renderBitmap(kopie, 8, 12);
  }
};
