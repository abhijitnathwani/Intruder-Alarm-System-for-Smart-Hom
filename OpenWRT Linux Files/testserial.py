import serial
import time
import os
import paho.mqtt.client as mqtt

global s

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if (msg.payload[0]=='6' and msg.payload[1]=='1'):
        click()
    if (msg.payload[0]=='5' and msg.payload[1]=='1'):
        s.write("51")
    if (msg.payload[0]=='5' and msg.payload[1]=='0'):
        s.write("50")
    if (msg.payload[0]=='8' and msg.payload[1]=='1'):
        s.write("81")
    if (msg.payload[0]=='8' and msg.payload[1]=='0'):
        s.write("80")

def on_connect(client, userdata, flags, rc):
    print("Connected with result code"+str(rc))
    client.subscribe("sensorpub")


client = mqtt.Client("Mediatek")
client.on_connect = on_connect
client.on_message = on_message

client.connect("broker.whileone.co.in", 1883, 6000)



s = serial.Serial("/dev/ttyS0",9600, timeout=1)





def click():
    os.system("fswebcam event.jpg")
    f = open("event.jpg","rb")
    imagestring = f.read()
    byteArray = bytearray(imagestring)
    client.publish("imagesub", byteArray ,0)
    client.publish("sensorsub", "60" ,0)
    os.system("rm event.jpg")
    print("Published")


while(1):
    r = s.readline()
    if r:
        print r
        client.publish("sensorsub",r)
        if r[0] is not '8':
            click()
        
    client.loop()
        
        



