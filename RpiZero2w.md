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
