# Binaire bomen in Python: oplossingen
 
Terug naar de opgaves: [bomen-oefeningen.md](bomen-oefeningen.md)
 
> **Probeer eerst zelf!** Er zijn meerdere juiste oplossingen. De hier getoonde oplossingen zijn niet de enige mogelijke. Alle code werd uitgetest.
 
Elke code-blok gaat ervan uit dat de klasse `Knoop` en de functie `teken` (zie Deel 0 van de opgaves) bovenaan je bestand staan.
 
---
 
## Deel 1: Bomen bouwen en benoemen
 
### Oplossing 1.1
 
1. Wortel: `8`
2. Bladeren: `1`, `4`, `7`, `13`
3. Interne knopen: `8`, `3`, `10`, `6`, `14`
4. Niveau van `6`: **2**. Niveau van `13`: **3**.
5. Diepte: **3** (het diepste blad, `4`, `7` of `13`, zit op niveau 3).
6. Pad naar `7`: `8 → 3 → 6 → 7`
7. Ja: elke knoop heeft maximaal twee kinderen.
### Oplossing 1.2
 
```python
boom = Knoop(8,
    Knoop(3,
        Knoop(1),
        Knoop(6, Knoop(4), Knoop(7))),
    Knoop(10,
        None,
        Knoop(14, Knoop(13), None)))
 
teken(boom)
```
 
De knoop `10` heeft geen linkerkind, dus `None`. De knoop `14` heeft enkel een linkerkind (`13`), dus `None` op de plaats van rechts. Het tweede `None` mag je ook weglaten, want `rechts=None` is de beginwaarde.
 
### Oplossing 1.3
 
```python
boom.rechts.waarde                  # 10
boom.links.rechts.waarde            # 6
boom.rechts.rechts.links.waarde     # 13
boom.links.rechts.rechts.waarde     # 7
```
 
### Oplossing 1.4
 
```python
def is_blad(knoop):
    return knoop.links is None and knoop.rechts is None
```
 
### Oplossing 1.5
 
```python
dieren = Knoop("dier",
    Knoop("zoogdier", Knoop("hond"), Knoop("kat")),
    Knoop("vogel"))
 
print(dieren.links.rechts.waarde)   # kat
```
 
---
 
## Deel 2: Recursie op bomen
 
### Oplossing 2.1
 
```python
def som(knoop):
    if knoop is None:
        return 0
    return knoop.waarde + som(knoop.links) + som(knoop.rechts)
```
 
Het basisgeval is `0`, omdat `0` het "neutrale element" van een som is: een lege boom verandert niets aan de som.
 
### Oplossing 2.2
 
```python
def aantal_bladeren(knoop):
    if knoop is None:
        return 0
    if is_blad(knoop):
        return 1
    return aantal_bladeren(knoop.links) + aantal_bladeren(knoop.rechts)
```
 
*Waarom twee basisgevallen?* Zonder het tweede zou je een blad tellen als `0 + 0 = 0`.
 
### Oplossing 2.3
 
```python
def diepte(knoop):
    if knoop is None:
        return -1
    return 1 + max(diepte(knoop.links), diepte(knoop.rechts))
```
 
De waarde `-1` voor de lege boom zorgt ervoor dat een blad diepte `1 + max(-1, -1) = 0` krijgt. Dat klopt met de definitie.
 
### Oplossing 2.4
 
```python
def bevat(knoop, waarde):
    if knoop is None:
        return False
    return (knoop.waarde == waarde
            or bevat(knoop.links, waarde)
            or bevat(knoop.rechts, waarde))
```
 
Dankzij `or` stopt Python zodra er één `True` is, dus zodra de waarde gevonden is.
 
### Oplossing 2.5
 
```python
def maximum(knoop):
    grootste = knoop.waarde
    if knoop.links is not None:
        grootste = max(grootste, maximum(knoop.links))
    if knoop.rechts is not None:
        grootste = max(grootste, maximum(knoop.rechts))
    return grootste
```
 
Hier is het basisgeval **een knoop zonder kinderen** (dan blijft `grootste` gewoon de eigen waarde). We vermijden zo een "maximum van een lege boom", dat niet bestaat.
 
### Oplossing 2.6
 
```python
def spiegel(knoop):
    if knoop is None:
        return None
    return Knoop(knoop.waarde, spiegel(knoop.rechts), spiegel(knoop.links))
```
 
Let op de volgorde in de oproep: `spiegel(knoop.rechts)` komt op de plaats van `links`. Deze functie bouwt een **nieuwe** boom en laat de originele ongemoeid.
 
### Oplossing 2.7
 
```python
def niveau_van(knoop, waarde, niveau=0):
    if knoop is None:
        return None
    if knoop.waarde == waarde:
        return niveau
    links = niveau_van(knoop.links, waarde, niveau + 1)
    if links is not None:
        return links
    return niveau_van(knoop.rechts, waarde, niveau + 1)
```
 
Bij elke stap naar beneden tellen we 1 bij het niveau. We controleren `links is not None` (en niet `if links:`), omdat niveau `0` anders als "niet gevonden" zou tellen.
 
---
 
## Deel 3: Een boom doorlopen
 
### Oplossing 3.1
 
- preorder: `8, 3, 1, 6, 4, 7, 10, 14, 13`
- inorder: `1, 3, 4, 6, 7, 8, 10, 13, 14`
- postorder: `1, 4, 7, 6, 3, 13, 14, 10, 8`
### Oplossing 3.2
 
```python
def preorder(knoop):
    if knoop is None:
        return []
    return [knoop.waarde] + preorder(knoop.links) + preorder(knoop.rechts)
 
 
def inorder(knoop):
    if knoop is None:
        return []
    return inorder(knoop.links) + [knoop.waarde] + inorder(knoop.rechts)
 
 
def postorder(knoop):
    if knoop is None:
        return []
    return postorder(knoop.links) + postorder(knoop.rechts) + [knoop.waarde]
```
 
De drie functies verschillen enkel in de **plaats van `[knoop.waarde]`**.
 
**Opmerking over de inorder-lijst:** ze is **gesorteerd** van klein naar groot. Dat is geen toeval: de voorbeeldboom is een zoekboom (zie Deel 4).
 
### Oplossing 3.3
 
```python
from collections import deque
 
 
def per_niveau(knoop):
    resultaat = []
    wachtrij = deque()
    if knoop is not None:
        wachtrij.append(knoop)
    while wachtrij:
        huidige = wachtrij.popleft()
        resultaat.append(huidige.waarde)
        if huidige.links is not None:
            wachtrij.append(huidige.links)
        if huidige.rechts is not None:
            wachtrij.append(huidige.rechts)
    return resultaat
```
 
Omdat een wachtrij *first in, first out* werkt, worden alle knopen van niveau 1 verwerkt vóór die van niveau 2, enzovoort.
 
### Oplossing 3.4
 
```python
uitdrukking = Knoop('+',
    Knoop('*', Knoop(3), Knoop(4)),
    Knoop('*', Knoop(8), Knoop(5)))
 
 
def bereken(knoop):
    if is_blad(knoop):
        return knoop.waarde
    links = bereken(knoop.links)
    rechts = bereken(knoop.rechts)
    if knoop.waarde == '+':
        return links + rechts
    if knoop.waarde == '-':
        return links - rechts
    if knoop.waarde == '*':
        return links * rechts
    if knoop.waarde == '/':
        return links / rechts
 
 
def naar_tekst(knoop):
    if is_blad(knoop):
        return str(knoop.waarde)
    return "(" + naar_tekst(knoop.links) + " " + knoop.waarde + " " + naar_tekst(knoop.rechts) + ")"
 
 
print(bereken(uitdrukking))      # 52
print(naar_tekst(uitdrukking))   # ((3 * 4) + (8 * 5))
```
 
Voor `(2 + 3) * 4`:
 
```python
andere = Knoop('*',
    Knoop('+', Knoop(2), Knoop(3)),
    Knoop(4))
 
print(bereken(andere))      # 20
print(naar_tekst(andere))   # ((2 + 3) * 4)
```
 
**Wat valt op?** De haakjes zijn in de boom **verdwenen**: de vorm van de boom legt de rekenvolgorde vast. Bij `(2 + 3) * 4` staat de `+` dieper in de boom dan de `*`, dus wordt hij **eerst** berekend. Een boom heeft haakjes of voorrangsregels dus niet nodig.
 
### Oplossing 3.5
 
```python
def alle_paden(knoop):
    if is_blad(knoop):
        return [[knoop.waarde]]
    paden = []
    for kind in (knoop.links, knoop.rechts):
        if kind is not None:
            for pad in alle_paden(kind):
                paden.append([knoop.waarde] + pad)
    return paden
```
 
### Oplossing 3.6
 
```python
def muntboom(worpen, waarde=None):
    if worpen == 0:
        return Knoop(waarde)
    return Knoop(waarde,
                 muntboom(worpen - 1, "kop"),
                 muntboom(worpen - 1, "munt"))
 
 
paden = alle_paden(muntboom(3))
print(len(paden))                                       # 8
print(sum(1 for pad in paden if pad.count("kop") >= 2)) # 4
```
 
Er zijn 8 paden (alle mogelijke uitkomsten). In **4** daarvan zit minstens twee keer `"kop"`, dus in de andere 4 minstens twee keer `"munt"`. Het spel is **eerlijk**: beide spelers hebben evenveel kans (4 op 8).
 
---
 
## Deel 4: De binaire zoekboom
 
### Oplossing 4.1
 
```python
def zoek(knoop, waarde):
    if knoop is None:
        return False
    if waarde == knoop.waarde:
        return True
    if waarde < knoop.waarde:
        return zoek(knoop.links, waarde)
    return zoek(knoop.rechts, waarde)
```
 
Anders dan `bevat` uit Deel 2 gaat deze functie **maar één kant op**. Daarom is zoeken in een zoekboom sneller.
 
### Oplossing 4.2
 
```python
def voeg_toe(knoop, waarde):
    if knoop is None:
        return Knoop(waarde)
    if waarde < knoop.waarde:
        knoop.links = voeg_toe(knoop.links, waarde)
    elif waarde > knoop.waarde:
        knoop.rechts = voeg_toe(knoop.rechts, waarde)
    return knoop
```
 
Elke knoop op het pad naar beneden "hangt zijn kind opnieuw vast" aan het resultaat van de recursieve oproep. Meestal is dat hetzelfde kind als voordien. Alleen op de plaats waar de nieuwe knoop komt, verandert er iets: daar vervangt `Knoop(waarde)` de `None`.
 
### Oplossing 4.3
 
```python
def bouw(lijst):
    wortel = None
    for getal in lijst:
        wortel = voeg_toe(wortel, getal)
    return wortel
 
 
for lijst in ([5, 2, 8, 1, 4, 7, 9],
              [1, 2, 4, 5, 7, 8, 9],
              [9, 8, 7, 5, 4, 2, 1],
              [5, 2, 8, 4, 1, 9, 7]):
    print(lijst, diepte(bouw(lijst)))
```
 
| lijst | diepte |
|---|---|
| `a = [5, 2, 8, 1, 4, 7, 9]` | 2 |
| `b = [1, 2, 4, 5, 7, 8, 9]` | **6** |
| `c = [9, 8, 7, 5, 4, 2, 1]` | **6** |
| `d = [5, 2, 8, 4, 1, 9, 7]` | 2 |
 
1. De boom van `a` is mooi evenwichtig. De boom van `b` heeft elke keer enkel een rechterkind.
2. Bij **al gesorteerde invoer** (oplopend of aflopend) komt elke nieuwe waarde achteraan het ene uiteinde. De "boom" is dan eigenlijk een **lijst** en zoeken is even traag als in een lijst (7 stappen i.p.v. 3).
3. Begin met de **middelste** waarde (de mediaan) en ga dan telkens het midden van elke helft af. Hierom bestaan er **zelfbalancerende bomen** (AVL-bomen, rood-zwarte bomen), die zichzelf herschikken.
### Oplossing 4.4
 
```python
def boomsort(lijst):
    wortel = None
    for getal in lijst:
        wortel = voeg_toe(wortel, getal)
    return inorder(wortel)
```
 
`inorder` van een zoekboom geeft **altijd een gesorteerde lijst**: eerst alles wat kleiner is (links), dan de knoop zelf, dan alles wat groter is (rechts). Dubbele waarden verdwijnen omdat `voeg_toe` die negeert.
 
### Oplossing 4.5
 
```python
def minimum_bst(knoop):
    while knoop.links is not None:
        knoop = knoop.links
    return knoop.waarde
 
 
def maximum_bst(knoop):
    while knoop.rechts is not None:
        knoop = knoop.rechts
    return knoop.waarde
```
 
Het kleinste element staat helemaal **links** onderaan, het grootste helemaal **rechts**.
 
### Oplossing 4.6
 
```python
def zoek_stappen(knoop, waarde):
    stappen = 0
    while knoop is not None:
        stappen += 1
        if waarde == knoop.waarde:
            return True, stappen
        if waarde < knoop.waarde:
            knoop = knoop.links
        else:
            knoop = knoop.rechts
    return False, stappen
 
 
import random
 
random.seed(1)
getallen = random.sample(range(1, 100000), 1000)
 
boom_1000 = None
for g in getallen:
    boom_1000 = voeg_toe(boom_1000, g)
 
print(diepte(boom_1000))   # 20
 
totaal_lijst = 0
totaal_boom = 0
for _ in range(200):
    gezocht = random.randint(1, 99999)
    if gezocht in getallen:
        totaal_lijst += getallen.index(gezocht) + 1
    else:
        totaal_lijst += len(getallen)
    totaal_boom += zoek_stappen(boom_1000, gezocht)[1]
 
print(totaal_lijst / 200)   # ongeveer 1000
print(totaal_boom / 200)    # ongeveer 12
```
 
(De precieze waarden hangen af van de gebruikte willekeurige getallen.) **Besluit:** in een lijst van 1000 elementen kijk je er gemiddeld ongeveer **1000** na (bijna altijd zit het getal er niet in), in de zoekboom ongeveer **12 tot 15**. Een boom van 1000 willekeurige getallen is ongeveer 20 niveaus diep. Elke verdubbeling van het aantal elementen kost je maar één extra stap. Hoe groter de data, hoe groter het verschil.
 
---
 
## Deel 5: Beslissingsbomen en een raadspel
 
### Oplossing 5.1
 
```python
def beslis(knoop, antwoorden):
    for antwoord in antwoorden:
        if is_blad(knoop):
            break
        if antwoord == "ja":
            knoop = knoop.links
        else:
            knoop = knoop.rechts
    return knoop.waarde
```
 
### Oplossing 5.2
 
```python
def vraag_ja_nee(tekst):
    antwoord = ""
    while antwoord not in ("ja", "nee"):
        antwoord = input(tekst + " (ja/nee) ").strip().lower()
    return antwoord == "ja"
 
 
def speel(knoop):
    if is_blad(knoop):
        if vraag_ja_nee("Is het een " + knoop.waarde + "?"):
            print("Ik heb gewonnen!")
        else:
            print("Ik heb verloren.")
    else:
        if vraag_ja_nee(knoop.waarde):
            speel(knoop.links)
        else:
            speel(knoop.rechts)
```
 
### Oplossing 5.3
 
```python
def speel(knoop):
    if is_blad(knoop):
        if vraag_ja_nee("Is het een " + knoop.waarde + "?"):
            print("Ik heb gewonnen!")
        else:
            leer_bij(knoop)
    else:
        if vraag_ja_nee(knoop.waarde):
            speel(knoop.links)
        else:
            speel(knoop.rechts)
 
 
def leer_bij(blad):
    oud_dier = blad.waarde
    nieuw_dier = input("Ik geef me gewonnen. Aan welk dier dacht je? ")
    vraag = input("Welke vraag onderscheidt een " + nieuw_dier +
                  " van een " + oud_dier + "? ")
    ja_voor_nieuw = vraag_ja_nee("Wat is het antwoord op die vraag voor een " + nieuw_dier + "?")
 
    nieuw_blad = Knoop(nieuw_dier)
    oud_blad = Knoop(oud_dier)
 
    blad.waarde = vraag             # het blad wordt een vraag-knoop
    if ja_voor_nieuw:
        blad.links, blad.rechts = nieuw_blad, oud_blad
    else:
        blad.links, blad.rechts = oud_blad, nieuw_blad
 
 
# Spelen, telkens opnieuw met dezelfde boom:
while True:
    speel(raadboom)
    if not vraag_ja_nee("Nog eens?"):
        break
```
 
Omdat we het blad **ter plekke aanpassen**, hoeft de ouder niets te weten van de verandering. Met twee aparte klassen (`Intern` en `Blad`) zou dat niet kunnen: een `Blad`-object kan je niet in een `Intern`-object omvormen. Je zou het bij de ouder moeten *vervangen*, en daarvoor moet je de ouder kennen. Eén klasse `Knoop` maakt dit veel eenvoudiger.
 
---
 
## Deel 6: De brug naar de heap
 
### Oplossing 6.1
 
```python
def links_kind(i):
    return 2 * i + 1
 
 
def rechts_kind(i):
    return 2 * i + 2
 
 
def ouder(i):
    return (i - 1) // 2
```
 
Voor de lijst `[1, 3, 2, 5, 9, 8, 7]`: index 1 (waarde `3`) heeft kinderen op index 3 en 4 (waarden `5` en `9`) en ouder op index 0 (waarde `1`). Index 5 (waarde `8`) heeft ouder op index 2 (waarde `2`). De kinderen zouden op index 11 en 12 staan, buiten de lijst: het is dus een blad.
 
### Oplossing 6.2
 
```python
def lijst_naar_boom(lijst, i=0):
    if i >= len(lijst):
        return None
    return Knoop(lijst[i],
                 lijst_naar_boom(lijst, links_kind(i)),
                 lijst_naar_boom(lijst, rechts_kind(i)))
```
 
### Oplossing 6.3
 
```python
def is_min_heap(lijst):
    for i in range(1, len(lijst)):
        if lijst[i] < lijst[ouder(i)]:
            return False
    return True
```
 
We beginnen bij index 1, want de wortel (index 0) heeft geen ouder. Het is voldoende elk element met zijn **ouder** te vergelijken.
 
### Oplossing 6.4
 
```python
def push(heap, waarde):
    heap.append(waarde)
    i = len(heap) - 1
    while i > 0 and heap[i] < heap[ouder(i)]:
        heap[i], heap[ouder(i)] = heap[ouder(i)], heap[i]
        i = ouder(i)
```
 
Na het toevoegen achteraan klopt de vorm, maar mogelijk niet de heap-eigenschap. Het nieuwe element klimt omhoog tot zijn ouder kleiner is of tot hij de wortel bereikt. Dat zijn hoogstens "diepte van de boom" wissels.
 
### Oplossing 6.5
 
```python
def pop(heap):
    minimum = heap[0]
    laatste = heap.pop()          # haalt het laatste element uit de lijst
    if heap:                      # er blijven nog elementen over
        heap[0] = laatste
        i = 0
        while True:
            l, r = links_kind(i), rechts_kind(i)
            kleinste = i
            if l < len(heap) and heap[l] < heap[kleinste]:
                kleinste = l
            if r < len(heap) and heap[r] < heap[kleinste]:
                kleinste = r
            if kleinste == i:     # heap-eigenschap hersteld
                break
            heap[i], heap[kleinste] = heap[kleinste], heap[i]
            i = kleinste
    return minimum
```
 
Waarom het **laatste** element bovenaan zetten? Zo blijft de **vorm** van de boom correct (geen gaten). Daarna herstellen we de **heap-eigenschap** door dat element naar onder te laten zakken. We wisselen met het **kleinste** kind, anders zou het andere kind kleiner zijn dan zijn nieuwe ouder.
 
Test:
 
```python
heap = []
for getal in [5, 3, 8, 1, 9, 2, 7]:
    push(heap, getal)
print(heap)                           # [1, 3, 2, 5, 9, 8, 7]
print([pop(heap) for _ in range(7)])  # [1, 2, 3, 5, 7, 8, 9]
```
 
### Oplossing 6.6
 
```python
class MinHeap:
    def __init__(self):
        self.elementen = []
 
    def is_leeg(self):
        return len(self.elementen) == 0
 
    def push(self, waarde):
        self.elementen.append(waarde)
        i = len(self.elementen) - 1
        while i > 0 and self.elementen[i] < self.elementen[(i - 1) // 2]:
            o = (i - 1) // 2
            self.elementen[i], self.elementen[o] = self.elementen[o], self.elementen[i]
            i = o
 
    def pop(self):
        e = self.elementen
        minimum = e[0]
        laatste = e.pop()
        if e:
            e[0] = laatste
            i = 0
            while True:
                l, r = 2 * i + 1, 2 * i + 2
                kleinste = i
                if l < len(e) and e[l] < e[kleinste]:
                    kleinste = l
                if r < len(e) and e[r] < e[kleinste]:
                    kleinste = r
                if kleinste == i:
                    break
                e[i], e[kleinste] = e[kleinste], e[i]
                i = kleinste
        return minimum
 
 
h = MinHeap()
for getal in [5, 3, 8, 1]:
    h.push(getal)
print(h.pop())      # 1
print(h.is_leeg())  # False
```
 
**Interface en implementatie:** de interface is `push`, `pop` en `is_leeg`. De implementatie is de lijst met de formules `2*i+1` en `2*i+2`. Je zou de heap ook met `Knoop`-objecten kunnen bouwen en de gebruiker zou niets merken, zolang `push` en `pop` hetzelfde doen.
 
**Waarom toch de lijst?** Er zijn geen verwijzingen nodig (minder geheugen), het laatste element en het eerstvolgende vrije plekje vind je meteen (`len(lijst) - 1` en `len(lijst)`), en je weet altijd waar de ouder en de kinderen staan. Met `Knoop`-objecten is het "laatste knoop vinden" bij `pop` een stuk lastiger. Het kan dankzij de vorm van de heap (altijd compleet, van links naar rechts opgevuld) zonder gaten in de lijst.
 
---
 
## Bonus: hints
 
- **`is_zoekboom`:** vergelijk niet enkel een knoop met zijn kinderen, maar geef bij de recursie een **toegelaten interval** `(minimum, maximum)` mee. Elke waarde moet binnen dat interval liggen. Naar links wordt het maximum de waarde van de knoop, naar rechts wordt het minimum de waarde van de knoop.
- **`verwijder`:** drie gevallen: de knoop is een blad (gewoon weghalen), heeft één kind (vervangen door dat kind), of heeft twee kinderen (vervang de waarde door het **kleinste** element van de rechterdeelboom en verwijder dat element daar).
 
