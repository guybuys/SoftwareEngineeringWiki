# Ringbuffer

Een ringbuffer bewaart de laatste metingen van de lijn. Is hij vol, dan overschrijft de nieuwste meting de oudste. Zo blijft het geheugen klein, ook als de robot lang rijdt.

Je gebruikt hem om terug te kijken: wat was de lijnstatus een paar rondes geleden, en hoe groot was de afwijking toen. Dat heb je nodig bij een stippellijn of een drempel. Eén meting van nú zegt daar te weinig over.

## Code

Zet `import array` bij de andere imports. Plak de klasse daaronder, vóór de rest van het programma.

```python
import array

class RingBuffer:
    SCALE = 10  # afwijking bewaard als int(afwijking * 10)

    def __init__(self, size=32):
        self.size = size
        self.ints = array.array('B', [0] * size)    # lijnstatus 0-15
        self.floats = array.array('h', [0] * size)  # afwijking * SCALE
        self.pos = 0
        self.count = 0

    def clear(self):
        self.pos = 0
        self.count = 0

    def add(self, i, f):
        self.ints[self.pos] = i
        self.floats[self.pos] = int(round(f * self.SCALE))
        self.pos = (self.pos + 1) % self.size
        if self.count < self.size:
            self.count += 1

    def get(self, offset=0):
        # offset 0 = nieuwste meting, 1 = de ronde ervoor, enzovoort
        idx = (self.pos - 1 - offset) % self.size
        return self.ints[idx], self.floats[idx] / self.SCALE

    def average(self, start_offset=0, num_samples=None):
        # gemiddelde afwijking, vanaf start_offset terug in de tijd
        if num_samples is None:
            num_samples = self.size
        available = self.count - start_offset
        if available <= 0:
            return 0.0
        n = min(num_samples, available)
        total = 0
        idx = (self.pos - 1 - start_offset) % self.size
        for _ in range(n):
            total += self.floats[idx]
            idx = (idx - 1) % self.size
        return total / (n * self.SCALE)
```

Maak één buffer aan, naast `base_power` en `kp`. Het getal is hoeveel metingen hij bewaart.

```python
buffer = RingBuffer(32)
```

## Wat je bewaart

Elke ronde van de rijlus lees je de sensoren en leg je één meting vast:

```python
line_status = mbuild.quad_rgb_sensor.get_line_sta("all", 1)
deviation = mbuild.quad_rgb_sensor.get_offset_track(1)
buffer.add(line_status, deviation)
```

`add` vraagt twee dingen:

- `line_status`: het getal 0 tot 15 van `get_line_sta("all", 1)`
- `deviation`: de afwijking van `get_offset_track`, van −100 tot +100

De afwijking wordt intern met 10 vermenigvuldigd en als geheel getal bewaard. `get` deelt weer door 10, dus je krijgt één decimaal terug.

## Teruglezen

`get(offset)` geeft twee waarden: de status en de afwijking.

| Aanroep | Wat je krijgt |
|---|---|
| `buffer.get(0)` | de meting van deze ronde |
| `buffer.get(1)` | één ronde eerder |
| `buffer.get(3)` | drie rondes eerder |

```python
status_nu, afwijking_nu = buffer.get(0)
status_toen, afwijking_toen = buffer.get(3)
```

`average` geeft alleen de gemiddelde afwijking, niet de status.

```python
gemiddelde = buffer.average(0, 5)
```

Dat is het gemiddelde van de nieuwste meting en de vier daarvoor. Zijn er nog geen vijf metingen, dan gemiddelde hij er zoveel als er al zijn.

## Lege plaatsen zijn 0

De buffer begint vol nullen. `get` kijkt niet of een plaats al een echte meting heeft gehad. Vóór de vierde `add` geeft `get(3)` dus status 0 en afwijking 0, ook als de robot op de lijn staat.

Kijk naar `buffer.count`. Dat is het aantal echte metingen, tot de buffer vol is. Daarna blijft het gelijk aan de grootte.

```python
if buffer.count > 3:
    status_toen, afwijking_toen = buffer.get(3)
```

Roep `buffer.clear()` aan als een nieuwe rit begint, samen met het zetten van `base_power` en `kp`. Anders zitten de metingen van de vorige rit nog in de buffer. `clear` zet de telstanden terug. De oude getallen blijven in het geheugen staan, maar `count` zorgt dat je ze negeert zolang je `count` gebruikt.

## Voorbeelden in de lus

Stippellijn: de status is nu 0, maar een paar rondes geleden lag de robot nog in het midden (status 6) met een kleine afwijking. Dan is het gat kort. Een status 0 na een al grote afwijking is eerder een bocht waar de lijn kwijt is.

```python
if buffer.count > 3:
    status_toen, afwijking_toen = buffer.get(3)
    if line_status == 0 and status_toen == 6 and afwijking_toen < 15 and afwijking_toen > -15:
        mbot2.straight(10)
```

Drempel: status 15 die meteen weer weg is, is iets anders dan status 15 die blijft. Bewaar de status en kijk de volgende rondes of hij 15 blijft, in plaats van op de eerste 15 meteen een manoeuvre te starten.

```python
if buffer.count > 2:
    vorige, _ = buffer.get(1)
    twee_terug, _ = buffer.get(2)
    if line_status == 15 and vorige == 15 and twee_terug == 15:
        pass  # hier de drempel of het moeras
```

Gemiddelde afwijking: één meting springt soms. Het gemiddelde van de laatste vijf rondes beweegt trager.

```python
if buffer.count >= 5:
    gemiddelde = buffer.average(0, 5)
```

## Grootte

32 metingen is een korte geschiedenis: de laatste rondes van de lus, niet de hele tegel. Voor “wat deed de lijn net?” is dat genoeg. Een grotere buffer mag, zolang de CyberPi bij de start geen geheugenfout geeft.

Zet geen `println`, geen led en geen kleurmeting in de rijlus om de buffer te bekijken. Die aanroepen maken de lus traag en de metingen komen dan met ongelijke tussenpozen binnen. Lees de buffer in de lus, en toon een waarde pas op het scherm als de robot stilstaat.
