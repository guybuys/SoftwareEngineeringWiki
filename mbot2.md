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

# Code Voorbeelden

Het onderstaande voorbeeld is functioneel hetzelfde programma dan het MBOT2 programma dat in het hoofdstuk van het **Finite State Machine** staat met de volgende verbeteringen:
- Alle variabelen werden in een lijst gezet zodat de wildgroei aan **global** variabelen niet nodig is. Voor een variabele toe te voegen, moet de lijst vergroot worden en er moet een index voor zien worden in `class VarIndex`. Er kan eventueel een beginwaarde gegeven worden.
- De **state** van het **FSM** wordt niet meer als een string opgeslagen. Dit maakt dat het veel efficienter wordt om de state te *checken*. Om een state toe te voegen, moet hij toevoegd worden aan `class State` en zijn naam aan `STATE_NAMES`.
- Bij het indrukken van **Knop B**, wacht de code tot de knop losgelaten wordt alvorens van state te veranderen.

```python
import event, time, cyberpi, mbot2, mbuild

# === Enumeraties voor states en variabele-indexen ===
class State:
    STOP = 0
    LINE_FOLLOWER = 1
    T_SHAPE = 2
    UNKNOWN = 3

STATE_NAMES = {
    0: "STOP",
    1: "LINE_FOLLOWER",
    2: "T_SHAPE",
}

class VarIndex:
    STATE = 0
    PREV_STATE = 1
    DEVIATION = 2
    LINE_SENSORS = 3
    KP = 4
    BASE_POWER = 5
    BATTERY_POWER = 6

# === Centrale variabelenlijst ===
# Waarden worden via var[] gelezen en geschreven, constanten zoals KP via index
var = [0] * 7
var[VarIndex.STATE] = State.STOP
var[VarIndex.PREV_STATE] = State.UNKNOWN
var[VarIndex.KP] = 0.3
var[VarIndex.BASE_POWER] = 30
var[VarIndex.BATTERY_POWER] = cyberpi.get_battery()

# Functie om state naar string om te zetten
def state_to_string(s):
    return STATE_NAMES.get(s, "UNKNOWN")

# === Hoofdprogramma ===
@event.start
def on_start():
    cyberpi.console.clear()
    while True:
        state = var[VarIndex.STATE]
        prev_state = var[VarIndex.PREV_STATE]

        # Check of state veranderd is
        new_state = (state != prev_state)
        if new_state:
            var[VarIndex.PREV_STATE] = state
            cyberpi.console.println(state_to_string(state))

        # --- STOP ---
        if state == State.STOP:
            mbot2.EM_stop("ALL")

            current_battery = cyberpi.get_battery()
            if current_battery != var[VarIndex.BATTERY_POWER]:
                var[VarIndex.BATTERY_POWER] = current_battery
                cyberpi.console.print("Battery: ")
                cyberpi.console.print(current_battery)
                cyberpi.console.println("%")

        # --- LINE FOLLOWER ---
        elif state == State.LINE_FOLLOWER:
            deviation = mbuild.quad_rgb_sensor.get_offset_track(1)
            line_sensors = mbuild.quad_rgb_sensor.get_line_sta("all", 1)
            var[VarIndex.DEVIATION] = deviation
            var[VarIndex.LINE_SENSORS] = line_sensors

            if line_sensors == 15:
                mbot2.EM_stop("ALL")
                var[VarIndex.STATE] = State.T_SHAPE
            else:
                kp = var[VarIndex.KP]
                base_power = var[VarIndex.BASE_POWER]
                left_power = base_power - kp * deviation
                right_power = -(base_power + kp * deviation)
                mbot2.drive_power(left_power, right_power)

        # --- T SHAPE ---
        elif state == State.T_SHAPE:
            # Acties voor T-shape...
            var[VarIndex.STATE] = State.STOP

        # --- ONBEKEND STATE ---
        else:
            if new_state:
                cyberpi.console.println("Unknown state:")
                cyberpi.console.println(str(state))

# === Knop A: Stop ===
@event.is_press('a')
def is_btn_press_a():
    var[VarIndex.STATE] = State.STOP

# === Knop B: Start line follower ===
@event.is_press('b')
def is_btn_press_b():
    while cyberpi.controller.is_press('b'):
        time.sleep_ms(50)
    if var[VarIndex.STATE] == State.STOP:
        var[VarIndex.STATE] = State.LINE_FOLLOWER

```