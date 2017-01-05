import os
import time
import sys
reload(sys)
import paho.mqtt.client as mqtt
sys.setdefaultencoding('utf-8')

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    f = open('receivedimage.jpg', 'w')
    f.write(msg.payload)
    f.close()
    

def on_connect(client, userdata, flags, rc):
    print("Connected with result code"+str(rc))


client = mqtt.Client("Mediatek")
client.on_connect = on_connect
#client.on_message = on_message

client.connect("broker.whileone.co.in", 1883, 6000)


#os.system("fswebcam abhi.jpg")

f = open("abhi.jpg","rb")
imagestring = f.read()
byteArray = bytearray(imagestring)
client.publish("imagesub", byteArray ,0)
print("Published")
# If the image is large, just calling publish() won't guarantee that all 
# of the message is sent. You should call one of the mosquitto.loop*()
# functions to ensure that happens. loop_forever() does this for you in a
# blocking call. It will automatically reconnect if disconnected by accident
# and will return after we call disconnect() above.
client.loop_forever()