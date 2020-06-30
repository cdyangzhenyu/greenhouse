## thingsboard的网关zigbee代理

### thingsboard本身的网关服务没办法直接和zigbee对接，需要写对接代理服务。

### 安装部署

```
cp -r zigbee-agent /usr/local/src
cd zigbee-agent
cp greenhouse_agent.service /etc/systemd/system/
systemctl restart greenhouse_agent
systemctl enable greenhouse_agent
```

### 日志路径
```
tail -f /var/log/greenhouse_agent.log
```

### 服务说明
`greenhouse_agent`是thingsboard的gateway对接zigbee模块的agent服务，功能包括：
- 1、从串口读取zigbee发来的消息并解析成正确的json字符发给网关mqtt服务。
- 2、接收外部控制rpc指令，并将指令转成zigbee需要的二进制数据流发送给开发板设备，开发板会根据指令做相应的操作。

### 注意事项：
- 要求thingsboard-gateway服务正常启动
- zigbee串口有数据接收
- 接收格式如下：
```
00e820015c20019920015c2003cf2000dc2007852003c22001a020000020
对应关系：
温度（2byte）20(空格) 空气湿度（2byte）20(空格) 光照（2byte） 20(空格) CO2浓度（2byte） 20(空格) 大气压（2byte） 20(空格) 土壤温度（2byte） 20(空格) 土壤含盐量（2byte） 20(空格) 土壤电导率（2Byte）20 土壤含水量(2byte) 20土壤PH（2byte） 20(空格)
```
- 转换的json格式如下：
```
{"co2": "0.03", "temperature": "23.2", "soilhumidity": "41.6", "serialNumber": "SN-0002", "soiltemperature": "22.0", "humidity": "34.7", "sensorType": "MIX", "pressure": "97.5", "sensorModel": "STM32", "tds": "0.19", "illumination": "409.0", "ph": "0.0", "conductivity": "0.1"}
```
```
co2  二氧化碳 单位 %
conductivity 电导率 单位 s/m
humidity 空气湿度 单位 %
illumination 光照强度 单位 lx
intrusion_count 入侵检测 单位 次
ph 酸碱度 无单位
plant_disease 病害 单位 次
pressure 气压 单位 kPa
soilhumidity 土壤湿度 单位%
soiltemperature 土壤温度 单位 度
tds 含盐量 单位 %
temperature 空气温度 单位 度
```

- rpc控制是走的thingsboard直连模式，没有通过网关服务（网关存在问题），所以需要在thingsboard上额外创建一个虚拟设备，并将对应设备的token写入`greenhouse_agent.py`中的`ACCESS_TOKEN`。

- rpc控制对应的zigbee协议如下：
```
网关端状态查询：FE 06 90 92 02 00（地址） 03（规定的问询指令） 05（PF5引脚） FF
开发板返回数据包:FE 06 92 90 02 00 01 05 FF （目前状态为开启）
网关端控制数据：FE 06（6个数据） 90 92(端口号)  02 00（地址） 01（开启状态，00是关闭状态） 05（PF5引脚） FF
开发板返回数据包：FE 06 92 90 02 00 01 05 FF（代表开启成功） 
```

- rpc控制接口（访问thingsboard server端api）：
```
curl -X POST --header 'Content-Type: application/json' --header 'Accept: application/json' --header 'X-Authorization: Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJ0ZW5hbnRAYXdjbG91ZC5jb20iLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInVzZXJJZCI6IjQzMWI1MzAwLTgwNzMtMTFlYS1iYWQ5LWQ1ODFhY2JkNjUyYiIsImVuYWJsZWQiOnRydWUsImlzUHVibGljIjpmYWxzZSwidGVuYW50SWQiOiI2NmIwYTgwMC04MDZmLTExZWEtODQ2ZC02N2YzNGI0OTdkNDYiLCJjdXN0b21lcklkIjoiMTM4MTQwMDAtMWRkMi0xMWIyLTgwODAtODA4MDgwODA4MDgwIiwiaXNzIjoidGhpbmdzYm9hcmQuaW8iLCJpYXQiOjE1ODc3MjczODAsImV4cCI6MTU4NzczNjM4MH0.R6Mz-V8F11EPZVMt3epN11ySZ26sBUsYSBEdFG2kQZAN1RyniNe4H2w9DXcrGpjhBMm_TUlfhahMH34E4wVzWg' -d '{  \ 
   "method": "getVentilated" \ 
 }' 'http://172.16.1.172:8080/api/plugins/rpc/twoway/cdd78fc0-860b-11ea-b51d-2fe6ff6aba55'

{
  "params": false
}
```

```
curl -X POST --header 'Content-Type: application/json' --header 'Accept: application/json' --header 'X-Authorization: Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJ0ZW5hbnRAYXdjbG91ZC5jb20iLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInVzZXJJZCI6IjQzMWI1MzAwLTgwNzMtMTFlYS1iYWQ5LWQ1ODFhY2JkNjUyYiIsImVuYWJsZWQiOnRydWUsImlzUHVibGljIjpmYWxzZSwidGVuYW50SWQiOiI2NmIwYTgwMC04MDZmLTExZWEtODQ2ZC02N2YzNGI0OTdkNDYiLCJjdXN0b21lcklkIjoiMTM4MTQwMDAtMWRkMi0xMWIyLTgwODAtODA4MDgwODA4MDgwIiwiaXNzIjoidGhpbmdzYm9hcmQuaW8iLCJpYXQiOjE1ODc3MjczODAsImV4cCI6MTU4NzczNjM4MH0.R6Mz-V8F11EPZVMt3epN11ySZ26sBUsYSBEdFG2kQZAN1RyniNe4H2w9DXcrGpjhBMm_TUlfhahMH34E4wVzWg' -d '{  \ 
   "method": "setVentilated",  \ 
   "params":  false \ 
 }' 'http://172.16.1.172:8080/api/plugins/rpc/twoway/cdd78fc0-860b-11ea-b51d-2fe6ff6aba55'

{
  "params": true
}
```

```
[
{'method': 'getVentilated'}, //获取通风开关状态，初始化调用
{'method': 'setVentilated', 'params': false}, //设置通风开关状态为关闭，返回结果如果是{'params': false}则认为已关闭，如果是true则是开着的状态。
{'method': 'getWatering'}, //获取灌溉开关状态，初始化调用
{'method': 'setWatering', 'params': false}, //设置灌溉开关状态为关闭，结果同上

{'method': 'getShading'}, //获取遮光开关状态，初始化调用
{'method': 'setShading', 'params': false}, //设置遮光开关状态为关闭，结果同上

{'method': 'getLighting'}, //获取照明开关状态，初始化调用
{'method': 'setLighting', 'params': false}, //设置照明开关状态为关闭，结果同上

{'method': 'getWetting'}, //获取加湿开关状态，初始化调用
{'method': 'setWetting', 'params': false}, //设置加湿开关状态为关闭，结果同上
]
```
