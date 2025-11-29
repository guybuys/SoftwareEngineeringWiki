# Raspberry Pi headless

Deze introductie legt uit hoe je een Raspberry Pi zonder monitor, toetsenbord of muis (headless) gebruikt. De stappen zijn in principe hetzelfde voor alle Raspberry Pi‑modellen, inclusief de Raspberry Pi Zero 2 W. Het enige verschil kan de stroomvoorziening en prestaties zijn.

## Wat betekent "headless"?
- Je beheert de Pi op afstand via je netwerk, meestal met `SSH` (command line) en optioneel bestandsbeheer (SFTP).
- Je hebt geen rechtstreeks aangesloten scherm of invoerapparatuur nodig.
- Handig voor kleine projecten, IoT, servers en krappe opstellingen.

## Benodigdheden
- **Raspberry Pi**: bijv. `Raspberry Pi Zero 2 W` + geschikte voeding.
- **MicroSD‑kaart**: 8–32 GB (A1/V30 aanbevolen) + kaartlezer.
- **Netwerk**: Wi‑Fi (Zero 2 W) of Ethernet (andere modellen).
- **Computer**: Windows/macOS/Linux.
- **Software**:
	- Windows: `Raspberry Pi Imager`, `PuTTY` (SSH), `WinSCP` (SFTP/bestanden).
	- macOS: `Raspberry Pi Imager`, Terminal (`ssh` ingebouwd), `Cyberduck` of `FileZilla` (SFTP).
	- Linux: `Raspberry Pi Imager`, Terminal (`ssh`), `FileZilla`/`sftp`.

Links (zoek in je store als nodig):
- Raspberry Pi Imager: https://www.raspberrypi.com/software/
- PuTTY: https://www.putty.org/
- WinSCP: https://winscp.net/

## SD‑kaart maken met Raspberry Pi Imager
1. **Installeer en start** Raspberry Pi Imager.
2. Klik op **Choose OS** en kies een geschikte image:
	 - `Raspberry Pi OS (32-bit)` is een goede start; voor headless volstaat Lite.
3. Klik op **Choose Storage** en selecteer je microSD‑kaart.
4. Klik op **Settings** (tandwieltje) of gebruik `Ctrl+Shift+X` om vooraf te configureren:
	 - **Set hostname**: bijv. `RPITSM5` maar kijk na dat de naam nog niet gebruikt is.
	 - **Enable SSH**: vink aan; kies `Use password authentication`.
	 - **Set username and password**: stel een eigen gebruiker/wachtwoord in.
	 - **Configure wireless LAN**: vul `SSID`, `Password`, en kies het **land** (BE/NL).
	 - **Set locale settings**: tijdzone en keyboard‑layout.
5. Bevestig en klik **Write**. Wacht tot het schrijven en verifiëren klaar is.
6. Verwijder de kaart veilig en plaats die in de Raspberry Pi. Geef stroom; na ~30–60s zou de Pi op het netwerk komen.

## Eerste verbinding (SSH)
- Zoek het IP‑adres:
	- Router/DHCP‑lijst, of `ping RPITSM5` / `ssh raspberrypi@raspberrypi.local` (mDNS).
- Verbind via SSH:
	- Windows met **PuTTY**: host `RPITSM5` of IP (dat zie je na de *ping*), poort `22`, login met ingestelde gebruikersnaam/wachtwoord.
	- macOS/Linux met Terminal:
		```bash
		ssh <gebruikersnaam>@raspberrypi.local
		# of
		ssh <gebruikersnaam>@<IP-adres>
		```
- Bestanden overzetten (SFTP):
	- Windows: **WinSCP** (protocol SFTP, host `RPITSM5`/IP, poort 22).
	- macOS/Linux: **FileZilla** of Terminal `sftp`.

## Eerste stappen op de Pi
```bash
# systeem updaten
sudo apt update && sudo apt full-upgrade -y

# optioneel: nuttige tools
sudo apt install -y git vim htop curl
```

## Tips & best practices
- **Stroomvoorziening**: gebruik een goede 5V‑voeding; voor Zero 2 W vaak 5V/2A.
- **Wi‑Fi signaal**: plaats de Pi niet te ver van de router.
- **Beveiliging**: verander standaard wachtwoorden; overweeg SSH‑sleutels en uitzetten van wachtwoord‑login.
- **Hostname**: geef een unieke naam zodat `*.local` (mDNS) werkt en je hem snel herkent.
- **Back‑ups**: maak een image of kopieer belangrijke projectbestanden regelmatig.
- **Andere Wi-Fi netwerken**: wil je de Raspberry Pi op een andere plaats gebruiken, voeg dan eerst de **SSID** en **password** toe.

## Veelgestelde vragen
- **Is headless hetzelfde voor alle Pi's?** Ja, de aanpak via Imager + SSH is hetzelfde. Alleen netwerk (Wi‑Fi/Ethernet) en performance verschillen per model.
- **Geen verbinding?** Controleer voeding, Wi‑Fi‑gegevens, landinstellingen, router‑DHCP, en probeer het IP te pingen. Eventueel herflashen met correcte SSID/wachtwoord.

Met deze stappen kun je snel een Raspberry Pi Zero 2 W (en andere modellen) headless inzetten.
