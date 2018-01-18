""" Programa: smart_light_raspberry.py
    Autor: Estela Romero
    Version: 1.0
    Fecha: 07 Julio 2017
    Descripcion: este programa hace uso de la clase cliente de paho mqtt para publicar en el topic "event/up"
    del broker "broker.hivemq.com" el estado (de tres posbiles: "Dia", "Noche" y "Presencia") que le llega a
    la RPI mediante comunicación serie desde un Arduino UNO que controla el HW sensor.
    A su vez, estamos subscritos al topic "event/down" para capturar y enviar al Arduino la orden de actuacion
    ("0" - ningun led encendido, "1" - un led encendido, "2" - dos leds encendido) sobre los dos leds que controla el mismo. """

import paho.mqtt.client as mqtt
import serial
import time

def on_connect(mosq, obj, rc):
    if rec == 0:
        print "Connected successfully."
    else:
        print "Problem " + str(rc)

def on_publish(mosq, obj, mid):
    global data
    global recibido
    print "Message " + str(mid) + " published."
    recibido = True
    data = ser.readline().strip()
    data = "datoDeData"

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_message(mqttc, obj, msg):
    global instruction
    global enviado
    print "En on message"
    print (msg.topic+" "+str(msg.qos)+" "+str(msg.payload))
    instruction = str(msg.payload)
    ser.write(instruction)
    print "Intruccion" + instruction
    enviado = enviado + 1

def on_log(mosq, obj, level, string):
    print string

broker = "broker.hivemq.com"
topic_up = "event/up"
topic_down = "event/down"
qos = 0

ser = serial.Serial('/dev/ttyUSB0', 9600)
print 'Conectado'


recibido = False
enviado = 0

mqttc = mqtt.Client()
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
mqttc.on_message = on_message
mqttc.on_connet = on_connect
mqttc.on_log = on_log
mqttc.connect(broker, port=1883)

mqttc.loop_start()
while True:
    mqttc.subscribe(topic_down, 0)
    data = ser.readline().strip()
    if data:
        print 'Recibido: %s' % data
        mqttc.publish(topic_up, data)
    time.sleep(10)
mqttc.loop_stop()

