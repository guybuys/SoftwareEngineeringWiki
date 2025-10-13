
# Introductie in MQTT

**MQTT** is een *lichtgewicht* **protocol** voor berichtenuitwisseling, ontworpen voor **IoT** (Internet of Things), sensornetwerken en andere situaties waar bandbreedte, energieverbruik en betrouwbaarheid belangrijk zijn.

## Hoe werkt het?
Stel je voor dat je je *abonneert* op een **YouTube-kanaal**. Je hoeft niet steeds te kijken of er een nieuwe video is: zodra er iets nieuws verschijnt, krijg je automatisch een melding. Zo werkt MQTT ook:

- **Publishen** is zoals een YouTuber die een nieuwe video uploadt. Iedereen die geabonneerd is (subscribed) op dat kanaal (topic) krijgt het bericht.
- **Subscriben** betekent dat je aangeeft welke onderwerpen (topics) je wilt volgen. Je krijgt alleen berichten over die onderwerpen.

De **broker** of centrale MQTT server is als YouTube zelf: hij zorgt dat alles bij de juiste mensen terechtkomt.

## Herkomst van de naam
MQTT staat voor "Message Queuing Telemetry Transport". Het protocol werd in 1999 ontwikkeld door Andy Stanford-Clark (IBM) en Arlen Nipper (Arcom, nu Eurotech) om olie- en gasleidingen op afstand te monitoren.

### QoS (Quality of Service)
Dit bepaalt hoe zeker je bent dat een bericht aankomt:
- **QoS 0 (at most once):** Bericht wordt één keer verstuurd, zonder bevestiging. Snel, maar niet gegarandeerd dat het aankomt.
- **QoS 1 (at least once):** Broker bevestigt ontvangst. Bericht kan soms dubbel aankomen, maar je weet zeker dat het er is.
- **QoS 2 (exactly once):** Broker en client zorgen dat het bericht precies één keer aankomt. Dit is het meest betrouwbaar, maar ook het traagst.

### Last Will and Testament (LWT)
Stel je voor dat een apparaat plots offline gaat (bv. door stroomuitval). Je kunt vooraf een "laatste boodschap" instellen. Als het apparaat onverwacht verdwijnt, stuurt de broker die boodschap automatisch naar een gekozen topic. Zo weten anderen dat er iets mis is.

### Clean Session
Als je "Clean Session" aanzet, vergeet de broker alles over jouw vorige verbindingen. Handig als je telkens opnieuw wilt beginnen. Zet je het uit, dan onthoudt de broker welke topics je volgde en welke berichten je nog moest krijgen. Dit is handig voor apparaten die soms offline zijn.

## Typische toepassingen
- IoT: sensoren, actuatoren, domotica
- Messaging tussen microservices
- Remote monitoring

## Voordelen
- Lichtgewicht en efficiënt
- Werkt goed op trage of onbetrouwbare netwerken
- Eenvoudig te implementeren

## Nadelen
- Geen ingebouwde encryptie. (TLS gebruiken is mogelijk).
- Minder geschikt voor grote berichten

### TLS en beveiliging
Berichten via MQTT zijn standaard niet versleuteld. Iedereen op het netwerk kan ze in principe lezen. Wil je dat berichten privé blijven, gebruik dan **TLS** (Transport Layer Security). Dit werkt zoals https bij websites: alles wordt versleuteld tussen client en broker.

- Standaard MQTT gebruikt poort **1883** (zonder encryptie).
- Voor beveiligde verbindingen met TLS wordt meestal poort **8883** gebruikt.

Let op: TLS moet zowel op de broker als op de client ingesteld worden.

## Gebruikte termen
- **Broker**: De centrale server die berichten ontvangt, opslaat en doorstuurt naar clients. Alle communicatie verloopt via de broker.
- **Client**: Een apparaat of applicatie die verbinding maakt met de broker. Kan berichten publiceren en/of ontvangen.
- **Publish**: Het versturen van een bericht naar een bepaald topic op de broker.
- **Subscribe**: Het inschrijven op een topic om berichten te ontvangen die daarop gepubliceerd worden.
- **Topic**: Een hiërarchische naam (bv. `sensor/temperatuur`) die gebruikt wordt om berichten te organiseren en te routeren. Je kunt wildcards gebruiken:
  - `+` (plus): één niveau wildcard, bv. `sensor/+` ontvangt alles zoals `sensor/temp`, `sensor/vocht`.
  - `#` (hekje): alle volgende niveaus, bv. `sensor/#` ontvangt alles wat begint met `sensor/`, zoals `sensor/temp/keuken`.
- **Payload**: De inhoud van het bericht (meestal tekst of JSON).
- **QoS (Quality of Service)**: Geeft aan hoe betrouwbaar de aflevering van berichten moet zijn. Zie uitleg hieronder.
- **Retain**: Een flag waarmee een bericht op een topic bewaard blijft, zodat nieuwe subscribers het direct ontvangen.
- **Last Will and Testament (LWT)**: Een bericht dat automatisch door de broker wordt verstuurd als een client onverwacht de verbinding verliest. Zie uitleg hieronder.
- **Clean Session**: Geeft aan of de broker de state van de client moet onthouden tussen verbindingen. Zie uitleg hieronder.

## Meer weten?
Zie [mqtt.org](https://mqtt.org/) voor documentatie en voorbeelden.

## Praktisch voorbeeld
Zie het bestand `mqtt_example.py` in deze repository voor een Python-voorbeeld.