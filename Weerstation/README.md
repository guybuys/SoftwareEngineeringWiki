# Weerstation op een Arduino Uno R4 WiFi

Testsketch voor 4TW. Een BMP280 meet temperatuur (°C) en luchtdruk (hPa). Het bord stuurt die waarden elke 10 seconden als JSON naar EMQX.

Voorbeeldbericht:

```json
{"temp":21.5,"luchtdruk":1013.2}
```

## Invullen

Kopieer `secrets.h.example` naar `secrets.h` als dat bestand er nog niet is. Vul daarin het wifi-netwerk, het IP-adres van EMQX, de MQTT-gebruikersnaam en het wachtwoord in. `secrets.h` blijft op de computer en gaat niet naar GitHub.

Elk bord krijgt een eigen `MQTT_CLIENT_ID` en een eigen `MQTT_TOPIC`, bijvoorbeeld `weerstation/2/meting`.

De sketch gebruikt poort 1883, MQTT met gebruikersnaam en wachtwoord, zonder TLS.

Wifi en MQTT staan in `MqttVerbinding.h`. Die tab laten de leerlingen dicht. In `weerstation_mqtt.ino` lezen ze de sensor en maken ze het JSON-bericht.

## Bibliotheken

In de Arduino IDE, bord **Arduino Uno R4 WiFi**:

- ArduinoMqttClient, gebruikt door `MqttVerbinding.h`
- Adafruit BMP280 Library
- Adafruit Unified Sensor

## Bedrading BMP280

| BMP280 | Uno R4 WiFi |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

Dat is dezelfde indeling als op de Uno R3. Naast AREF staan twee pinnen die op de officiële print SCL en SDA heten. Dat zijn dezelfde draden als A5 en A4. De Qwiic-stekker is een andere bus en hoort niet bij deze sketch.

Zonder sensor zet je bovenaan de sketch `SIMULEER_SENSOR` op `1`. Dan publiceert het bord temperatuur rond 21 °C en luchtdruk rond 1013 hPa, met een kleine stap bij elke meting. Zet die regel op `0` zodra de BMP280 aangesloten is.

De seriële monitor staat op 115200 baud. Daar verschijnt het bericht ook als de broker nog niet bereikbaar is.

## LED-matrix

De 12×8-matrix op de Uno R4 WiFi zit in het bordpakket. Geen extra bibliotheek. `MqttVerbinding.h` zet zelf een symbool:

| Symbool | Betekenis |
|---|---|
| Boogjes | Nog geen wifi |
| Wolk | Wifi wel, de broker niet |
| Vink | Verbonden |
| Uitroepteken | `mqtt.fout()`, bijvoorbeeld geen BMP280 |

In de sketch volstaat `mqtt.fout()` om het uitroepteken te zetten, en `mqtt.wisFout()` om het weg te halen. Daarna toont de matrix weer de verbinding. De sketch doet dat al rond `startSensor()`.

## Een meting toevoegen

In `leesSensor()` lees je de extra sensor. In `maakBericht()` komt er een veld bij in de JSON, bijvoorbeeld `luchtvochtigheid`. De buffer is 160 tekens. Wordt het bericht langer, vergroot dan die buffer.
