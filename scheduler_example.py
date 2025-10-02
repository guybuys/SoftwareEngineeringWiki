import time
from scheduler import Scheduler, Task

gps_data_ready = False  # Dummy vlag voor interrupt

def uart_interrupt_handler():
    global gps_data_ready
    gps_data_ready = True

def gps_condition():
    global gps_data_ready
    if gps_data_ready:
        gps_data_ready = False
        return True
    return False

def read_gps():
    print("GPS data gelezen (dummy)")

def read_pressure():
    print("Druksensor gelezen (dummy)")

def read_temperature():
    print("Temperatuursensor gelezen (dummy)")

def send_data():
    print("Gemiddelde data verzonden (dummy)")

scheduler = Scheduler()
scheduler.add_task(Task("GPS", 1000, read_gps, condition=gps_condition))
scheduler.add_task(Task("Druk", 5, read_pressure))
scheduler.add_task(Task("Temperatuur", 100, read_temperature))
scheduler.add_task(Task("Verzenden", 1000, send_data))

# Simuleer een interrupt (voor demo)
uart_interrupt_handler()

# Start de scheduler (voor demo: 1 cyclus)
for _ in range(1100):
    for task in scheduler.tasks:
        if task.should_run():
            task.run()
    time.sleep_ms(1)
