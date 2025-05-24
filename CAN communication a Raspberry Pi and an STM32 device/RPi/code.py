import json
import can
import time
import os
import paho.mqtt.client as mqtt
#NguyenNhan:author

host = "rpi"
broker = "127.0.0.1"
port = 1883

topic_in = "domoticz/in"
topic_out = "domoticz/out"
domoticz_temp_idx = 1
domoticz_lux_idx = 2
stm_temp_id = '0x11'
stm_lux_id = '0x22'
stm_led_id = '0x33'
rpi_can_id = 0x407

global bus

def init():
    global bus
    print('CAN Rx test')
    print('Bring up CAN0....')
    os.system("sudo systemctl start mosquitto")
    os.system("sudo /sbin/ip link set can0 up type can bitrate 500000")
    time.sleep(0.1)	

    try:
        bus = can.interface.Bus(channel='can0', bustype='socketcan_native')
    except OSError:
        print('Cannot find CAN board.')
        exit()
	
    print('Ready')

def get_data():
    global bus
    message = bus.recv()	# Wait until a message is received.
	
    if message.dlc == 3:
        id = hex(message.arbitration_id)
        device = hex(message.data[0])
        data = hex((int(message.data[1]) << 8) | int(message.data[2]))

        return {"id": id, "device": device, "data": data}



def connect(host, broker, port):
    '''' connecting to broker '''
    client = mqtt.Client(host)
    client.connect(broker, port)
    client.loop_start()
    return client

def subscribe(client, topics):
    for topic in topics:
        client.subscribe(topic)

def on_message(client, userdata, msg):
    global bus
    if msg.topic == topic_out:
        data = json.loads(msg.payload.decode())
        
        if data['nvalue'] == 0:
            data = [int(stm_led_id, 16), 0]
        elif data['nvalue'] == 1:
            data = [int(stm_led_id, 16), 1]
        
        try:
            msg = can.Message(arbitration_id=rpi_can_id, data=data, is_extended_id=False)
            bus.send(msg)
            print("Message sent on {}".format(bus.channel_info))
        except can.CanError:
            print("Message NOT sent")


def mqtt_setup():
    client = connect(host, broker, port)
    client.on_message = on_message
    client.tls_set()
    print("MQTT: Connected to server")
    subscribe(client, [topic_in, topic_out])
    return client


def main():
    init()
    client = mqtt_setup()
    try:
        while True:
            data = get_data()
            if data:
                if data['device'] == stm_temp_id:
                    val = int(data['data'], 16)
                    print("New Temp Data:" +str(val))
                    json_data = json.dumps({"command":"udevice","idx":domoticz_temp_idx,"nvalue":0,"svalue":str(val)})
                    client.publish(topic_in, json_data, qos=2)
                elif data['device'] == stm_lux_id:
                    val = int(data['data'], 16)
                    print("New Luminosity Data:" +str(val))
                    json_data = json.dumps({"command":"udevice","idx":domoticz_lux_idx,"nvalue":0,"svalue":str(val)})
                    client.publish(topic_in, json_data, qos=2)
    except KeyboardInterrupt:
        #Catch keyboard interrupt
        os.system("sudo /sbin/ip link set can0 down")
        print('\n\rKeyboard interrtupt')

#author-NguyenNhan
if __name__ == "__main__":
    main()
