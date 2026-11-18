# Binaire bomen in Python: oefenreeks

Deze reeks leert je werken met **binaire bomen** in Python: bouwen, doorlopen, zoeken en (op het einde) de stap naar een **heap**.

**Wat moet je al kunnen?** Variabelen, lijsten, `if`/`while`/`for`, functies, en zelf een eenvoudige **klasse** schrijven (met `__init__`, attributen en methodes).

**Oplossingen** staan in een apart bestand: [bomen-oplossingen.md](bomen-oplossingen.md). Probeer eerst zelf, kijk pas daarna!

Moeilijkheid: ⭐ = basis, ⭐⭐ = middel, ⭐⭐⭐ = uitdaging.

---

## Inhoud

0. [Wat is een binaire boom?](#deel-0-wat-is-een-binaire-boom)
1. [Bomen bouwen en benoemen](#deel-1-bomen-bouwen-en-benoemen)
2. [Recursie op bomen](#deel-2-recursie-op-bomen)
3. [Een boom doorlopen](#deel-3-een-boom-doorlopen)
4. [De binaire zoekboom](#deel-4-de-binaire-zoekboom)
5. [Beslissingsbomen en een raadspel](#deel-5-beslissingsbomen-en-een-raadspel)
6. [De brug naar de heap](#deel-6-de-brug-naar-de-heap)

---

## Deel 0: Wat is een binaire boom?

Een **boom** is een datastructuur waarin gegevens **hiërarchisch** georganiseerd zijn (niet na elkaar zoals in een lijst). Een boom bestaat uit **knopen** die verbonden zijn door **takken**.

```
        8            <- wortel (niveau 0)
       / \
      3   10         <- niveau 1
     / \    \
    1   6    14      <- niveau 2
       / \   /
      4   7 13       <- niveau 3 (diepte van de boom = 3)
```

| Begrip | Betekenis |
|---|---|
| **knoop** | een element van de boom, met een waarde |
| **wortel** | de bovenste knoop, die geen ouder heeft (er is altijd precies één) |
| **ouder / kind** | de knoop net boven / net onder een knoop |
| **blad** | een knoop zonder kinderen |
| **interne knoop** | een knoop die geen blad is (heeft minstens één kind) |
| **niveau** van een knoop | de afstand tot de wortel (de wortel heeft niveau 0) |
| **diepte** van de boom | het grootste niveau dat voorkomt |
| **pad** | een opeenvolging van knopen van de wortel naar een blad |
| **binaire boom** | elke knoop heeft **maximaal twee** kinderen: een **linkerkind** en een **rechterkind** |

### De klasse `Knoop`

Python heeft geen ingebouwde boom, dus maken we er zelf een. Eén klasse volstaat:

```python
class Knoop:
    def __init__(self, waarde, links=None, rechts=None):
        self.waarde = waarde
        self.links = links      # het linkerkind (of None)
        self.rechts = rechts    # het rechterkind (of None)
```

- `None` betekent: *"hier is geen knoop"*.
- Een **blad** is dus gewoon een knoop met `links = None` en `rechts = None`.
- De parameters `links` en `rechts` hebben een beginwaarde `None`, dus `Knoop(5)` maakt een blad.
- We stellen een boom voor door **zijn wortelknoop**. De hele boom bereik je via de wortel.

Voorbeeld: een boom met wortel `2` en twee bladeren `1` en `3`.

```python
mini = Knoop(2, Knoop(1), Knoop(3))
print(mini.waarde)          # 2
print(mini.links.waarde)    # 1
print(mini.rechts.waarde)   # 3
```

### Hulpcode om een boom te tekenen

Zet deze code **bovenaan elk oefenbestand**. Je hoeft ze niet te begrijpen, ze print een boom als ingesprongen tekst (`L` = linkerkind, `R` = rechterkind).

```python
class Knoop:
    def __init__(self, waarde, links=None, rechts=None):
        self.waarde = waarde
        self.links = links
        self.rechts = rechts


def teken(knoop):
    """Print een boom als ingesprongen tekst."""
    print(knoop.waarde)
    _teken_kinderen(knoop, "")


def _teken_kinderen(knoop, prefix):
    kinderen = [("L", knoop.links), ("R", knoop.rechts)]
    kinderen = [(zijde, kind) for zijde, kind in kinderen if kind is not None]
    for i, (zijde, kind) in enumerate(kinderen):
        laatste = i == len(kinderen) - 1
        print(prefix + ("└─ " if laatste else "├─ ") + zijde + ": " + str(kind.waarde))
        _teken_kinderen(kind, prefix + ("   " if laatste else "│  "))
```

---

## Deel 1: Bomen bouwen en benoemen

We gebruiken in de volgende delen telkens dezelfde **voorbeeldboom**:

```
        8
       / \
      3   10
     / \    \
    1   6    14
       / \   /
      4   7 13
```

### Oefening 1.1 ⭐: Begrippen (op papier)

Beantwoord voor de voorbeeldboom:

1. Wat is de wortel?
2. Welke knopen zijn bladeren?
3. Welke knopen zijn interne knopen?
4. Wat is het niveau van de knoop `6`? En van `13`?
5. Wat is de diepte van de boom?
6. Geef het pad van de wortel naar het blad `7`.
7. Is dit een binaire boom? Waarom?

### Oefening 1.2 ⭐: De voorbeeldboom bouwen

Bouw de voorbeeldboom in Python met de klasse `Knoop`. Sla de wortel op in een variabele `boom` en print hem met `teken(boom)`.

*Tip:* bouw "van onder naar boven". Maak eerst de bladeren, en geef die dan mee als kinderen. Een ontbrekend kind geef je mee als `None`.

Verwachte uitvoer:

```
8
├─ L: 3
│  ├─ L: 1
│  └─ R: 6
│     ├─ L: 4
│     └─ R: 7
└─ R: 10
   └─ R: 14
      └─ L: 13
```

### Oefening 1.3 ⭐: Wandelen door de boom

Gebruik de variabele `boom` uit 1.2. Schrijf **één expressie** (bv. `boom.links.waarde`) die de volgende waarden geeft:

1. `10`
2. `6`
3. `13`
4. `7`

### Oefening 1.4 ⭐: Is het een blad?

Schrijf een functie `is_blad(knoop)` die `True` teruggeeft als de knoop een blad is, en anders `False`.

```python
print(is_blad(boom))                # False
print(is_blad(boom.links.links))    # True  (de knoop 1)
```

### Oefening 1.5 ⭐: Een eigen boom

Bouw deze boom met strings:

```
        dier
       /    \
  zoogdier  vogel
   /    \
 hond   kat
```

Welke expressie geeft je `"kat"`, vertrekkend vanaf de wortel?

---

## Deel 2: Recursie op bomen

Een boom heeft een bijzondere eigenschap: **elk kind is zelf de wortel van een kleinere boom**. Daarom werken we met bomen bijna altijd met **recursie** (een functie die zichzelf oproept).

Het patroon is telkens hetzelfde:

1. **Basisgeval:** is de knoop `None`? Dan is er geen boom en geef je direct het antwoord voor een lege boom.
2. **Los het probleem op** voor de linkerdeelboom en voor de rechterdeelboom (*vertrouw erop dat dit werkt!*).
3. **Combineer** de resultaten met de waarde van de huidige knoop.

### Voorbeeld: het aantal knopen tellen

```python
def grootte(knoop):
    if knoop is None:                 # 1. basisgeval: lege boom heeft 0 knopen
        return 0
    links = grootte(knoop.links)      # 2. tel links
    rechts = grootte(knoop.rechts)    #    tel rechts
    return 1 + links + rechts         # 3. combineer: deze knoop + links + rechts

print(grootte(boom))   # 9
```

> **Denk zo:** *"Het aantal knopen van een boom is 1 (de wortel) plus het aantal knopen links plus het aantal rechts."*

### Oefening 2.1 ⭐: Som

Schrijf `som(knoop)` die de som van alle waarden in de boom teruggeeft (we gaan uit van getallen).

```python
print(som(boom))   # 66
```

### Oefening 2.2 ⭐⭐: Bladeren tellen

Schrijf `aantal_bladeren(knoop)`. *Let op:* hier heb je **twee** basisgevallen: een lege boom (`None`) en een blad.

```python
print(aantal_bladeren(boom))   # 4
```

### Oefening 2.3 ⭐⭐: Diepte

Schrijf `diepte(knoop)` die de diepte van de boom teruggeeft. Dit is het grootste niveau van een blad.

Spreek af: een boom met **één knoop** heeft diepte `0` en een lege boom (`None`) heeft diepte `-1`. Gebruik `max(...)`.

```python
print(diepte(boom))        # 3
print(diepte(Knoop(5)))    # 0
```

### Oefening 2.4 ⭐⭐: Zit het erin?

Schrijf `bevat(knoop, waarde)` die `True` teruggeeft als de waarde ergens in de boom voorkomt. (De boom is hier niet gesorteerd, je moet dus overal kijken.)

```python
print(bevat(boom, 7))   # True
print(bevat(boom, 5))   # False
```

### Oefening 2.5 ⭐⭐: Maximum

Schrijf `maximum(knoop)` die de grootste waarde in de boom teruggeeft. Je mag veronderstellen dat de boom niet leeg is. *Let op:* een knoop kan een kind missen.

```python
print(maximum(boom))   # 14
```

### Oefening 2.6 ⭐⭐: Spiegelbeeld

Schrijf `spiegel(knoop)` die een **nieuwe** boom teruggeeft waarin overal links en rechts verwisseld zijn.

```python
teken(spiegel(boom))
```

Verwachte uitvoer:

```
8
├─ L: 10
│  └─ L: 14
│     └─ R: 13
└─ R: 3
   ├─ L: 6
   │  ├─ L: 7
   │  └─ R: 4
   └─ R: 1
```

### Oefening 2.7 ⭐⭐⭐: Op welk niveau?

Schrijf `niveau_van(knoop, waarde, niveau=0)` die het niveau teruggeeft van de knoop met die waarde, of `None` als de waarde niet voorkomt.

```python
print(niveau_van(boom, 13))   # 3
print(niveau_van(boom, 8))    # 0
print(niveau_van(boom, 99))   # None
```

---

## Deel 3: Een boom doorlopen

Een lijst doorloop je van voor naar achter. Bij een boom zijn er meerdere logische volgordes. De drie klassieke (*diepte-eerst*) volgordes verschillen enkel in **wanneer je de waarde van de knoop zelf neemt**:

| Volgorde | Eerst… | …dan… | …dan… |
|---|---|---|---|
| **preorder** | de knoop zelf | links | rechts |
| **inorder** | links | de knoop zelf | rechts |
| **postorder** | links | rechts | de knoop zelf |

### Oefening 3.1 ⭐⭐: Voorspellen

Voorspel **op papier** de preorder-, inorder- en postorder-volgorde van de voorbeeldboom. Programmeer ze daarna in 3.2 en controleer.

### Oefening 3.2 ⭐⭐: Drie volgordes

Schrijf `preorder(knoop)`, `inorder(knoop)` en `postorder(knoop)`. Elk geeft een **lijst** van waarden terug. Basisgeval: een lege boom geeft een lege lijst `[]`.

*Tip:* lijsten kan je samenvoegen met `+`.

```python
print(preorder(boom))    # [8, 3, 1, 6, 4, 7, 10, 14, 13]
print(inorder(boom))     # [1, 3, 4, 6, 7, 8, 10, 13, 14]
print(postorder(boom))   # [1, 4, 7, 6, 3, 13, 14, 10, 8]
```

> Valt je iets op aan de inorder-lijst van deze boom? Hou dat in je achterhoofd voor Deel 4!

### Oefening 3.3 ⭐⭐⭐: Niveau per niveau

Schrijf `per_niveau(knoop)` die de waarden teruggeeft van boven naar onder en per niveau van links naar rechts. Hier werkt recursie minder goed. Gebruik een **wachtrij** (`collections.deque`):

1. Steek de wortel in de wachtrij.
2. Zolang de wachtrij niet leeg is: haal de voorste knoop eruit, noteer zijn waarde en steek zijn kinderen (als die bestaan) achteraan.

```python
print(per_niveau(boom))   # [8, 3, 10, 1, 6, 14, 4, 7, 13]
```

### Oefening 3.4 ⭐⭐: Rekenen met een boom

Wiskundige uitdrukkingen kan je als boom voorstellen: de **interne knopen** zijn operatoren en de **bladeren** zijn getallen. De uitdrukking `3 * 4 + 8 * 5` ziet er zo uit:

```
        +
       / \
      *   *
     / \ / \
    3  4 8  5
```

1. Bouw deze boom in een variabele `uitdrukking`.
2. Schrijf `bereken(knoop)` die het resultaat uitrekent. Is de knoop een blad, geef dan zijn waarde. Anders reken je eerst links en rechts uit (dat is postorder!) en pas je dan de operator toe (`'+'`, `'-'`, `'*'` of `'/'`).
3. Schrijf `naar_tekst(knoop)` die de uitdrukking met haakjes teruggeeft (inorder).
4. Bouw de boom voor `(2 + 3) * 4`. Wat valt je op aan de vorm van de boom ten opzichte van `2 + 3 * 4`?

```python
print(bereken(uitdrukking))      # 52
print(naar_tekst(uitdrukking))   # ((3 * 4) + (8 * 5))
```

### Oefening 3.5 ⭐⭐⭐: Alle paden

Een **pad** loopt van de wortel naar een blad. Schrijf `alle_paden(knoop)` die een **lijst van paden** teruggeeft (elk pad is een lijst van waarden).

```python
print(alle_paden(boom))
# [[8, 3, 1], [8, 3, 6, 4], [8, 3, 6, 7], [8, 10, 14, 13]]
```

*Tip:* als de knoop een blad is, is `[[knoop.waarde]]` het antwoord. Anders neem je de paden van elk kind en zet je de waarde van de huidige knoop vooraan.

### Oefening 3.6 ⭐⭐⭐: Wedden dat…

Je gooit drie keer een muntstuk. Heb je **twee keer of meer kop**, dan betaalt je vriend een drankje. Anders betaal jij.

1. Schrijf `muntboom(worpen, waarde=None)` die een boom bouwt met alle mogelijke uitkomsten. De wortel heeft waarde `None`, elke andere knoop heeft de waarde `"kop"` of `"munt"`. Het **linkerkind** is steeds `"kop"` en het **rechterkind** `"munt"`. Een boom van `worpen = 0` is één blad.
2. Gebruik `alle_paden` op `muntboom(3)` en tel in hoeveel paden er minstens twee keer `"kop"` voorkomt.
3. Is het een eerlijk spel?

*Tip:* `pad.count("kop")` telt hoe vaak `"kop"` in een lijst staat.

---

## Deel 4: De binaire zoekboom

Een **binaire zoekboom** (*binary search tree*, BST) is een binaire boom waarin de waarden **geordend** staan. Voor **elke** knoop geldt:

- alle waarden in de **linkerdeelboom** zijn **kleiner** dan de waarde van de knoop;
- alle waarden in de **rechterdeelboom** zijn **groter**.

(Om het eenvoudig te houden laten we geen dubbele waarden toe.)

De voorbeeldboom uit de vorige delen *is* een zoekboom. Zoeken naar `7` gaat zo: `7 > 8`? Nee, dus links, naar `3`. `7 > 3`, dus rechts, naar `6`. `7 > 6`, dus rechts, en je vindt `7`. Je moest slechts **4 knopen** bekijken, en je **hoefde de rest van de boom niet te bekijken**. Dat is veel sneller dan een hele lijst te overlopen.

### Oefening 4.1 ⭐⭐: Zoeken

Schrijf `zoek(knoop, waarde)` die `True` of `False` teruggeeft. Gebruik de ordening: ga **enkel naar links of enkel naar rechts**, nooit naar allebei.

```python
print(zoek(boom, 7))   # True
print(zoek(boom, 5))   # False
```

### Oefening 4.2 ⭐⭐: Toevoegen

Schrijf `voeg_toe(knoop, waarde)` die een waarde op de juiste plaats toevoegt en de **wortel van de boom teruggeeft**.

- Is `knoop` gelijk aan `None`? Dan geef je een nieuwe `Knoop(waarde)` terug.
- Is de waarde kleiner? Dan wordt het resultaat van `voeg_toe` op de linkerdeelboom het nieuwe linkerkind. Analoog voor groter en rechts.
- Is de waarde al aanwezig? Doe niets.
- Geef op het einde de knoop zelf terug.

Gebruik het zo om een boom op te bouwen vanaf een lege boom:

```python
zoekboom = None
for getal in [8, 3, 10, 1, 6, 14, 4, 7, 13]:
    zoekboom = voeg_toe(zoekboom, getal)

teken(zoekboom)   # dezelfde boom als de voorbeeldboom!
```

### Oefening 4.3 ⭐⭐: Telkens een andere boom

Bouw met `voeg_toe` een zoekboom uit elk van deze lijsten, en bepaal telkens de `diepte`:

```python
a = [5, 2, 8, 1, 4, 7, 9]
b = [1, 2, 4, 5, 7, 8, 9]
c = [9, 8, 7, 5, 4, 2, 1]
d = [5, 2, 8, 4, 1, 9, 7]
```

1. Voorspel eerst de dieptes. Teken `a` en `b` op papier.
2. Wat merk je op? Waarom is `b` een slechte zoekboom?
3. Hoe zou je de getallen toevoegen om een zo ondiepe boom te krijgen?

### Oefening 4.4 ⭐⭐: Sorteren met een boom

Bouw een zoekboom van `[5, 2, 8, 1, 4, 7, 9]` en print `inorder(...)` (uit Oefening 3.2). Wat valt je op?

Schrijf daarmee `boomsort(lijst)` die een lijst van getallen **gesorteerd** teruggeeft: bouw een BST en geef de inorder-lijst terug.

```python
print(boomsort([5, 3, 5, 1]))   # [1, 3, 5]   (dubbels verdwijnen)
```

### Oefening 4.5 ⭐⭐: Kleinste en grootste

Schrijf `minimum_bst(knoop)` en `maximum_bst(knoop)` **zonder recursie en zonder alle knopen te bekijken**. Waar moet je de kleinste en grootste waarde zoeken in een zoekboom?

```python
print(minimum_bst(zoekboom))   # 1
print(maximum_bst(zoekboom))   # 14
```

### Oefening 4.6 ⭐⭐⭐: Hoeveel sneller?

Schrijf `zoek_stappen(knoop, waarde)` die een tuple `(gevonden, aantal_knopen_bekeken)` teruggeeft (**zonder recursie**, met een `while`-lus).

Vergelijk daarna een lijst met een zoekboom met 1000 willekeurige getallen:

```python
import random

random.seed(1)
getallen = random.sample(range(1, 100000), 1000)

lijst = getallen
boom_1000 = None
for g in getallen:
    boom_1000 = voeg_toe(boom_1000, g)

print(diepte(boom_1000))
```

Zoek 200 willekeurige getallen (de meeste zitten er niet in). Tel hoeveel elementen je gemiddeld moet bekijken in de lijst (dat is de lengte van de lijst als het getal er niet in zit) en hoeveel knopen in de boom. Wat besluit je?

---

## Deel 5: Beslissingsbomen en een raadspel

In een **beslissingsboom** staan op de interne knopen **vragen** en in de bladeren **antwoorden**. Bij een ja/nee-vraag krijgen we een binaire boom: `links` = ja, `rechts` = nee.

```python
raadboom = Knoop("Heeft het dier vier poten?",
    Knoop("Blaft het?",
        Knoop("hond"),
        Knoop("kat")),
    Knoop("Kan het vliegen?",
        Knoop("mus"),
        Knoop("slang")))
```

### Oefening 5.1 ⭐⭐: Een gids door de boom

Schrijf `beslis(knoop, antwoorden)` met een lijst van `"ja"`/`"nee"` antwoorden. De functie loopt door de boom (links bij `"ja"`, rechts bij `"nee"`) en geeft de waarde van het blad terug waar je uitkomt.

```python
print(beslis(raadboom, ["ja", "nee"]))   # kat
print(beslis(raadboom, ["nee", "ja"]))   # mus
```

### Oefening 5.2 ⭐⭐: Het raadspel

Schrijf `speel(knoop)` die het spel interactief speelt met `input()`:

- Is de knoop een blad? Print `"Is het een <dier>?"` en vraag ja/nee. Bij `ja` wint de computer, anders verliest hij.
- Is het een vraag? Stel de vraag met `input()` en ga verder links (ja) of rechts (nee).

### Oefening 5.3 ⭐⭐⭐: De computer leert bij

Breid `speel` uit: verliest de computer, dan vraagt hij:

1. Aan welk dier dacht je?
2. Welke vraag onderscheidt jouw dier van het dier dat hij zelf voorstelde?
3. Wat is het antwoord op die vraag voor jouw dier (ja/nee)?

Het **blad verandert dan in een vraag-knoop** met twee nieuwe bladeren (het oude en het nieuwe dier). Speel een paar keer na elkaar met dezelfde boom: de computer wordt slimmer!

*Tip:* je kan het blad ter plekke aanpassen (`blad.waarde = ...`, `blad.links = ...`). Zou dat ook gelukt zijn met twee aparte klassen voor interne knopen en bladeren?

---

## Deel 6: De brug naar de heap

Een **heap** (min-heap) is een binaire boom met twee eigenschappen:

1. **Vorm:** de boom is op elk niveau *vol*, behalve eventueel het laatste niveau, dat van links naar rechts opgevuld wordt.
2. **Heap-eigenschap:** elke knoop is **kleiner dan of gelijk aan** zijn kinderen. De kleinste waarde staat dus altijd bovenaan, in de wortel.

Door die vorm kan je een heap **zonder knopen of verwijzingen** opslaan: in een **gewone lijst**, laag per laag. Voor de knoop op **index `i`** geldt:

| | formule |
|---|---|
| linkerkind | `2*i + 1` |
| rechterkind | `2*i + 2` |
| ouder | `(i - 1) // 2` |

```
lijst:  [1, 3, 2, 5, 9, 8, 7]
index:   0  1  2  3  4  5  6

          1            index 0
        /   \
       3     2         index 1, 2
      / \   / \
     5   9 8   7       index 3, 4, 5, 6
```

### Oefening 6.1 ⭐: Wijzers

Schrijf `links_kind(i)`, `rechts_kind(i)` en `ouder(i)`. Controleer met de lijst hierboven: wat zijn de kinderen en de ouder van de knoop op index 1? En van index 5?

### Oefening 6.2 ⭐⭐: Van lijst naar boom

Schrijf `lijst_naar_boom(lijst, i=0)` die de lijst omzet naar een boom van `Knoop`-objecten. Is `i` buiten de lijst, dan is er geen knoop (`None`). Anders maak je een knoop met `lijst[i]` en als kinderen de bomen vanaf `links_kind(i)` en `rechts_kind(i)`.

```python
teken(lijst_naar_boom([1, 3, 2, 5, 9, 8, 7]))
```

Verwachte uitvoer:

```
1
├─ L: 3
│  ├─ L: 5
│  └─ R: 9
└─ R: 2
   ├─ L: 8
   └─ R: 7
```

### Oefening 6.3 ⭐⭐: Is het een heap?

Schrijf `is_min_heap(lijst)` die controleert of elk element groter dan of gelijk aan zijn ouder is.

```python
print(is_min_heap([1, 3, 2, 5, 9, 8, 7]))   # True
print(is_min_heap([1, 5, 2, 6, 3, 0]))      # False
```

### Oefening 6.4 ⭐⭐⭐: Toevoegen aan een heap

Schrijf `push(heap, waarde)`:

1. Voeg de waarde **achteraan** de lijst toe (het eerstvolgende vrije plekje in de boom).
2. Zolang de nieuwe waarde **kleiner is dan haar ouder**, wissel je ze om en schuif je een plaats op naar boven (*"naar boven laten borrelen"*).

```python
heap = []
for getal in [5, 3, 8, 1, 9, 2, 7]:
    push(heap, getal)
print(heap)   # [1, 3, 2, 5, 9, 8, 7]
```

### Oefening 6.5 ⭐⭐⭐: Het kleinste element eruit halen

Schrijf `pop(heap)` die het **kleinste element** (index 0) teruggeeft en verwijdert:

1. Bewaar `heap[0]`.
2. Haal het **laatste** element uit de lijst (`heap.pop()`) en zet het op index 0.
3. Laat het naar **onder** zakken: vergelijk het met zijn kinderen en wissel het met het **kleinste kind** zolang dat kleiner is dan het element. Let op dat de kinderen binnen de lijst vallen!
4. Geef het bewaarde minimum terug.

```python
print([pop(heap) for _ in range(7)])   # [1, 2, 3, 5, 7, 8, 9]
```

> Je hebt zonet gesorteerd met een heap (*heapsort*): alles in een heap steken en het kleinste telkens eruit halen.

### Oefening 6.6 ⭐⭐⭐: Eigen klasse `MinHeap`

Verpak `push` en `pop` in een klasse `MinHeap` met een attribuut `elementen` (de lijst) en de methodes `push(waarde)`, `pop()` en `is_leeg()`.

```python
h = MinHeap()
for getal in [5, 3, 8, 1]:
    h.push(getal)
print(h.pop())     # 1
print(h.is_leeg()) # False
```

Denk daarna na: **een gebruiker van `MinHeap` hoeft niet te weten dat er een lijst achter zit** (dat is de *implementatie*). Hij heeft enkel `push` en `pop` nodig (de *interface*). Zou je de heap ook kunnen implementeren met `Knoop`-objecten, zonder dat de gebruiker iets merkt? Waarom kiest men toch meestal voor de lijst?

---

## Bonus: ideeën om verder te gaan

- Een functie `is_zoekboom(knoop)` die controleert of een boom aan de zoekboom-eigenschap voldoet. *Opgelet:* het volstaat niet om enkel de kinderen te vergelijken met hun ouder!
- `verwijder(knoop, waarde)` voor de zoekboom (de moeilijkste operatie!).
- Een **trie** (voor woorden en autocomplete): een boom waarin elke knoop een letter is.
- Een bestandssysteem als boom: een mappenstructuur modelleren met knopen met een **lijst** van kinderen, en een functie die de totale grootte of het aantal bestanden berekent.
- Teken de recursieve oproepen van `diepte(boom)` uit voor een kleine boom, op papier.
