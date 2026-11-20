# Inleiding tot decorators
## Wat is een decorator?

Een decorator is een **speciale functie** in Python die een **andere functie** *aanpast* of *extra gedrag toevoegt*, zonder dat je de oorspronkelijke functie hoeft te veranderen.

Een decorator herken je aan het `@`-teken boven een functie:
```python
@decorator_naam
def mijn_functie():
    ...
```

Python voert dit eigenlijk zo uit:
```python
def mijn_functie():
    ...

mijn_functie = decorator_naam(mijn_functie)
```

Met andere woorden: de decorator **krijgt de functie als argument**, en **geeft een aangepaste versie ervan** terug.
Zo kun je gemakkelijk functionaliteit toevoegen, zoals:

- loggen,
- meten van tijd,
- rechtencontrole of 
- reageren op gebeurtenissen (zoals bij mBlock).

## Een eenvoudig voorbeeld

Hier is een zelfgemaakte decorator die meet hoe lang een functie duurt:
```python
import time

def meet_tijd(func):
    def wrapper():
        start = time.time()
        func()
        einde = time.time()
        print(f"Duur: {einde - start:.3f} s")
    return wrapper

@meet_tijd
def hallo():
    print("Hallo wereld!")

hallo()
```

Wanneer je `hallo()` aanroept, voert Python eigenlijk `wrapper()` uit.
Die functie:

1. start een timer,
2. roept `hallo()` aan,
3. drukt de duur af.

## Decorators en events

Decorators **registreren gedrag** of **wijzigen de werking** van een functie.

In mBlock (bijv. `@event.is_press('b')`) gebruikt de onderliggende bibliotheek de decorator om:

- de functie te registreren als callback voor een bepaald event,

- en later de functie automatisch uit te voeren wanneer dat event optreedt (zoals het indrukken van een knop).

De *eventlistener* draait in de **mBlock-runtime**, niet in Python zelf.
De decorator is enkel het mechanisme waarmee die registratie gebeurt.

## Decorators en FastAPI

FastAPI gebruikt decorators om Python-functies te koppelen aan **URL's en HTTP-methodes**. Zo'n koppeling heet een *route* of *endpoint*.

```python
from fastapi import FastAPI

app = FastAPI()

@app.get("/")
def startpagina():
    return {"bericht": "Hallo wereld!"}
```

De regel `@app.get("/")` vertelt FastAPI: “Als de server een `GET`-verzoek ontvangt op `/`, gebruik dan de functie `startpagina`.” FastAPI registreert die functie als endpoint wanneer de applicatie wordt ingeladen. De functie wordt dus niet meteen uitgevoerd door de decorator; ze wordt uitgevoerd wanneer er een passend verzoek binnenkomt.

Je kunt de applicatie bijvoorbeeld starten met:

```bash
uvicorn main:app --reload
```

Hierbij staat `main` voor het Python-bestand `main.py` en `app` voor de FastAPI-applicatie in dat bestand. Daarna kun je de route `/` bezoeken, bijvoorbeeld via `http://127.0.0.1:8000/`.

Het idee lijkt op mBlock: in beide gevallen koppelt een decorator een functie aan iets waarop later gereageerd wordt. Het verschil is wat de functie activeert:

- Bij **mBlock** is dat een event, zoals het indrukken van een knop.
- Bij **FastAPI** is dat een HTTP-verzoek naar een URL met een bepaalde methode, zoals `GET`.

De FastAPI-server ontvangt en verwerkt het verzoek; de decorator legt bij het opstarten vast welke functie bij welke route hoort.

## Decorators in MicroPython

**Decorators werken ook in MicroPython** — het is een standaardonderdeel van de Python-taal (sinds versie 2.4).
Er zijn wel enkele **beperkingen**:
- Decorators die veel geheugen gebruiken of complexe objecten aanmaken zijn minder geschikt.
- Je kunt geen decorators gebruiken die afhankelijk zijn van modules die niet in MicroPython bestaan (zoals `functools.lru_cache`).
- Voor embedded toepassingen worden decorators vaak gebruikt voor **callbacks** (bijv. pin-interrupts, timers, of MQTT-events).

Voorbeeld in MicroPython:
```python
def bij_timer(func):
    def wrapper():
        print("Timerfunctie:")
        func()
    return wrapper

@bij_timer
def tik():
    print("tik!")

tik()
```
## Decorators en schedulers

Een decorator lijkt op het eerste zicht op een **scheduler**, omdat beide bepalen **wanneer** of **hoe** een functie wordt uitgevoerd.
Het verschil:

| Aspect | Decorator | Scheduler |
|:--|:--|:--|
| Wat het doet | Past gedrag van een functie aan | Roept een functie op een bepaald moment of interval aan |
| Wanneer actief | Zodra de code wordt ingeladen | Tijdens runtime, volgens planning of events |
| Typisch gebruik | Logging, caching, toegang, events | Periodieke taken, timers, achtergrondprocessen |

Toch is de verwantschap er wel:
Een scheduler zou intern ook **decorators** kunnen gebruiken om taken te registreren, net zoals mBlock dat doet met `@event.is_press()`.

## Samenvatting

- Een **decorator** is een functie die een andere functie aanpast of uitbreidt.
- Ze worden aangeduid met het `@`-teken boven een functie.
- Decorators **starten zelf geen events**, maar kunnen worden gebruikt om een functie aan een eventlistener te **koppelen**.
- In **FastAPI** koppelt een decorator zoals `@app.get("/")` een functie aan een route; FastAPI voert die functie uit als een passend HTTP-verzoek binnenkomt.
- MicroPython ondersteunt decorators volledig, al zijn complexe toepassingen minder geschikt door beperkte middelen.
- Het idee van “iets uitvoeren op het juiste moment” verbindt decorators conceptueel met schedulers.
