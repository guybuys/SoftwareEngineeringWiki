# Oplossingen

> Alle deelopdrachten staan in een functie om gemakkelijk te knippen/plakken.

## Oefening 1 en 2 – Secret Santa als graaf.

```python
def secret_santa():
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
        
    # Opdracht bij 1.5: Schrijf fuctie die True geeft als a en b in dezelfde familie zitten
    def zelfde_familie(a, b):
        for familie in families:
            if a in familie and b in familie:
                return True

        return False

    santa = Graaf()
    # Opdracht bij 1.5: Voeg alle personen toe als knoop
    for p in personen:
        santa.voeg_knoop_toe(p)
    # Opdracht bij 1.5: Voeg een boog toe tussen elk paar dat NIET in dezelfde familie zit
    for i, a in enumerate(personen):
        for b in personen[i + 1:]:
            if not zelfde_familie(a, b):
                santa.voeg_boog_toe(a, b)

    # Controle en bijvragen
    print(santa.aantal_bogen())   # 38
    print(santa.graad("Guy"))     # 9
    print(santa.graad("Celien"))  # 7

    # Oefening 2: Oefening 2 – Controleer een oplossing.

    def is_geldige_ronde(graaf, ronde):
        if len(ronde) != len(graaf.knopen()):
            return False

        for knoop in graaf.knopen():
            if knoop not in ronde:
                return False

        vorige = ronde[-1]

        for knoop in ronde:
            if knoop not in graaf.buren_van(vorige):
                return False
            vorige = knoop            
        return True

    # Test bij Oefening 2 – Controleer een oplossing.

    ronde = ["Arne", "Jessica", "Iebe", "Bea", "Hanne", "Guy", "Frederik", "Dora", "Evert", "Celien"]
    print(is_geldige_ronde(santa, ronde))                       # True
    print(is_geldige_ronde(santa, ["Arne", "Bea"] + ronde[2:])) # False

    # Deze ronde Tristan en Thijs op papier:
    ronde_tt = ["Bea", "Celien", "Jessica", "Dora", "Frederik", "Hanne", "Evert", "Arne", "Iebe", "Guy"]
    print(is_geldige_ronde(santa, ronde_tt))

```

## Oefening 3 – Uitbreiden van de klasse.

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
            totaal = totaal + len(buren)

        if self.gericht:
            return totaal
        else:
            return totaal // 2
        
    # Uitbreiden van de klasse: nieuwe methodes
    def heeft_boog(self, a, b):
        if b in self.buren_van(a):
            return True
        return False
    
    def ingraad(self, knoop):
        aantal = 0
        for value in self.verbindingen.values():
            if knoop in value:
                aantal += 1
            
        return aantal
    
    def totaal_gewicht(self):
        resultaat = 0
        for value in self.verbindingen.values():
            for gewicht in value.values():
                resultaat += gewicht
        if not self.gericht:
            return resultaat // 2
        return resultaat

    # Dit hoort niet bij deze opdracht maar is een uitbreiding voor de uitbreidingsoefeningen
    def gewicht_boog(self, a, b):
        if b in self.buren_van(a):
            return self.verbindingen[a][b]
        return None
```

## Oefening 4 – Klein onderzoek.

```python
def oef_steden():
    steden = Graaf()
    steden.voeg_boog_toe("New York", "Los Angeles", 4496)
    steden.voeg_boog_toe("New York", "Phoenix", 3900)
    steden.voeg_boog_toe("Los Angeles", "Phoenix", 600)
    steden.voeg_boog_toe("Los Angeles", "Chicago", 3250)
    steden.voeg_boog_toe("Chicago", "Phoenix", 2821)
    steden.voeg_boog_toe("Chicago", "Houston", 1743)
    steden.voeg_boog_toe("Houston", "Phoenix", 1892)

    print(steden.verbindingen["Phoenix"])

    for stad in steden.knopen():
        print(f"{stad} -> {steden.graad(stad)}")
```

### Minimaal opspannende boom. 

**Met de beschreven methode**
Gesorteerde gewichten: 
600 (LA–Phoenix), 
1743 (Chicago–Houston), 
1892 (Houston–Phoenix), 
2821 (Chicago–Phoenix, geeft een cyclus: overslaan), 
3250 (LA–Chicago, cyclus: overslaan), 
3900 (New York–Phoenix), 
4496 (New York–LA, cyclus: overslaan). 

Totaal: 600 + 1743 + 1892 + 3900 = 8135.

**Methode `Prim`**

Begin bij één stad of boog en laat de boom groeien. Kies telkens de goedkoopste boog die vanuit je boom naar een nieuwe stad leidt.

Start met LA–Phoenix (600). De boom bevat nu {LA, Phoenix}.
De bogen die uit de boom vertrekken zijn:
Phoenix–Houston (1892)
Phoenix–Chicago (2821)
LA–Chicago (3250)
Phoenix–New York (3900)
LA–New York (4496)

De goedkoopste is Phoenix–Houston (1892).
De boom is nu {LA, Phoenix, Houston}. 

De goedkoopste boog naar een nieuwe stad is Houston–Chicago (1743).
Alleen New York ontbreekt nog. De goedkoopste boog naar New York is Phoenix–New York (3900).

Totaal: 600 + 1892 + 1743 + 3900 = 8135.
