# MBOT2

In dit document verzamelen we de *ge-reverse-engineerde* data van de **MBOT2**.

Deze data moet nog worden nagekeken. Sommige meetdata bleek niet *consistent* te zijn, mogelijk werden er fouten gemaakt tijdens de meting of het documenteren.


| Methode |	Waarde | Gemiddelde (µs) |Min (µs)	| Max (µs)	| Opmerkingen |
|---------|--------|-----------------|-----------|-----------|-------------|
|mbuild.quad_rgb_sensor.get_red("L2", 1) <br>mbuild.quad_rgb_sensor.get_green("L2", 1)<br>mbuild.quad_rgb_sensor.get_blue("L2", 1)|0..255<br>0..255<br>0..255|≈ 110 000	|100 000	|208 600	|Na elkaar uitlezen van de RGB waarden van linker sensor|
|mbuild.quad_rgb_sensor.get_red("R2", 1) <br>mbuild.quad_rgb_sensor.get_green("R2", 1) <br>mbuild.quad_rgb_sensor.get_blue("R2", 1)	|0..255<br>0..255<br>0..255|≈ 113 000	|85 000	|133 000	|Na elkaar uitlezen van de RGB waarden van rechter sensor|
|mbuild.quad_rgb_sensor.get_offset_track(1)	|-100..100|≈ 15 000	|2 477	|42 287	|Berekening van afwijking (“deviation”) t.o.v. lijn|
|mbuild.quad_rgb_sensor.get_line_sta("middle", 1)	|?|≈ 1 000	|574	|20 400	|Lichtintensiteit (line sensor) uitlezen|
|mbuild.quad_rgb_sensor.get_line_sta("all", 1)|0..15|≈ 600	|444	|5 558	|4 bits van de lijnsensor|
|mbuild.quad_rgb_sensor.get_green_sta("all", 1)	|0..15|≈ 1 100	|862	|2 769	|4 bits van de groenherkenning|
|mbuild.quad_rgb_sensor.get_custom_sta("all", 1)|0..15|≈ 37 300	|32 821	|44 471	|4 bits van de “user-defined”-kleurherkenning|
|cyberpi.led.on(r, g, b, 1) |rgb|≈ 2 500	|647	|6 462	|LED-aansturing|
|cyberpi.get_battery()|0..100|≈ 20 000	| -	|-	|Batterij percentage opvragen|

## Besluit

Ondanks dat er bedenkingen zijn bij de verzamelde data dus omzichtigheid geboden is, zijn de volgende opmerkingen waarschijnlijk aan de orde:
- Het gebruik van LED's om te debuggen kan beter vermeden worden in tijdskritische routines.
- De methode `get_green_sta` heeft zeer bruikbare (korte) executietijden maar helaas wordt de kleur groen van het bord niet altijd correct herkend.
- De methode `get_custom_sta` levert zeer bruikbare resultaten qua kleurherkenning maar de executietijden duren heel erg lang. In vergelijking met het uitlezen van de RGB waarden van 1 RGB-sensor, is deze methode nog steeds ~3 keer sneller dus waarschijnlijk de beste optie.