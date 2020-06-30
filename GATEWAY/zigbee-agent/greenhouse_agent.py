# -*- coding: utf-8 -*
import serial
import time
import json
import binascii
import paho.mqtt.client as mqtt
import logging

logger = logging.getLogger(__name__)
logger.setLevel(level = logging.INFO)
handler = logging.FileHandler("/data/log/greenhouse-agent.log")
handler.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)

global gh_metadata
# 打开串口
ser = serial.Serial("/dev/ttyAMA0", 9600)
MQTT_HOST = "127.0.0.1"
MQTT_PORT = 1883

RPC_MQTT_HOST = "192.168.0.10"
RPC_MQTT_PORT = 1883
#RPC_ACCESS_TOKEN = "v0B6KENY2lOjVuWe6KnK"
RPC_ACCESS_TOKEN = "iC0inpxW3V6LfKl7oEqD"

def parse_data(str):
    str=binascii.b2a_hex(str)
    logger.info(str)
    if not str.startswith('fe'):
        return 
    if not str.endswith('ff'):
        return
    data_len = int(str[2:4], 16) - 4
    data = str[6*2:data_len*2+6*2]
    data_pkg = {
            "format": 16,
            "len": str[2:4],
            "src_port": str[4:6],
            "dst_port": str[6:8],
            "addr": str[8:12],
            "data": data,
            "sensor_id": "SN-%s%s" % (str[10:12],str[8:10]),
            }
    logger.info(data_pkg)
    return data_pkg

data_index = ['temperature', 'humidity', 'illumination', 'co2', 'pressure', 
              'soiltemperature', 'tds', 'conductivity', 'soilhumidity', 'ph']

gh_metadata = {
    "device_id": "SN-0002",
    "method": {'Ventilated': '05', 'Watering': '06', 'Shading': '07', 'Lighting': '08', "Wetting":"00"},
    "data_callback": {'Ventilated': '', 'Watering': '', 'Shading': '', 'Lighting': '', "Wetting":""}
    }

def parse_sensor_data(data):
    sensor_data = {}
    if not data:
        return
    sensor_data['serialNumber'] = data['sensor_id']
    sensor_data['sensorType'] = 'MIX'
    sensor_data['sensorModel'] = 'STM32'
    #sensor_data['video_url'] = 'http://127.0.0.1:15000/video_feed'
    #sensor_data['pic_url'] = 'http://127.0.0.1:15001/video_feed'
    data_str = binascii.a2b_hex(data['data'])
    logger.info(data_str)
    try:
        if data['src_port'] == '91':
            items = data_str.split(' ')
            for idx, item in enumerate(items):
                if item:
                    if data_index[idx] == 'conductivity':
                        #sensor_data[data_index[idx]] = str(round(float(int(binascii.hexlify(item), 16))/10000, 2))
                        sensor_data[data_index[idx]] = round(float(int(binascii.hexlify(item), 16))/10000, 2)
                    elif data_index[idx] in ['tds', 'co2']:
                        #sensor_data[data_index[idx]] = str(round(float(int(binascii.hexlify(item), 16))/10000, 2))
                        sensor_data[data_index[idx]] = round(float(int(binascii.hexlify(item), 16))/10000, 2)
                    elif data_index[idx] == 'illumination':
                        #sensor_data[data_index[idx]] = str(float(int(binascii.hexlify(item), 16)))
                        sensor_data[data_index[idx]] = float(int(binascii.hexlify(item), 16))
                    else:
                        #sensor_data[data_index[idx]] = str(float(int(binascii.hexlify(item), 16))/10)
                        sensor_data[data_index[idx]] = float(int(binascii.hexlify(item), 16))/10
        elif data['src_port'] == '92':
            logger.info(data)
            ctl_no = data['data'][2:4]
            s = gh_metadata['method']
            ctl_key = list(s.keys())[list(s.values()).index(ctl_no)]
            gh_metadata['data_callback'][ctl_key] = int(data['data'][0:2])
        '''
        elif data['src_port'] == '93':
            logger.info(data)
            for i in range(4):
                sensor_data[gh_metadata['method'].keys()[i].lower()] = str(bool(int(data['data'][2*i:2*(i+1)])))
            sensor_data['wetting'] = 'False' 
        '''
    except Exception, e:
        logger.error("parse sensor data error!")
        logger.error(e)
        return
    logger.info(sensor_data)
    return sensor_data

RECONNECT_DELAY_SECS = 2

def on_connect(client, userdata, flags, rc):
    logger.info("Connected with result code %s" % rc)

def on_disconnect(client, userdata, rc):
    logger.info("Disconnected from MQTT server with code: %s" % rc)
    while rc != 0:
        sleep(RECONNECT_DELAY_SECS)
        logger.info("Reconnecting...")
        rc = client.reconnect()

def send_mqtt(data):
    if not data:
        return
    res = gw_client.publish("/sensor/data", json.dumps(data), 0)
    logger.info("mqtt publish result: %s, publish data: %s" % (res, json.dumps(data)))

def main():
    while True:
        # 获得接收缓冲区字符
        count = ser.inWaiting()
        if count != 0:
            # 读取内容并回显
            recv = ser.read(count)
            #logger.info(recv)
            send_mqtt(parse_sensor_data(parse_data(recv)))
            #ser.write(recv)
        # 清空接收缓冲区
        ser.flushInput()
        # 必要的软件延时
        time.sleep(0.1)

def parse_zigbee_data(data):
    '''
    data = {
        'device_id': 'SN-0002',
        #'data': True,
        'data': False,
        'ctl_type': 'Ventilated' #Watering,Shading,Lighting,Wetting
    }
    '''
    app_flags = gh_metadata['method']
    dst_port = '92'
    src_port = '90'
    dst_addr = data['device_id'].split('-')[1]
    dst_addr = dst_addr[2:4]+dst_addr[0:2]
    logger.info(dst_addr)
    data['data'] = binascii.b2a_hex(chr(int(data['data'])))
    zb_pkg = src_port+dst_port+dst_addr+data['data']+app_flags[data['ctl_type']]
    logger.info(zb_pkg)
    pkg_len = binascii.b2a_hex(chr(len(binascii.a2b_hex(zb_pkg))))
    zb_pkg = 'FE%s%sFF'%(pkg_len, zb_pkg)
    logger.info(zb_pkg)
    return binascii.a2b_hex(zb_pkg)

def wait_for_status(ctl_key, timeout=3):
    start = time.time()
    while gh_metadata['data_callback'][ctl_key] == '':
        time.sleep(0.1)
        if time.time() - start >= timeout:
            return None
    res = gh_metadata['data_callback'][ctl_key]
    gh_metadata['data_callback'][ctl_key] = ''
    return res

global gw_client
gw_client = mqtt.Client()
gw_client.connect_async(MQTT_HOST, MQTT_PORT, 600)
gw_client.on_connect = on_connect
gw_client.on_disconnect = on_disconnect
gw_client.loop_start()

def on_message(client, userdata, msg):
    logger.info('Topic: ' + msg.topic + '\nMessage: ' + str(msg.payload))
    data = json.loads(msg.payload)
    publish_data = {
         'device_id': gh_metadata['device_id'],
         'data': '',
         'ctl_type': ''
        }
    if 'get' in data['method']:
        publish_data['data'] = '03' # query status
        ctl_type = data['method'].replace('get', '')
        publish_data['ctl_type'] = ctl_type
        logger.info("get request=====")
    if 'set' in data['method']:
        publish_data['data'] = data['params']
        ctl_type = data['method'].replace('set', '')
        publish_data['ctl_type'] = ctl_type
        logger.info("set request=====")
    #logger.info("set %s status success!" % ctl_type)
    ser_data = parse_zigbee_data(publish_data)
    #logger.info(binascii.b2a_hex(ser_data))
    ser.write(ser_data)
    if 'get' in data['method']:
        ctl_status = wait_for_status(ctl_type)
    if 'set' in data['method']:
        ctl_status = data['params']
    logger.info('==%s now status is: %s=='%(ctl_type, ctl_status))
    time.sleep(0.1)
    logger.info(binascii.b2a_hex(ser_data))
    client.publish(msg.topic.replace('request', 'response'), json.dumps({"params": bool(ctl_status)}), 0)
    if 'set' in data['method']:
        sensor_data = {}
        sensor_data[ctl_type.lower()] = str(bool(ctl_status))
        client.publish("v1/devices/me/attributes", json.dumps(sensor_data), 0) 
        logger.info("rpcclient.publish: %s" % json.dumps(sensor_data))

def rpc_on_connect(client, userdata, rc, *extra_params):
    logger.info('Connected with result code ' + str(rc))
    # Subscribing to receive RPC requests
    #client.subscribe('sensor/SN-0002/request/no-reply/+')
    client.subscribe('v1/devices/me/rpc/request/+')

rpcclient = mqtt.Client()
# Register connect callback
rpcclient.on_connect = rpc_on_connect
# Registed publish message callback
rpcclient.on_message = on_message
rpcclient.username_pw_set(RPC_ACCESS_TOKEN)
rpcclient.connect_async(RPC_MQTT_HOST, RPC_MQTT_PORT, 600)
rpcclient.on_disconnect = on_disconnect
rpcclient.loop_start()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        if ser != None:
            ser.close()
