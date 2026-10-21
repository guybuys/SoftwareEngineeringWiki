# Heaps in Python: van lijst tot heap sort

Uitbreidingsmodule voor leerlingen secundair onderwijs die al kunnen werken met Python, klassen en basis-sorteeralgoritmes.

## Wat ga je leren?

Na deze module kan je:

- uitleggen wat een **heap** is en waarom ze handig is;
- een heap voorstellen als boom én als gewone Python-lijst;
- een willekeurige lijst omvormen tot een heap (**heapify**);
- zelf een **heap sort** schrijven;
- een eigen klasse `MaxHeap` maken met `toevoegen` en `haal_grootste_op`.

## Wat moet je al kunnen?

- Lijsten, `for`- en `while`-lussen, functies;
- twee elementen van plaats wisselen: `a[i], a[j] = a[j], a[i]`;
- het idee van een sorteeralgoritme (bv. bubble sort of selection sort);
- basis van klassen (`__init__`, methodes).

Deze module is bedoeld om in **2 tot 3 lesuren** door te nemen. Neem de tijd voor de papieren oefeningen: wie de heap op papier begrijpt, schrijft de code veel sneller.

## 1. Het probleem: snel het grootste element vinden

Stel dat je steeds het grootste getal uit een lijst wil halen, telkens opnieuw, terwijl er ook nieuwe getallen bijkomen. Denk aan een wachtrij op een spoedafdeling: de patiënt met de hoogste prioriteit komt eerst.

Met een gewone lijst heb je twee keuzes:

- de lijst altijd gesorteerd houden: invoegen is traag;
- de lijst niet sorteren: het grootste element zoeken kost telkens `n` stappen.

Een **heap** is een slimme tussenoplossing: ze is *niet volledig gesorteerd*, maar het grootste element staat altijd vooraan, en de heap blijft snel aan te passen.

## 2. Wat is een heap?

Een heap kan je voorstellen als een **binaire boom**: elk bolletje (knoop) heeft hoogstens twee kinderen, een linker- en een rechterkind.

Voor een **max-heap** gelden twee regels:

1. **Vorm:** de boom is van boven naar onder en van links naar rechts volledig gevuld. Enkel op de laatste rij kunnen er plaatsen rechts open zijn.
2. **Heap-eigenschap:** elke ouder is **groter dan of gelijk aan** zijn kinderen.

Gevolg: de grootste waarde staat altijd bovenaan, in de **wortel**.

Voorbeeld van een max-heap:

```
        50
       /  \
     30    40
    /  \   /
  10   20 35
```

Controleer: 50 ≥ 30 en 40; 30 ≥ 10 en 20; 40 ≥ 35. Klopt.

Let op: de heap is **niet** gesorteerd. 30 staat links van 40, en toch staat 35 (onder 40) lager dan 20 (onder 30) niet in volgorde. Enkel ouder-kind telt, niet "links tegenover rechts".

Bij een **min-heap** is het omgekeerd: elke ouder is kleiner dan of gelijk aan zijn kinderen, en de kleinste waarde staat bovenaan. In deze module werken we met max-heaps.

### Denkvragen

1. Is de boom `10 → (20, 5)` een geldige max-heap? Waarom (niet)?
2. Waar in een max-heap kan het kleinste element zich bevinden?

## 3. Een boom opslaan in een gewone lijst

Het mooie aan een heap: je hebt **geen aparte boomstructuur** of knoop-klasse nodig. Omdat de boom altijd netjes van links naar rechts is opgevuld, kan je hem rij per rij in een lijst zetten.

De heap hierboven wordt:

```python
heap = [50, 30, 40, 10, 20, 35]
#index:  0   1   2   3   4   5
```

De rijen van de boom worden achter elkaar gezet: rij 1 is `50`, rij 2 is `30, 40`, rij 3 is `10, 20, 35`.

De familierelaties volgen uit simpele formules (index `i`, we starten te tellen bij 0):

| Wie? | Formule |
| --- | --- |
| linkerkind van `i` | `2 * i + 1` |
| rechterkind van `i` | `2 * i + 2` |
| ouder van `i` | `(i - 1) // 2` |

Controleer met het voorbeeld: het element op index 1 (30) heeft kinderen op index 3 en 4 (10 en 20). Klopt! Het element op index 5 (35) heeft als ouder index `(5 - 1) // 2 = 2` (40). Klopt ook.

### Oefening 1: familieleden in code

Schrijf drie kleine functies:

```python
def links(i):
    ...

def rechts(i):
    ...

def ouder(i):
    ...
```

Test ze met de lijst hierboven. Schrijf daarna een functie `is_max_heap(lijst)` die `True` teruggeeft als de lijst voldoet aan de heap-eigenschap.

> **Hint:** loop over elk element `i` vanaf index 1 en vergelijk `lijst[i]` met `lijst[ouder(i)]`.

## 4. Het kernidee: een element laten zakken

Alle heap-algoritmes steunen op één bouwsteen. Stel dat de wortel een te kleine waarde heeft, maar de rest van de heap is wel in orde:

```
        4          ← te klein!
       / \
     10   3
    /  \
   5    1
```

Herstel in drie stappen:

1. Vergelijk het element met zijn **grootste kind**.
2. Is het kind groter? Wissel van plaats.
3. Herhaal op de nieuwe plaats, tot het element groter is dan beide kinderen of geen kinderen meer heeft.

Hier: 4 wisselt met 10, en daarna wisselt 4 met 5. Het element is als het ware naar beneden **gezakt**. Dit heet in het Engels *sift down* of *heapify down*.

Resultaat:

```
       10
       / \
      5   3
     / \
    4   1
```

### In Python

```python
def laat_zakken(lijst, n, i):
    """Laat lijst[i] zakken tot de heap-eigenschap hersteld is.
    Enkel de eerste n elementen van de lijst tellen mee als heap."""
    while True:
        grootste = i
        l = 2 * i + 1
        r = 2 * i + 2

        if l < n and lijst[l] > lijst[grootste]:
            grootste = l
        if r < n and lijst[r] > lijst[grootste]:
            grootste = r

        if grootste == i:          # ouder is al groter dan zijn kinderen
            break

        lijst[i], lijst[grootste] = lijst[grootste], lijst[i]
        i = grootste               # ga verder op de nieuwe plaats
```

Waarom is de parameter `n` nodig? Straks (bij heap sort) zal een deel van de lijst al "klaar" zijn en mag dat deel niet meer meetellen als heap. Met `n` bepaal je hoe groot de heap momenteel is. De controles `l < n` en `r < n` zorgen dat we niet buiten de heap kijken.

### Oefening 2

1. Test `laat_zakken` op de lijst `[4, 10, 3, 5, 1]` met `n = 5` en `i = 0`. Wat verwacht je dat er gebeurt? Klopt het met wat je print?
2. Voeg `print(lijst)` toe in de lus om elke wissel te volgen.

## 5. Een lijst heapify-en

Nu willen we een **willekeurige lijst** omvormen tot een max-heap. Neem `[4, 10, 3, 5, 1]`.

Het idee: laat elk element zakken, maar begin **achteraan** en werk naar voren.

- Elementen in de laatste rij hebben geen kinderen. Ze zijn al een (piepkleine) heap en moeten niets doen.
- Het laatste element dat wél kinderen heeft, staat op index `n // 2 - 1`.
- Vanaf daar werk je terug naar index 0 en roep je telkens `laat_zakken` op.

```python
def heapify(lijst):
    n = len(lijst)
    for i in range(n // 2 - 1, -1, -1):
        laat_zakken(lijst, n, i)
```

### Volg het voorbeeld stap voor stap

Lijst: `[4, 10, 3, 5, 1]`, dus `n = 5` en we starten bij `i = 5 // 2 - 1 = 1`.

| Stap | i | Wat gebeurt er? | Lijst daarna |
| --- | --- | --- | --- |
| 1 | 1 | 10 heeft kinderen 5 en 1, en is groter: niets te doen | `[4, 10, 3, 5, 1]` |
| 2 | 0 | 4 heeft kinderen 10 en 3: wissel met 10 | `[10, 4, 3, 5, 1]` |
| 3 | 1 | 4 heeft kinderen 5 en 1: wissel met 5 | `[10, 5, 3, 4, 1]` |

Eindresultaat: `[10, 5, 3, 4, 1]`. Teken de boom en controleer zelf de heap-eigenschap.

**Waarom werkt het van achter naar voren?** Als je `laat_zakken` op index `i` oproept, zijn beide deelbomen onder `i` al in orde. Dat is precies de voorwaarde waaronder `laat_zakken` correct werkt.

### Oefening 3 (eerst op papier!)

Heapify de lijst `[2, 9, 7, 6, 5, 8]` met pen en papier. Teken telkens de boom. Controleer daarna met je code.

## 6. Heap sort

Met een max-heap kan je nu een lijst sorteren. Het idee is verrassend eenvoudig:

1. Maak van de lijst een max-heap. Het grootste element staat nu vooraan.
2. **Wissel** het eerste element (het grootste) met het laatste element van de heap. Het grootste element staat nu op zijn definitieve plaats achteraan.
3. **Verklein de heap met 1**: dat laatste element telt niet meer mee.
4. De nieuwe wortel is waarschijnlijk te klein: laat hem **zakken** zodat de heap hersteld is.
5. Herhaal stap 2 tot 4 tot de heap nog maar 1 element bevat.

Je haalt dus telkens het grootste element uit de heap en zet het achteraan. De lijst wordt zo van achter naar voor gevuld met de grootste elementen, en eindigt oplopend gesorteerd.

```python
def heap_sort(lijst):
    n = len(lijst)
    heapify(lijst)                         # stap 1

    for einde in range(n - 1, 0, -1):
        lijst[0], lijst[einde] = lijst[einde], lijst[0]   # stap 2
        laat_zakken(lijst, einde, 0)       # stap 3 en 4: heap is nu `einde` groot
```

Test:

```python
getallen = [4, 10, 3, 5, 1]
heap_sort(getallen)
print(getallen)    # [1, 3, 4, 5, 10]
```

### Het voorbeeld doorlopen

Na `heapify`: `[10, 5, 3, 4, 1]`.

| einde | Na de wissel | Na het zakken (de gesorteerde staart is **vet**) |
| --- | --- | --- |
| 4 | `[1, 5, 3, 4, 10]` | `[5, 4, 3, 1, 10]` |
| 3 | `[1, 4, 3, 5, 10]` | `[4, 1, 3, 5, 10]` |
| 2 | `[3, 1, 4, 5, 10]` | `[3, 1, 4, 5, 10]` |
| 1 | `[1, 3, 4, 5, 10]` | `[1, 3, 4, 5, 10]` |

De lijst is gesorteerd. Voeg zelf `print(lijst)` toe in de lus en vergelijk met de tabel.

### Oefening 4

1. Laat `heap_sort` aflopend sorteren. Welke kleine aanpassing is nodig? (Denk aan min-heap.)
2. Test je functie op: een lege lijst, een lijst met 1 element, een al gesorteerde lijst, een omgekeerd gesorteerde lijst en een lijst met dubbele waarden.
3. Vergelijk je resultaat telkens met `sorted(lijst)`.

## 7. Een eigen klasse: `MaxHeap`

Tot nu werkten we met losse functies. Een heap wordt vooral gebruikt als **datastructuur** waaraan je dingen toevoegt en het grootste element uit haalt. Dat past perfect in een klasse.

Daarvoor hebben we een tweede bouwsteen nodig: een nieuw element **laten stijgen**.

- **Toevoegen:** zet het element achteraan de lijst (de enige plaats waar de vorm van de boom behouden blijft). Vergelijk het met zijn ouder: is het groter, wissel dan. Herhaal tot het niet meer groter is dan zijn ouder, of tot het de wortel is.
- **Grootste halen:** neem het eerste element. Zet het laatste element op de eerste plaats, verwijder het laatste, en laat de nieuwe wortel zakken.

```python
class MaxHeap:
    def __init__(self):
        self.data = []

    def __len__(self):
        return len(self.data)

    def is_leeg(self):
        return len(self.data) == 0

    def toevoegen(self, waarde):
        self.data.append(waarde)
        self._laat_stijgen(len(self.data) - 1)

    def haal_grootste_op(self):
        if self.is_leeg():
            raise IndexError("De heap is leeg")
        grootste = self.data[0]
        laatste = self.data.pop()
        if not self.is_leeg():
            self.data[0] = laatste
            laat_zakken(self.data, len(self.data), 0)
        return grootste

    def _laat_stijgen(self, i):
        while i > 0:
            ouder = (i - 1) // 2
            if self.data[i] > self.data[ouder]:
                self.data[i], self.data[ouder] = self.data[ouder], self.data[i]
                i = ouder
            else:
                break
```

Gebruik:

```python
h = MaxHeap()
for x in [4, 10, 3, 5, 1]:
    h.toevoegen(x)

while not h.is_leeg():
    print(h.haal_grootste_op())   # 10, 5, 4, 3, 1
```

Merk op: door telkens het grootste op te halen, krijg je de getallen aflopend gesorteerd. Dit is eigenlijk heap sort in een andere vorm!

### Oefening 5

1. Schrijf een methode `bekijk_grootste(self)` die het grootste element teruggeeft *zonder* het te verwijderen.
2. Maak een klasse `Spoedafdeling` die patiënten opslaat met een prioriteit. Gebruik tuples zoals `(prioriteit, naam)`: Python vergelijkt tuples eerst op het eerste element.
3. Maak een `MinHeap`. Wat moet je enkel aanpassen?

## 8. Hoe snel is dit eigenlijk?

Een heap met `n` elementen is een boom met ongeveer `log₂(n)` rijen. Een element laten zakken of stijgen kost dus hoogstens ongeveer zoveel wissels.

| Aantal elementen | Aantal rijen in de heap |
| --- | --- |
| 1 000 | 10 |
| 1 000 000 | 20 |
| 1 000 000 000 | 30 |

Daardoor:

- **toevoegen** en **grootste ophalen** kosten ongeveer `log n` stappen;
- **heapify** van een hele lijst kost ongeveer `n` stappen;
- **heap sort** kost ongeveer `n · log n` stappen, ook in het slechtste geval.

Vergelijk met bubble sort of selection sort, die ongeveer `n²` stappen nodig hebben. Bij 1 000 000 elementen is dat het verschil tussen ongeveer 20 miljoen en 1 000 miljard stappen.

### Oefening 6 (meten is weten)

Meet met de module `time` hoe lang `heap_sort` en jouw eigen `bubble_sort` of `selection_sort` erover doen voor lijsten van 1 000, 5 000 en 20 000 willekeurige getallen (`random.randint`). Maak een tabel van je resultaten. Wat valt op wanneer de lijst 5 keer langer wordt?

## 9. Voor later: `heapq`, de heap van Python zelf

Python heeft een ingebouwde module voor heaps. Nu je begrijpt wat er onder de motorkap gebeurt, kan je ze met vertrouwen gebruiken:

```python
import heapq

getallen = [4, 10, 3, 5, 1]
heapq.heapify(getallen)          # lijst wordt een heap
heapq.heappush(getallen, 7)      # toevoegen
print(heapq.heappop(getallen))   # kleinste ophalen: 1
```

Belangrijk: `heapq` is een **min-heap**. De kleinste waarde komt eerst. Wil je toch een max-heap, dan sla je de getallen negatief op (`-x`).

De kleine "onder de motorkap"-code uit deze module doet in essentie hetzelfde als `heapq`, maar dan voor max-heaps en met Nederlandse namen.

## 10. Samenvatting

- Een **heap** is een binaire boom die van boven naar onder en links naar rechts volledig gevuld is, waarbij elke ouder groter is dan zijn kinderen (max-heap).
- Een heap sla je op in een **gewone lijst**. Kinderen van `i`: `2i + 1` en `2i + 2`. Ouder van `i`: `(i - 1) // 2`.
- **Laten zakken** herstelt de heap als de wortel (of een ander element) te klein is.
- **Heapify** = van achter naar voren (vanaf `n // 2 - 1`) elk element laten zakken.
- **Heap sort** = heapify, dan herhaald het grootste achteraan zetten en de heap met 1 verkleinen.
- Een **prioriteitswachtrij** is de typische toepassing van een heap.

## 11. Uitdagingen voor wie meer wil

1. **Visualiseren:** schrijf een functie die een heap als boom in de terminal afdrukt (rij per rij).
2. **Stap voor stap:** toon bij heap sort na elke wissel de lijst, en markeer het gesorteerde deel.
3. **Top-k:** gegeven een lijst met 1 000 000 getallen, geef de 10 grootste. Kan je dit met een heap van maar 10 elementen oplossen? (Tip: gebruik een min-heap.)
4. **Dijkstra of Huffman:** zoek op hoe heaps gebruikt worden in kortste-padalgoritmes of bij bestandscompressie.
5. **Stabiliteit:** is heap sort *stabiel*? Zoek uit wat dat betekent en test het met tuples.
