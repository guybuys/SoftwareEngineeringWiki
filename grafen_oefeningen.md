# Grafen

> **Doel:** je leert wat een graaf is, hoe je een probleem met een graaf modelleert, en hoe je een graaf voorstelt en bevraagt in Python.
> **Voorkennis:** klassen, functies, lijsten, verzamelingen, dictionaries, loops.

---

# Deel 1 – Klassikaal

## 1.1 Opwarmer: Secret Santa (op papier, 10 min)

Een vriendengroep van 10 personen organiseert Secret Santa. Regels:

- Broers en zussen mogen niet voor elkaar kopen.
- Als A voor B koopt, mag B niet voor A kopen.
- Iedereen geeft precies één cadeau en krijgt precies één cadeau.

| Familie | Personen |
|---|---|
| 1 | Arne, Bea |
| 2 | Celien, Dora, Hanne |
| 3 | Evert, Frederik, Jessica |
| 4 | Guy |
| 5 | Iebe |

**Opdracht (in groepjes, op papier of bord):**

1. Teken de 10 personen als bolletjes.
2. Trek een lijn tussen twee personen als ze **elkaar een cadeau mogen geven**.
3. Zoek een manier om iedereen één keer te bezoeken en terug bij je startpunt uit te komen.

**Klassikaal bespreken:**

- Wat stelt een bolletje voor? Wat stelt een lijn voor?
- Hoeveel lijnen heb je getekend? Is er een handige manier om dat te tellen (45 mogelijke paren, min 7 familieparen)?
- Is er maar één oplossing?

---

## 1.2 Wat is een graaf?

Een **graaf** (Engels: *graph*) bestaat uit:

- **knopen**: de "dingen" (personen, steden, websites, …)
- **bogen**: de verbindingen tussen twee knopen

Enkele begrippen:

| Begrip | Betekenis | Voorbeeld |
|---|---|---|
| **Buren** | knopen die door een boog met elkaar verbonden zijn | Gent en Kortrijk |
| **Graad** van een knoop | aantal bogen die bij de knoop komen | Brussel heeft graad 3 |
| **Pad** | rij knopen waarbij elke twee opeenvolgende knopen buren zijn | Kortrijk → Gent → Brussel |
| **Cyclus** | een pad dat terug bij de start eindigt | Gent → Antwerpen → Brussel → Gent |
| **Gerichte graaf** | bogen hebben een richting (pijl) | Instagram: "A volgt B" |
| **Gewogen graaf** | bogen hebben een getal (afstand, kost, tijd) | afstand tussen steden |

**Denkvragen (mondeling):**

- Een vriendschap op Facebook is wederzijds. "Volgen" op Instagram niet. Welke soort graaf gebruik je telkens?
- Een boom is ook een graaf. Welke voorwaarden gelden er extra? (Geen cycli, alles verbonden.)
- Geef drie dingen uit het dagelijks leven die je als graaf kan voorstellen.

---

## 1.3 Modelleren zonder code (10 min)

Teken telkens een graaf. Kies eerst: gericht of niet? Gewogen of niet?

**A. Strava**
- Laurens volgt Kevin en Marie
- Kevin volgt Marie
- Marie en Lisa volgen elkaar
- Emiel volgt Laurens en Kevin

**B. Splitwise** (wie moet wie hoeveel betalen)
- Yente betaalt 15 euro aan Simon en 12 euro aan Jannes
- Simon betaalt 6 euro aan Bryan
- Jannes betaalt 19 euro aan Bryan

**C. Schoolwissels** (aantal leerlingen dat van school verandert)
- 16 leerlingen: college → atheneum
- 12 leerlingen: humaniora → college
- 6 leerlingen: atheneum → college
- 2 leerlingen: college → Steinerschool
- 1 leerling: Steinerschool → atheneum

---

## 1.4 Een graaf in Python

Er bestaat geen ingebouwde graaf in Python. We maken zelf een klasse.

**Idee: een dictionary.** Bij elke knoop bewaren we zijn buren, samen met het gewicht van de boog.

```python
vlaanderen = {
    "Kortrijk":  {"Gent": 1},
    "Gent":      {"Kortrijk": 1, "Antwerpen": 1, "Brussel": 1},
    "Antwerpen": {"Gent": 1, "Brussel": 1, "Hasselt": 1},
    "Brussel":   {"Gent": 1, "Antwerpen": 1, "Hasselt": 1},
    "Hasselt":   {"Antwerpen": 1, "Brussel": 1},
}
```

**Vragen:**

- Hoe zie je in deze dictionary dat de graaf niet gericht is?
- Hoe vind je de buren van Gent? En de graad van Gent?
- Wat is het nadeel als we elke boog dubbel moeten typen? → daarom een klasse.

### De klasse (samen live coderen)

```python
class Graaf:
    def __init__(self, gericht=False):
        self.gericht = gericht
        self.verbindingen = {}          # knoop -> {buur: gewicht}

    def voeg_knoop_toe(self, knoop):
        if knoop not in self.verbindingen:
            self.verbindingen[knoop] = {}

    def voeg_boog_toe(self, van, naar, gewicht=1):
        self.voeg_knoop_toe(van)
        self.voeg_knoop_toe(naar)
        self.verbindingen[van][naar] = gewicht
        if not self.gericht:
            self.verbindingen[naar][van] = gewicht

    def knopen(self):
        return list(self.verbindingen)

    def buren_van(self, knoop):
        return list(self.verbindingen[knoop])

    def graad(self, knoop):
        return len(self.verbindingen[knoop])

    def aantal_bogen(self):
        totaal = 0
    
        for buren in self.verbindingen.values():
            totaal += len(buren)
    
        if not self.gericht:
            totaal //= 2
    
        return totaal
```

> *INFO, NIEUW*
> `list()` zet de sleutels van een dictionary om naar een gewone lijst,
> zodat we de gerourneerde waarden als lijst kunnen gebruiken.

**Bespreek:**

- Waarom voegt `voeg_boog_toe` eerst de knopen toe? (Een boog naar een onbestaande knoop kan niet.)
- Waarom staat er `if not self.gericht`? Wat gebeurt er als we dat weglaten?
- Waarom delen we door 2 in `aantal_bogen` bij een ongerichte graaf?
- Waarom is een dictionary hier handiger dan een verzameling tupels? (Snel de buren opvragen, gewichten opslaan.)

### De Vlaamse snelwegen

```python
snelwegen = Graaf()
snelwegen.voeg_boog_toe("Kortrijk", "Gent")
snelwegen.voeg_boog_toe("Gent", "Antwerpen")
snelwegen.voeg_boog_toe("Gent", "Brussel")
snelwegen.voeg_boog_toe("Antwerpen", "Brussel")
snelwegen.voeg_boog_toe("Antwerpen", "Hasselt")
snelwegen.voeg_boog_toe("Brussel", "Hasselt")

print(snelwegen.knopen())
print(snelwegen.buren_van("Gent"))
print(snelwegen.graad("Brussel"))
print(snelwegen.aantal_bogen())
```

**Voorspel eerst, run dan:** wat verwacht je als uitvoer? (Antwoord: 5 knopen, buren van Gent = Kortrijk, Antwerpen, Brussel, graad 3, 6 bogen.)

### Een gewogen graaf

```python
steden = Graaf()
steden.voeg_boog_toe("New York", "Los Angeles", 4496)
steden.voeg_boog_toe("New York", "Phoenix", 3900)
steden.voeg_boog_toe("Los Angeles", "Phoenix", 600)
steden.voeg_boog_toe("Los Angeles", "Chicago", 3250)
steden.voeg_boog_toe("Chicago", "Phoenix", 2821)
steden.voeg_boog_toe("Chicago", "Houston", 1743)
steden.voeg_boog_toe("Houston", "Phoenix", 1892)

print(steden.verbindingen["Phoenix"])
```

### Een gerichte graaf

```python
strava = Graaf(gericht=True)
strava.voeg_boog_toe("Laurens", "Kevin")
strava.voeg_boog_toe("Laurens", "Marie")
strava.voeg_boog_toe("Kevin", "Marie")
strava.voeg_boog_toe("Marie", "Lisa")
strava.voeg_boog_toe("Lisa", "Marie")
strava.voeg_boog_toe("Emiel", "Laurens")
strava.voeg_boog_toe("Emiel", "Kevin")

print(strava.buren_van("Marie"))
print(strava.aantal_bogen())
```

**Vragen:**

- Wat is hier het verschil tussen `buren_van("Marie")` en "wie volgt Marie"?
- Hoe zou je dat tweede opvragen? (Doorloop alle knopen en kijk of Marie in hun buren zit.)

---

## 1.5 Klassikale oefeningen (leerlingen coderen mee)

**Oefening 1 – Secret Santa als graaf.** Bouw de graaf van de Secret Santa-groep in code.

```python
families = [
    {"Arne", "Bea"},
    {"Celien", "Dora", "Hanne"},
    {"Evert", "Frederik", "Jessica"},
    {"Guy"},
    {"Iebe"},
]
personen = []

for familie in families:
    personen.extend(sorted(familie))

def zelfde_familie(a, b):
    ...   # TODO: geef True als a en b in dezelfde familie zitten

santa = Graaf()
# TODO: voeg alle personen toe als knoop
# TODO: voeg een boog toe tussen elk paar dat NIET in dezelfde familie zit
```

Controle: `santa.aantal_bogen()` moet **38** geven. Wat is de graad van Guy? En van Celien?

**Oefening 2 – Controleer een oplossing.** Schrijf `is_geldige_ronde(graaf, ronde)`. De functie geeft `True` als `ronde` een lijst is waarin

1. elke knoop precies één keer voorkomt,
2. elke twee opeenvolgende personen buren zijn,
3. de laatste persoon ook buur is van de eerste.

Test met:

```python
ronde = ["Arne", "Jessica", "Iebe", "Bea", "Hanne",
         "Guy", "Frederik", "Dora", "Evert", "Celien"]
print(is_geldige_ronde(santa, ronde))                     # True
print(is_geldige_ronde(santa, ["Arne", "Bea"] + ronde[2:]))  # False
```

**Oefening 3 – Uitbreiden van de klasse.** Voeg toe aan `Graaf`:

- `heeft_boog(self, a, b)` → `True`/`False`
- `ingraad(self, knoop)` → hoeveel bogen komen bij `knoop` **toe** (bedoeld voor gerichte grafen). Test met `strava.ingraad("Marie")` (verwacht: 3).
- `totaal_gewicht(self)` → som van alle gewichten van de bogen (denk aan dubbel tellen bij ongerichte grafen).

**Oefening 4 – Klein onderzoek.** Bereken voor `steden` de graad van elke knoop met een loop. Welke stad is het "drukst"? Vergelijk met de tekening.

---

## 1.6 Toepassingen (klassikaal bespreken)

| Toepassing | Knopen | Bogen | Soort graaf |
|---|---|---|---|
| Routeplanner | kruispunten / steden | wegen | gewogen (afstand of tijd), soms gericht |
| Het web | websites | links | gericht |
| Sociaal netwerk | personen | vriendschap / volgen | ongericht of gericht |
| Spoorwegnet | stations | sporen | gewogen |
| Kaart kleuren | landen | gemeenschappelijke grens | ongericht |

**Koningsbergen (Euler, 1736).** Kan je een wandeling maken die elke brug precies één keer gebruikt? Euler tekende de landsdelen als knopen en de bruggen als bogen en keek naar de **graad** van de knopen.

De graden zijn hier 3, 3, 3 en 5. Euler toonde aan dat zo'n wandeling alleen kan als hoogstens twee knopen een oneven graad hebben. Hier zijn het er vier. **Dus kan het niet.** Laat de leerlingen zelf een eigen graaf bedenken waarbij het wél kan.

**Minimaal opspannende boom.** Je wil alle steden verbinden met zo weinig mogelijk spoor. Doe dit met `steden` op papier: sorteer de bogen van klein naar groot gewicht en voeg ze toe zolang er geen cyclus ontstaat. Wat is het totaal? (Zie oplossingen.)

---

## 1.7 Uitbreiding voor snelle leerlingen

**Six degrees of separation.** Zoek het kortste pad (in aantal stappen) tussen twee knopen.

```python
def kortste_pad(graaf, start, doel):
    wachtrij = [[start]]
    bezocht = {start}
    while wachtrij:
        pad = wachtrij.pop(0)
        knoop = pad[-1]
        if knoop == doel:
            return pad
        for buur in graaf.buren_van(knoop):
            if buur not in bezocht:
                bezocht.add(buur)
                wachtrij.append(pad + [buur])
    return None

print(kortste_pad(snelwegen, "Kortrijk", "Hasselt"))
```

Vragen: waarom gebruiken we `bezocht`? Wat gebeurt er zonder? Waarom vinden we zo het kortste pad? Wat geeft `None` aan?

**Secret Santa automatisch oplossen.** Schrijf een functie die zelf een geldige ronde zoekt. Hint: probeer een persoon toe te voegen aan een deelronde, en stap terug als je vastloopt (backtracking).
