## 安装thingsboard-gateway服务

```
apt install libffi-dev
apt install mosquitto
pip install paho-mqtt==1.5.0
apt install ./python3-thingsboard-gateway.deb -y
cp tb-config/mqtt.json /etc/thingsboard-gateway/config/
cp tb-config/tb_gateway.yaml /etc/thingsboard-gateway/config/
```

### 根据实际情况修改配置tb_gateway.yaml：

```
  security:
    accessToken: tQ7NlG5tWOGuU3WJKPJV
```

上面的`accessToken`是thingsboard服务中的gateway的token