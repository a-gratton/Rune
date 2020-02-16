import paho.mqtt.client as mqtt
import numpy as np

username = "agratton"
password = "monkeymerge"
topic = "gesture"
broker_ip = "192.168.137.241"
PORT = 1883
MIN_LENGTH = 10

gesture_data = np.array

client = mqtt.Client()
client.username_pw_set(username, password)

def connect(client, userdata, flags, rc):
    # rc is the error code returned when connecting to the broker
    print("Connected!", str(rc))
    
    # sub to topic
    client.subscribe(topic)
    fout = open(r"data","w")

def on_message(client, userdata, msg):     
    if msg.payload == 'end':
        if len(gesture.data) > MIN_LENGTH:
            fout.writelines(L for L in gesture_data)
            #call tensorflow function with np.array(gesture_data)
        else:
            pass
        gesture_data = [[None] * 9]

    else:
        gesture_data += [[float(i) for i in msg.payload.split(' ')]]

    
    # The message itself is stored in the msg variable
    # and details about who sent it are stored in userdata

# Here, we are telling the client which functions are to be run
# on connecting, and on receiving a message
client.on_connect = connect
client.on_message = on_message

# Once everything has been set up, we can (finally) connect to the broker
# 1883 is the listener port that the MQTT broker is using
client.connect(broker_ip, PORT)

# Once we have told the client to connect, let the client object run itself
client.loop_forever()
client.disconnect()
