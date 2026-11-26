# Blad-steen-schaar

Je maakt een spel voor twee ESP32-borden. Elk bord heeft drie knoppen en drie NeoPixel-LEDs: blad, steen en schaar. De borden praten niet met elkaar. Een spelserver op de computer van de leerkracht is de scheidsrechter. Jouw bord stuurt alleen wat het zelf doet, en doet wat de server terugstuurt.

Je oefent drie dingen:

1. een klasse voor de knoppen en de LEDs
2. MQTT, met een eigen topic om te sturen en een eigen topic om te ontvangen
3. een toestandsmachine: het bord staat altijd in precies één toestand, en alleen de gebeurtenissen uit de tabel hieronder laten het verspringen

Teken die tabel op papier voor je ze in code zet. Als de tabel klopt, is de code vooral die tabel overtikken.

## Waarom een server

Tussen een bericht versturen en het ontvangen zit ongeveer een halve seconde. Als twee borden elkaar rechtstreeks een keuze sturen, is het lastig om tegelijk te kiezen en eerlijk te vergelijken.

Daarom doet de server de klok en de regels:

- Tijdens het aftellen kies je lokaal, met een knop. Dat gaat niet over MQTT.
- Op het bericht `kies` stuur je meteen de keuze die je al vastgelegd hebt. Je hoeft dan niet meer na te denken.
- De server wacht daarna nog even, zodat een traag netwerk je keuze niet kwijtraakt. Die wachttijd is geen denktijd.

De keuze blijft op jouw bord tot het startschot. De tegenstander ziet ze niet in de berichten die jij tijdens het aftellen zou kunnen sturen, want je stuurt ze dan nog niet.

## Wat jij niet bouwt

De wachtrij, het aftellen, de winnaar en “opnieuw bij gelijk” zitten in de server. Jouw programma reageert op berichten en stuurt op het juiste moment een keuze of `vrij`.

De server laat maximaal vijf borden toe. Zijn er twee vrij, dan start ze een spel met de twee die het langst vooraan in de wachtrij staan. Er kunnen twee spellen tegelijk lopen. De rest wacht. Na een spel met een winnaar gaan beide borden achteraan staan, in de volgorde waarin ze aan dat spel begonnen. Bij gelijk spelen dezelfde twee meteen opnieuw. Na drie gelijke rondjes stopt dat spel toch, en gaan beide borden ook achteraan.

## De klasse `KnoppenLeds`

Deze heb je al zelf gemaakt en heet mogelijk anders. Je kan ze opnieuw gebruiken.

Knop 0 is blad, knop 1 is steen, knop 2 is schaar. Dezelfde volgorde voor de LEDs.

Test deze klasse voor je met WiFi begint. De lus onderaan `main.py` doet dat al: een knop zet de bijbehorende LED aan. Haal die lus weg als dat werkt.

## MQTT

Gebruik jullie WiFi-code van vorige week. Het bord moet op hetzelfde netwerk zitten als de broker.

Elk bord heeft twee topics. Het nummer is het bordnummer, uniek in de klas.

| Richting | Topic | Wie publiceert |
| --- | --- | --- |
| Bord naar server | `BladSteenSchaar/3/status` | jij |
| Server naar bord | `BladSteenSchaar/3/commando` | de server |

Vervang `3` door `BORD_ID`. Schrijf de naam exact zo, met die hoofdletters. Je publiceert alleen op `status`. Je subscribe't alleen op `commando`. Zo krijg je je eigen berichten niet terug, en lees je de keuze van een ander bord niet mee.

Verbinden, in deze volgorde:

```python
client = MQTTClient("bord-{}".format(BORD_ID), BROKER, keepalive=30)
client.set_callback(on_message)
client.connect()
client.subscribe(TOPIC_COMMANDO.encode())
```

De client-id moet uniek zijn. `bord-1` en `bord-2` mogen niet dezelfde id gebruiken.

Publiceren:

```python
client.publish(TOPIC_STATUS.encode(), ujson.dumps({"vrij": True}).encode())
```

Maak een dictionary en laat `ujson.dumps` de tekst maken. Schrijf de JSON niet zelf als een string. `True` in Python wordt `true` in JSON. Met aanhalingstekens erom, `"true"`, begrijpt de server het niet.

Publish zonder retain. Dat is de standaard als je de parameter weg laat. Een retained `vrij` blijft op de broker plakken en de server negeert het.

### Berichten die jij stuurt

Alleen deze twee, en alleen in de toestand die erbij staat.

| Wanneer | Payload |
| --- | --- |
| In `idle`, meteen bij het binnenkomen, en daarna elke 10 seconden | `{"vrij": true}` |
| In `aftellen`, op het moment dat `kies` binnenkomt, en alleen als je een keuze hebt | `{"keuze": "blad"}` |

`"steen"` en `"schaar"` zijn de andere geldige keuzes. Kleine letters. Stuur per rondje hooguit één keuze. De server houdt de eerste.

Stuur `vrij` niet tijdens een spel. De server negeert het dan, en in de log van de leerkracht is te zien dat jouw bord dat toch deed.

### Berichten die jij ontvangt

Elk commando is een dictionary met minstens `"fase"`. Extra velden mag je negeren, behalve de velden die in de tabel staan.

| Payload | Betekenis |
| --- | --- |
| `{"fase": "aftellen", "tel": 3}` | Nieuw rondje. `tel` wordt daarna 2 en dan 1. Bij `tel` 3 wis je de vorige keuze. |
| `{"fase": "kies"}` | Stuur nu je keuze, als je die hebt. |
| `{"fase": "resultaat", "uitslag": "win"}` | Jij wint. Ook `"verlies"` of `"gelijk"`. |
| `{"fase": "wacht"}` | Het spel is voorbij. Ga naar `idle`. |
| `{"fase": "vol"}` | Er spelen al vijf borden. Blijf `vrij` sturen. |

Bij het resultaat zitten ook `"jij"` en `"tegenstander"`. Dat is `"blad"`, `"steen"`, `"schaar"` of `null` als dat bord niets stuurde. Je hebt ze niet nodig voor de basistaak. `"rondje"` is het nummer van het rondje, ook niet verplicht om te gebruiken.

`"wacht"` kan `"reden": "te veel gelijk"` bevatten. Drie gelijke rondjes na elkaar. Ook dan ga je naar `idle`. Je hebt niet gewonnen.

Regels die de server toepast, zodat de LEDs kloppen met wat er gebeurde:

- Blad wint van steen, steen wint van schaar, schaar wint van blad. Dezelfde keuze is gelijk.
- Geen keuze gestuurd: je verliest. Sturen jullie allebei niets, dan is het gelijk en volgt een nieuw rondje.
- Een keuze die vóór `kies` toekomt, telt niet. Een tweede keuze in hetzelfde rondje telt niet.

## De toestandsmachine

Vier toestanden: `idle`, `aftellen`, `wachten`, `resultaat`.

Het bord start in `idle`.

```text
                         knop = keuze vastleggen
                    +----------------------------------+
                    |                                  |
                    v                                  |
+--------+  aftellen   +----------+  kies   +----------+  resultaat  +------------+
|  idle  | ----------> | aftellen | ------> | wachten  | ----------> | resultaat  |
+--------+             +----------+         +----------+             +------------+
    ^                      |                                               |   |
    |                      |                                               |   |
    | wacht                +-------------------- wacht -------------------+   |
    |                                                                          |
    +------------------------------ wacht -------------------------------------+
                                           |
                         resultaat + aftellen (gelijk, tel 3)
                         gaat terug naar aftellen, niet naar idle
```

`wacht` en `vol` mag je vanuit elke toestand krijgen. `wacht` gaat altijd naar `idle`. `vol` blijft in `idle` en zet de LEDs op paars.

Doe een LED-actie op het moment van de overgang, of als er tijdens `aftellen` een knop wordt ingedrukt. Zet niet elke lus opnieuw alle LEDs uit, anders veeg je het paars van `vol` meteen weg.

| Toestand nu | Gebeurtenis | Actie | Nieuwe toestand |
| --- | --- | --- | --- |
| `idle` | net binnengekomen, of 10 s later | publiceer `{"vrij": true}` | `idle` |
| `idle` | `aftellen` | wis de keuze als `tel` 3 is; LEDs dimwit | `aftellen` |
| `idle` | `vol` | LEDs paars | `idle` |
| `idle` | `wacht` | LEDs uit | `idle` |
| `aftellen` | knop 0, 1 of 2 | keuze = die knop; alleen die LED in de keurkleur | `aftellen` |
| `aftellen` | `aftellen` met een nieuwe `tel` | bij `tel` 3 de keuze wissen en de LEDs weer dimwit; anders de `tel` alleen tonen (printen mag) | `aftellen` |
| `aftellen` | `kies` | heb je een keuze en nog niet gestuurd: publiceer ze één keer | `wachten` |
| `wachten` | `resultaat` | alle LEDs groen, rood of oranje, volgens `uitslag` | `resultaat` |
| `resultaat` | `aftellen` | zelfde actie als hierboven bij een nieuw aftellen | `aftellen` |
| elke | `wacht` | LEDs uit | `idle` |
| elke | `vol` | LEDs paars | `idle` |

Kleuren, al ingevuld in `KLEUR`:

| Situatie | Kleur |
| --- | --- |
| Nog geen keuze tijdens het aftellen | wit op alle drie |
| Blad | groen, LED 0 |
| Steen | blauw, LED 1 |
| Schaar | rood, LED 2 |
| Win | groen, alle drie |
| Verlies | rood, alle drie |
| Gelijk | oranje, alle drie |
| `vol` | paars, alle drie |
| `idle` na `wacht` | uit |

In `wachten` laat je de LED van je eigen keuze branden.

Als `kies` binnenkomt en je hebt geen knop geduwd, publiceer je niets en ga je toch naar `wachten`. De server rekent dat als verlies, tenzij de tegenstander ook niets stuurde.

Bij gelijk blijf je uit `idle`. Je stuurt dan geen `vrij`. Het volgende bericht is opnieuw `aftellen` met `tel` 3. Wis dan de oude keuze, zodat een nieuw rondje een nieuwe druk vraagt.

## De hoofdlus

`umqtt` levert een bericht in een callback. Vanuit die callback mag je niet publiceren: de bibliotheek kan daar niet tegen. De callback stopt het bericht in een lijst. De lus haalt de lijst leeg en doet het werk.

```python
inbox = []

def on_message(topic, msg):
    inbox.append(ujson.loads(msg.decode()))

def lus(client, bord):
    while True:
        client.check_msg()          # niet wait_msg(); die blokkeert de knoppen
        while inbox:
            verwerk(inbox.pop(0), bord)   # op volgorde
        # heartbeat of knoppen, afhankelijk van de toestand
        time.sleep_ms(20)
```

Eén overgang als voorbeeld. De andere rijen schrijf je op dezelfde manier, vanuit de tabel.

```python
vrij_meteen = True

def verwerk(bericht, bord):
    global toestand, keuze, keuze_verstuurd, vrij_meteen
    fase = bericht.get("fase")

    if fase == "wacht":
        toestand = "idle"
        vrij_meteen = True
        bord.uit()
        return

    if toestand == "idle" and fase == "aftellen":
        toestand = "aftellen"
        if bericht.get("tel") == 3:
            keuze = None
            keuze_verstuurd = False
            bord.zet_alle(KLEUR["wit"])
        return
```

In `idle` stuurt de lus `vrij`, niet de callback. Is `vrij_meteen` gezet, dan meteen één keer, en daarna pas als `time.ticks_diff` minstens 10000 ms is. Zet `vrij_meteen` uit en zet de timer op het moment van verzenden, anders vertrekt elke 20 ms een bericht.

## Een rondje, in de tijd

Bord 1 en bord 2 hebben allebei `{"vrij": true}` gestuurd.

| Tijd | Wat er gebeurt |
| --- | --- |
| 0,0 s | Server stuurt beide `{"fase": "aftellen", "tel": 3}` |
| 1,0 s | `tel` 2 |
| 2,0 s | `tel` 1. Ondertussen duwt iemand op een knop. Dat blijft lokaal. |
| 3,0 s | Server stuurt `{"fase": "kies"}` |
| 3,4 s | Bord 1 publiceert `{"keuze": "blad"}` |
| 3,6 s | Bord 2 publiceert `{"keuze": "schaar"}` |
| 3,6 s | Beide keuzes zijn binnen. Schaar wint van blad. Server stuurt het resultaat. |
| 6,6 s | Server stuurt `{"fase": "wacht"}`. Beide borden gaan naar `idle` en sturen meteen weer `vrij`. |

De seconden hierboven zijn de klok van de server. Op het bord komt hetzelfde bericht ongeveer een halve seconde later aan. Beide borden hebben dezelfde vertraging, dus het aftellen loopt ongeveer gelijk.

## Volgorde van bouwen

1. `KnoppenLeds`. Zonder netwerk: knop 0 maakt LED 0 groen, knop 1 LED 1 blauw, knop 2 LED 2 rood.
2. WiFi en MQTT. Alleen `idle`: `vrij` sturen. Je nummer moet in de terminal van de server verschijnen. Laat dit even lopen en controleer dat je niet tientallen berichten per seconde stuurt.
3. `verwerk`: de overgangen, eerst met `print(toestand)` en pas daarna met LEDs.
4. Tijdens `aftellen` de knop onthouden en tonen.
5. Op `kies` één keer de keuze publiceren.
6. Het resultaat tonen, en op `wacht` terug naar `idle` met de LEDs uit.
7. Gelijk: op het nieuwe aftellen de oude keuze wissen en niet naar `idle` gaan.

Test stap 2 met één bord. Test stap 5 met twee borden. Daarna pas met de hele klas, zodat je de wachtrij ziet.

## Klaar?

- Elke knop steekt alleen de bijbehorende LED aan, en een lange druk telt als één keuze.
- In `idle` vertrekt `{"vrij": true}` ongeveer elke 10 seconden, en meteen nadat een spel eindigt.
- Tijdens een spel vertrekt geen `vrij`.
- Een keuze vertrekt één keer, en pas na `kies`.
- Na winst of verlies gaan de LEDs uit en wil het bord opnieuw spelen.
- Na gelijk begint het aftellen opnieuw op hetzelfde bord, met een lege keuze.
- `wacht` en `vol` werken ook als je het vorige bericht gemist hebt.
- Twee borden met hetzelfde `BORD_ID` komen niet voor.

## Fouten die vaak gebeuren

- `wait_msg()` gebruiken. De lus staat dan stil en de knoppen doen niets. Gebruik `check_msg()`.
- Publiceren vanuit `on_message`. Bewaar het bericht en publiceer in de lus.
- `{"vrij": true}` in elke doorgang van de lus sturen. Eén keer per 10 seconden is genoeg.
- De keuze pas kiezen als `kies` al binnen is. Die tijd is er niet: de knop moet tijdens het aftellen.
- De keuze al tijdens `tel` 2 publiceren. De server gooit die weg.
- Bij gelijk naar `idle` gaan. Wacht op het volgende `aftellen`.
- De keuze van het vorige rondje laten staan. Wis ze als `tel` 3 is.
- JSON met de hand schrijven, of `True` tussen aanhalingstekens zetten.
- Retain aanzetten.
- `time.time()` op het bord gebruiken voor de 10 seconden. Gebruik `ticks_ms` en `ticks_diff`.

## Als je vroeg klaar bent

Toon na het resultaat ook de keuze van de tegenstander. Die staat in het veld `tegenstander`. Bijvoorbeeld eerst een seconde jouw kleur, dan een seconde de zijne, en dan pas het groen of rood van de uitslag. De server stuurt die velden al; je hoeft niets aan het protocol te veranderen.
