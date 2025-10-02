# Scheduler in MicroPython

## Waarom een scheduler?

Een microprocessor kan maar één ding tegelijk uitvoeren. Toch lijkt het vaak alsof hij meerdere taken "tegelijk" doet, zoals sensoren uitlezen, data verzenden en berekeningen uitvoeren. Dit komt door het gebruik van een scheduler.

Een scheduler verdeelt de tijd van de processor over verschillende taken. Elke taak krijgt een eigen interval (hoe vaak hij moet uitgevoerd worden) en de scheduler zorgt ervoor dat elke taak op het juiste moment aan de beurt komt. Zo kun je bijvoorbeeld:
- Om de 5ms een druksensor uitlezen
- Om de 100ms een temperatuursensor uitlezen
- Om de 1s GPS-data verwerken en verzenden

Hierdoor lijkt het alsof alles tegelijk gebeurt, terwijl de processor eigenlijk heel snel tussen de taken wisselt.

## Voordelen van een scheduler
- Je code blijft overzichtelijk: elke taak is een aparte functie.
- Je kunt makkelijk nieuwe taken toevoegen of intervallen aanpassen.
- Je kunt controleren of een taak te lang duurt (en eventueel een waarschuwing geven).
- Je kunt taken laten wachten op een externe gebeurtenis (zoals een interrupt van een GPS-module).

## Voorbeeld code

In [`scheduler_example.py`](https://github.com/guybuys/SoftwareEngineeringWiki/blob/main/scheduler_example.py) zie je hoe je de scheduler gebruikt. De GPS-taak wacht op een *vlag* die door een *interrupt* gezet wordt. In het voorbeeld wordt die vlag handmatig gezet met `uart_interrupt_handler()`.

Je kunt de scheduler makkelijk uitbreiden met je eigen taken en voorwaarden!

###  Uitleg bij de voorbeeldcode

In het voorbeeld worden vier taken toegevoegd aan de scheduler:
- **GPS**: wordt uitgevoerd als er data is ontvangen via een interrupt én minstens 1 seconde sinds de vorige run.
- **Druk**: wordt elke 5ms uitgevoerd.
- **Temperatuur**: wordt elke 100ms uitgevoerd.
- **Verzenden**: wordt elke 1s uitgevoerd.

Elke taak is een aparte functie. De scheduler kijkt steeds of het tijd is om een taak uit te voeren (en of de condition True is, als die er is). Als een taak te lang duurt, krijg je een waarschuwing.


## Scheduler code
De *broncode* van de scheduler staat hier: [`scheduler.py`](https://github.com/guybuys/SoftwareEngineeringWiki/blob/main/scheduler.py). De leerlingen van de richting **ICW** kunnen vlot werken met klassen dus voor hen zou de code begrijpbaar moeten zijn. Voor de anderen is het vooral belangrijk dat je het *concept* van een scheduler begrijpt en dat je er gebruik van kunt maken in je eigen projecten. Vergeet niet de broncode toe te voegen onder hetzelfde *path* van je eigen code. Dit bestand moet dus ook op de **ESP32** of **raspberry pi pico** gezet worden.


## Meer leren?
Experimenteer met verschillende intervallen en taken. Probeer bijvoorbeeld een LED te laten knipperen, een sensor uit te lezen of data te versturen op vaste tijdstippen.

Veel succes!
