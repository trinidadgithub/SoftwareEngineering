import paho.mqtt.client as mqtt

# Define the MQTT settings
broker_url = "your-iot-endpoint.amazonaws.com"
broker_port = 8883
topic = "iot/devices/status"

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(topic)

def on_message(client, userdata, msg):
    print(f"Message received: {msg.payload.decode()}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# Setup TLS and authentication
client.tls_set(ca_certs="certificates/AmazonRootCA1.pem",
               certfile="certificates/device-certificate.pem.crt",
               keyfile="certificates/device-private.pem.key")

# Connect to the broker
client.connect(broker_url, broker_port)

client.loop_forever()
