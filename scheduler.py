import time

class Task:
    def __init__(self, name, interval_ms, func, condition=None):
        self.name = name
        self.interval_ms = interval_ms
        self.func = func
        self.last_run = time.ticks_ms()
        self.warning = False
        self.condition = condition  # Optional function that returns True/False

    def should_run(self):
        now = time.ticks_ms()
        tijd_ok = time.ticks_diff(now, self.last_run) >= self.interval_ms
        cond_ok = True if self.condition is None else self.condition()
        return tijd_ok and cond_ok

    def run(self):
        start = time.ticks_ms()
        self.func()
        end = time.ticks_ms()
        duration = time.ticks_diff(end, start)
        if duration > self.interval_ms:
            self.warning = True
            print("Warning: task '{}' took {}ms (interval: {}ms)".format(self.name, duration, self.interval_ms))
        else:
            self.warning = False
        self.last_run = end

class Scheduler:
    def __init__(self):
        self.tasks = []

    def add_task(self, task):
        self.tasks.append(task)

    def run(self):
        while True:
            for task in self.tasks:
                if task.should_run():
                    task.run()
            time.sleep_ms(1)  # Small pause to save CPU
