# Korte introductie tot de Raspberry Pi Zero 2 W

## Operating system

Om de Raspberry Pi Zero 2 W te gebruiken, moet je eerst een operating system installeren. Dit doe je met de **Raspberry Pi Imager**. Op het moment van het maken van dit document, was de recentste versie `v2.0.0`.

Bij **Device** kies je dan **Raspberry Pi Zero 2 W** en daarna klik je **NEXT**.
Voor het operating system neem je bij voorkeur iets licht, zodat het meeste processor power naar de applicaties kan gaan. Kies bijvoorbeeld eerst **Raspberry Pi OS (other)** en daar **Raspberry Pi OS Lite (32-bit) A port of debian Trixie with no desktop environment

Bij **Storage** kies je het microSD kaartje dat je gaat gebruiken.

**Customisation**: Nog te fine tunen:
- Hostname instellen: gebruik een unieke naam want er kunnen meerdere Raspberry Pi's op het (school) netwerk zitten. Vb RPITSM3 (want 1 en 2 bestaan al. ToDo: maak lijst van borden en hostnames)
- User instellen (om te kunnen verbinden via SSH) 
- Wifi instellen enzo.

**Writing**

**Done**

Steek het kaartje in de RPI en power het bordje.

---

## WiFi netwerken toevoegen

Omdat we het bordje *headless* gebruiken (zonder toetsenbord en scherm), stellen we alles in via SSH. Als we dus een ander netwerk willen toevoegen, moeten we dit op een bestaand netwerk doen.

**Sinds Raspberry Pi OS Bookworm** (en dus ook **Trixie**) wordt **NetworkManager** gebruikt i.p.v. `wpa_supplicant` rechtstreeks.
Daarom staat er geen `/etc/wpa_supplicant/wpa_supplicant.conf` meer zoals ChatGPT waarschijnlijk zal voorstellen.

Je moet nu **NetworkManager** gebruiken om extra wifi-netwerken toe te voegen.

Goed nieuws: dat werkt heel eenvoudig via SSH of de terminal. Eerst kan je best controleren of **NetworkManager** actief is

```bash
nmcli general status
```
Je zou iets moeten zien zoals:
```bash
STATE   CONNECTIVITY  WIFI-HW  WIFI...
connected  ...
```
Als dat het geval is, dan gebruik je NetworkManager.

We moeten het nieuwe WiFi-profiel manueel toevoegen, omdat het andere netwerk niet aanwezig is.

```bash
sudo nmcli connection add type wifi ifname wlan0 con-name TSM-Graad23 ssid "TSM-Graad23"
```
En stel dan de beveiliging en het wachtwoord in:
``` bash
sudo nmcli connection modify TSM-Graad23 wifi-sec.key-mgmt wpa-psk
sudo nmcli connection modify TSM-Graad23 wifi-sec.psk '********'
```
> Gebruik enkele quotes rond het wachtwoord! Ander kan hij karakters zoals bijvoorbeeld `!` *interpreteren*, wat niet de bedoeling is.

Controleer of het nieuwe netwerk netjes in de lijst staat:
``` bash
nmcli connection show
```
---
## OpenCV installeren

OpenCV (Open Source Computer Vision Library) is een open-source bibliotheek voor computervisie en machine learning. Het bevat meer dan 2500 geoptimaliseerde algoritmes voor taken zoals:

- Beeld- en videoverwerking
- Objectdetectie en -herkenning
- Gezichtsdetectie en -herkenning
- Bewegingstracking
- 3D-reconstructie
- Detectie en matching van kenmerken
- Camerakalibratie

Het is geschreven in C++ maar heeft bindings voor Python, Java en andere talen. OpenCV wordt veel gebruikt in robotica, autonome voertuigen, augmented reality en verschillende AI-toepassingen. Het is platformonafhankelijk en werkt op Windows, Linux, macOS, Android en iOS.

Voor OpenCV te installeren, moet je dit uitvoeren:
``` bash
sudo apt update
sudo apt install python3-opencv
```
Controleer daarna:
``` bash
python3 -c "import cv2; print(cv2.__version__)"
```

Je zou iets krijgen als: `4.10.0`

## Installeer paho-mqtt (Python 3)

Voor het geval dat je mqtt nodig hebt.

Voer uit:
``` bash
sudo apt update
sudo apt install python3-paho-mqtt
```

Daarna testen:
``` bash
python3 -c "import paho.mqtt.client as mqtt; print('OK')"
```

## Installatie van pupil-apriltags op een Raspberry Pi

> Hier heb ik verschrikkelijk lang mee geworsteld. Het is mogelijk dat ik niet alle tussenstappen goed genoteerd heb hier. Ik heb in elk geval eerst OpenCV en numpy geinstalleerd. En daarna pas met de virtuele Python omgeving omdat ik de error `error: externally-managed-environment` kreeg. Dit mislukte vaak en ik ben vaak opnieuw begonnen met een nieuwe virtuele omgeving te maken. 

### 1. Update je systeem

Zorg dat je Raspberry Pi up-to-date is.
``` bash
sudo apt update
sudo apt upgrade -y
```

Dit zorgt ervoor dat alle pakketten en beveiligingsupdates actueel zijn.

### 2. Installeer vereiste pakketten

Installeer de basistools om Python-pakketten te bouwen.
> Ik ben niet zeker of dit moet, ik herinner me niet dat ik dat gedaan heb ...

``` bash
sudo apt install python3-venv python3-pip cmake build-essential -y
sudo apt install python3-opencv python3-numpy -y
```

python3-venv maakt het mogelijk een virtuele omgeving te maken.
cmake en build-essential zijn nodig om C-extensies te compileren.
python3-opencv en python3-numpy zijn vooraf vereisten voor pupil-apriltags.

### 2. Essentiële build-tools installeren 

> Deze herinner ik me wel, weet alleen niet precies meer waar, wanneer en waarom ik het moest doen. 

Dit is nodig zodat Python pakketten zoals pupil-apriltags überhaupt kunnen bouwen:
``` bash
sudo apt install -y python3-dev python3-pip python3-setuptools python3-wheel
```

### 3. Maak een virtuele Python omgeving aan

Dit voorkomt conflicten met systeem-pakketten.
``` bash
python3 -m venv --system-site-packages ~/apriltag_env
```
--system-site-packages zorgt ervoor dat alle Python-pakketten die al op het systeem staan (zoals python3-numpy en python3-opencv) beschikbaar blijven in de virtuele omgeving.

Daardoor hoef je deze zware pakketten niet opnieuw te installeren binnen de venv. Alleen pupil-apriltags wordt dan nieuw geïnstalleerd.

Hierdoor wordt een eigen geïsoleerde Python-omgeving aangemaakt.

### 4. Activeer de virtuele omgeving
``` bash
source ~/apriltag_env/bin/activate
```

Je ziet nu (apriltag_env) voor je prompt. Alle Python-installaties gebeuren nu in deze omgeving.

### 5. Zorg voor extra geheugen (optioneel, bij RPi Zero)

Omdat de installatie van pupil-apriltags veel RAM gebruikt, kun je de swapfile vergroten:

Versie van ChatGPT:
``` bash
sudo fallocate -l 2G /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
```
Die eerste stap deed ik niet of werkte niet. Wat ik deed:
``` bash
sudo swapoff -a
sudo dd if=/dev/zero of=/swapfile bs=1M count=2048
``` 
Dat gaf iets van:
```
2048+0 records in
2048+0 records out
2147483648 bytes (2.1 GB, 2.0 GiB) copied, 110.12 s, 19.5 MB/s
```
Daarna deed ik
``` bash
sudo chmod 600 /swapfile
sudo mkswap /swapfile
```
Dit gaf:
```
Setting up swapspace version 1, size = 2 GiB (2147479552 bytes)
no label, UUID=e1a70af4-36e4-4719-803d-ae08d2ef2bbe
```
Daarna deed ik:
``` bash
sudo swapon /swapfile
sudo nano /etc/fstab
```
In die file voegde ik dit onderaan toe:
`/swapfile none swap sw 0 0`

Daarna **Ctrl +  O** om te saven en **Ctrl + X** om te verlaten.
``` bash
free -h
```
Geeft dan:
```
               total        used        free      shared  buff/cache   available
Mem:           425Mi       121Mi       183Mi       3.2Mi       177Mi       304Mi
Swap:          2.0Gi          0B       2.0Gi
```

De swapfile dient als tijdelijk extra geheugen. Zonder voldoende geheugen kan de installatie mislukken, wat bij mij dan ook gebeurde: `cc: fatal error: Killed signal terminated program cc1`, het programma werd gekilld door gebrek aan RAM.

### 6. Installeer pupil-apriltags
``` bash
pip install --no-cache-dir pupil-apriltags
```

--no-cache-dir zorgt dat er geen oude buildbestanden gebruikt worden, wat kan helpen bij beperkte opslag.

Op de RPi Zero kan dit enkele minuten duren, omdat de C-extensies gecompileerd worden.

### 7. Controleer de installatie

Maak een testbestand test_apriltags.py:
``` python
import numpy
import cv2
from pupil_apriltags import Detector

print("Alles is aanwezig!")
```

Voer het script uit:
``` bash
python test_apriltags.py
```

Als je "Alles is aanwezig!" ziet, is alles correct geïnstalleerd.

### 8. Deactiveer de virtuele omgeving (optioneel)
``` bash
deactivate
```

Je keert terug naar het systeem-Python. Activeer de omgeving opnieuw met `source ~/apriltag_env/bin/activate` als je later aan dit project werkt.