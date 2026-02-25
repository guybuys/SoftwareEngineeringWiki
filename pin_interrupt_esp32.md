# Pin interrupt in MicroPython (ESP32)

Deze korte gids legt uit hoe je pin interrupts gebruikt op een ESP32 met MicroPython.

## Basis

In MicroPython gebruik je een `Pin` object en registreer je een interrupt via `irq()`.
De interrupt trigger definieer je met een bitmask van `Pin.IRQ_RISING` en/of
`Pin.IRQ_FALLING`.

## Rising of falling

```python
from machine import Pin

button = Pin(14, Pin.IN, Pin.PULL_UP)

def handle_irq(pin):
    print("IRQ on pin", pin)

# Alleen rising
a = button.irq(trigger=Pin.IRQ_RISING, handler=handle_irq)

# Alleen falling
b = button.irq(trigger=Pin.IRQ_FALLING, handler=handle_irq)
```

## Zowel rising als falling

Gebruik bitwise OR om beide triggers te combineren.

```python
from machine import Pin

button = Pin(14, Pin.IN, Pin.PULL_UP)

def handle_irq(pin):
    print("IRQ on pin", pin)

# Rising en falling combineren
irq = button.irq(
    trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING,
    handler=handle_irq,
)
```

## Tijdstempel en afhandeling in de main loop

Sla in de interrupt enkel een tijdstempel en een vlag op, en werk het verder af
in je hoofdloop. Dit houdt de interrupt kort en voorspelbaar.

```python
from machine import Pin
import utime

button = Pin(14, Pin.IN, Pin.PULL_UP)

irq_flag = False
irq_time_us = 0

def handle_irq(pin):
    global irq_flag, irq_time_us
    irq_time_us = utime.ticks_us()
    irq_flag = True

button.irq(
    trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING,
    handler=handle_irq,
)

while True:
    if irq_flag:
        irq_flag = False
        now_ms = utime.ticks_ms()
        age_ms = utime.ticks_diff(now_ms, irq_time_us // 1000)
        print("IRQ gezien", age_ms, "ms geleden")
    utime.sleep_ms(1)
```

## Opmerkingen

- De handler moet zo kort mogelijk zijn. Vermijd lang blokkerende code in de interrupt; `print()` is traag en daarom geen goed idee in een interrupt.
- Je kan de interrupt later uitzetten met `irq.disable()`.
- Voor debouncing is vaak een timer of softwarematige filtering nodig.
