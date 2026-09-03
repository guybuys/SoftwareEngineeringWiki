# Korte introductie tot reproduceerbare Python-projecten

Elke Python-applicatie heeft haar eigen **omgeving** en haar eigen lijstje *dependencies* of *bibliotheken*, nodig om de applicatie te laten werken.

## Wat doet venv?

Een **virtual environment** is een afgeschermde Python-omgeving voor één project.

Zonder **venv** installeer een je bijvoorbeeld:
```python
pip install requests
pip install flask
pip install numpy
```
in de *algemene* Python-installatie. Na een tijdje krijg je dan problemen:

- project A heeft requests versie X nodig
- project B heeft versie Y nodig
- je installeert van alles door elkaar
- op een andere machine werkt het project niet zomaar

Met **venv** krijgt elk project zijn eigen omgeving:
```markdown
mijn_project/
│
├── .venv/
├── main.py
└── requirements.txt
```

De map `.venv` bevat de specifieke Python-omgeving en geïnstalleerde packages.

**Die `.venv` zelf zet je normaal niet in Git of in een zip die je aan iemand anders geeft.** Je maakt hem op de doelmachine opnieuw aan.

## Een eenvoudige workflow voor leerlingen

### 1. Project maken
```bash
mkdir mijn_project
cd mijn_project
```
### 2. Virtual environment maken
```bash
python -m venv .venv
```
Op **Linux/Raspberry Pi** kan `python` soms naar Python 2 verwijzen of niet aanwezig zijn. Daar is dan:
```bash
python3 -m venv .venv
```
een alternatief.

### 3. Virtual environment activeren

**Windows PowerShell:**
```PowerShell
.venv\Scripts\Activate.ps1
```
**Windows CMD:**
```cmd
.venv\Scripts\activate.bat
```
**Linux / Raspberry Pi:**
```bash
source .venv/bin/activate
```
Daarna zie je meestal iets als:

`(.venv) C:\Users\Leerling\mijn_project>`

of:

`(.venv) leerling@raspberrypi:~/mijn_project$`

Dat `(.venv)` is een handig visueel signaal: **ik werk nu in mijn projectomgeving.**

### 4. Packages installeren

Bijvoorbeeld:
```bash
python -m pip install requests
python -m pip install numpy
python -m pip install flask
```

Nadat de benodigde packages geïnstalleerd zijn, kan je de geïnstalleerde packages noteren in een text file. Hiervoor gebruiken we `requirements.txt`:
```bash
python -m pip freeze > requirements.txt
```
Dan krijg je bijvoorbeeld:
```bash
Flask==3.1.2
requests==2.32.5
```
Je project kan dan bijvoorbeeld zijn:
```markdown
mijn_project/
├── .venv/
├── main.py
└── requirements.txt
```
Iemand die je project wil gebruiken, hoeft vervolgens niet te weten welke libraries jij allemaal hebt geïnstalleerd. Die kan gewoon doen:
```bash
python -m venv .venv
```
environment activeren en:
```bash
python -m pip install -r requirements.txt
```
Klaar.

## Waarom dit voor Windows ↔ Linux/RPi zo interessant is

Stel dat je voor een opdracht een Python-programma hebt gemaakt dat een webpagina uitleest.

Je hebt dan:
```markdown
opdracht/
├── main.py
└── requirements.txt
```
Op een Windows computer doe je:
```bash
python -m venv .venv
.venv\Scripts\Activate.ps1
python -m pip install -r requirements.txt
python main.py
```
Maar op je NUC (of ander Linux machine):
```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements.txt
python main.py
```
**De dependencies zijn hetzelfde, maar de virtual environment is anders en wordt lokaal opgebouwd.**

`requirements.txt` is eigenlijk een **lijst** van **Python-packages**.

## Werken met Git en/of GitHub
Wanneer je werkt met Git of GitHub, hoef je de **virtual environment** niet mee op te nemen in de `repository`. Om dit te bekomen, maak je gebruik van de `.gitignore` file:

``` gitignore
.venv/
__pycache__/
*.pyc
```
En dan:
```markdown
mijn_project/
├── .venv/             ← NIET committen
├── __pycache__/       ← NIET committen
├── main.py
├── requirements.txt   ← WEL committen
└── .gitignore         ← WEL committen
```
