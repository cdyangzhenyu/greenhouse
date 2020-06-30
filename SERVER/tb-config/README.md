## 操作系统要求

```
ubuntu 18.04 
python3
```

## 使用docker镜像启动服务：


```
mkdir /thingsboard
mkdir /thingsboard/log
```

### 启动thingsboard服务端
```
docker run -itd -p 8080:9090 -p 1883:1883 -p 5683:5683/udp -v /thingsboard/data:/data -v /thingsboard/log:/var/log/thingsboard -v /etc/localtime:/etc/localtime --restart=always --name thingsboard-server aiven86/tb-cassandra:awcloud
```
### 初始化thingsboard数据，包括租户，设备
```
GATEWAY
SN0002-RPC
```

### 启动大屏监控服务
```
mkdir /thingsboard/dpws
tar -zxvf htdocs.tar.gz -C /thingsboard/dpws/
```

```
docker run  -itd --net=host -v /thingsboard/dpws/htdocs:/usr/local/apache2/htdocs --restart=always --name dpws-ui aiven86/httpd-dpws
```

### 访问服务

```
大屏访问
127.0.0.1

thingsboard访问：
127.0.0.1:8080

租户用户名：tenant@awcloud.com 
密码：tenant
系统管理员用户：sysadmin@thingsboard.com
密码：sysadmin
```