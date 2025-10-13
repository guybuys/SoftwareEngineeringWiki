#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MQTT voorbeeldscript

Benodigdheden:
- paho-mqtt (installeren met: pip install paho-mqtt)
- mqtt_config.py met BROKER_ADDRESS variabele

Gebruik:
- Start het script: python mqtt_example.py
- Typ een bericht om te publishen. Gebruik 'topic:bericht' om een ander topic te kiezen.
- Typ 'exit' of 'quit' om af te sluiten.
"""
import paho.mqtt.client as mqtt
import json
import sys
from mqtt_config import BROKER_ADDRESS

SUB_TOPIC = "test/ontvangst"  # Hardcoded subscribe topic
DEFAULT_PUB_TOPIC = "test/verzenden"  # Default publish topic

def on_connect(client, userdata, flags, rc):
    print(f"Verbonden met broker ({BROKER_ADDRESS}), status: {rc}")
    client.subscribe(SUB_TOPIC)
    print(f"Geabonneerd op topic: {SUB_TOPIC}")

def on_message(client, userdata, msg):
    print(f"Ontvangen op {msg.topic}: {msg.payload.decode('utf-8')}")

def is_json(data):
    try:
        json.loads(data)
        return True
    except Exception:
        return False

def main():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    try:
        client.connect(BROKER_ADDRESS)
    except Exception as e:
        print(f"Kan niet verbinden met broker: {e}")
        sys.exit(1)

    client.loop_start()

    print("Typ een bericht om te publishen. Gebruik 'topic:bericht' om een ander topic te kiezen.")
    while True:
        try:
            user_input = input("> ").strip()
            if not user_input:
                continue
            if user_input.lower() in ["exit", "quit"]:
                break
            if ":" in user_input:
                topic, payload = user_input.split(":", 1)
            else:
                topic, payload = DEFAULT_PUB_TOPIC, user_input
            if is_json(payload):
                client.publish(topic, payload)
                print(f"JSON gepubliceerd naar {topic}")
            else:
                client.publish(topic, payload)
                print(f"String gepubliceerd naar {topic}")
        except KeyboardInterrupt:
            break
        except Exception as e:
            print(f"Fout: {e}")

    client.loop_stop()
    client.disconnect()
    print("Verbonden met broker afgesloten.")

if __name__ == "__main__":
    main()
