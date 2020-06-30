## Plant disease detect demo

### This demo use openvino and movidius to inference, the models trained base on yolov3 and use flask server to support web service.

### 先解压模型
```
cat yolov3_plant_model.tar.bz2.a* | tar -jx
```

### 启动服务参考flask目录

### 启动后访问服务

```
http://localhost:15000/video_feed
```
