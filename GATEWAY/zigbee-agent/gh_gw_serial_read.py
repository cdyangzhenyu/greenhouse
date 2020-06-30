# -*- coding: utf-8 -*
import serial
import time
import json
import binascii
import paho.mqtt.client as mqtt
import logging

logger = logging.getLogger(__name__)
logger.setLevel(level = logging.INFO)
handler = logging.FileHandler("/data/log/gh_gw_serial_read.log")
handler.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)

# 打开串口
ser = serial.Serial("/dev/ttyAMA0", 9600)
MQTT_HOST = "127.0.0.1"
MQTT_PORT = 1883

def print_hex_str(str):
    print len(str)
    print str
    str1=binascii.b2a_hex(str)
    print str1

def parse_data(str):
    str=binascii.b2a_hex(str)
    print(str)
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

def parse_sensor_data(data):
    sensor_data = {}
    if not data:
        return
    sensor_data['serialNumber'] = data['sensor_id']
    sensor_data['sensorType'] = 'MIX'
    sensor_data['sensorModel'] = 'STM32'
    sensor_data['video_url'] = 'http://127.0.0.1:15000/video_feed'
    sensor_data['pic_url'] = 'http://127.0.0.1:15001/video_feed'
    data_str = binascii.a2b_hex(data['data'])
    logger.info(data_str)
    try:
        if data['src_port'] == '91':
            items = data_str.split(' ')
            for idx, item in enumerate(items):
                if item:
                    if data_index[idx] == 'conductivity':
                        sensor_data[data_index[idx]] = str(round(float(int(binascii.hexlify(item), 16))/10000, 2))
                    elif data_index[idx] in ['tds', 'co2']:
                        sensor_data[data_index[idx]] = str(round(float(int(binascii.hexlify(item), 16))/10000, 2))
                    elif data_index[idx] == 'illumination':
                        sensor_data[data_index[idx]] = str(float(int(binascii.hexlify(item), 16)))
                    else:
                        sensor_data[data_index[idx]] = str(float(int(binascii.hexlify(item), 16))/10)
    except Exception, e:
        logger.error("parse sensor data error!")
        logger.error(e)
        return
    print(sensor_data)
    return sensor_data

def send_mqtt(data):
    if not data:
        return
    res = client.publish("/sensor/data", json.dumps(data), 0)
    logger.info("mqtt publish result: %s, publish data: %s" % (res, json.dumps(data)))

def main():
    while True:
        # 获得接收缓冲区字符
        count = ser.inWaiting()
        if count != 0:
            # 读取内容并回显
            recv = ser.read(count)
            #print(recv)
            send_mqtt(parse_sensor_data(parse_data(recv)))
            #ser.write(recv)
        # 清空接收缓冲区
        ser.flushInput()
        # 必要的软件延时
        time.sleep(0.1)

if __name__ == '__main__':
    try:
        client = mqtt.Client()
        client.connect(MQTT_HOST, MQTT_PORT, 60)
        main()
    except KeyboardInterrupt:
        if ser != None:
            ser.close()
