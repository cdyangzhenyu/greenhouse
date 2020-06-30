
安装openvino_2020.1.023
下载地址：http://10.1.1.105/resources/tools/l_openvino_toolkit_fpga_p_2020.1.023.tgz
```
sh install.sh
```
注意去除fpga相关的安装选项

openvino运行依赖的ld可以放到:
配置openvino库
```
cat /etc/ld.so.conf.d/openvino.conf 
/opt/intel/openvino_2020.1.023/deployment_tools/ngraph/lib
/opt/intel/opencl
/opt/intel/openvino_2020.1.023/deployment_tools/inference_engine/external/hddl/lib
/opt/intel/openvino_2020.1.023/deployment_tools/inference_engine/external/mkltiny_lnx/lib
/opt/intel/openvino_2020.1.023/deployment_tools/inference_engine/external/tbb/lib
/opt/intel/openvino_2020.1.023/deployment_tools/inference_engine/lib/intel64
```

使配置生效
```
ldconfig
```

安装openvino的python库，注意选择正确的python版本
```
cd /opt/intel/openvino/python/python3.6/
pip3 install -r requirements.txt
cp -r * /usr/lib/python3/dist-packages/
```

安装服务需要的字体
```
mkdir /usr/share/fonts/simhei
cp simhei.ttf /usr/share/fonts/simhei/
chmod -R 755 /usr/share/fonts/simhei/
```

安装，修改以下配置中的路径：
```
cp -r plant-disease-detect /usr/local/src/
cd plant-disease-detect 
pip install -r requirements.txt
cp plant-flask-*.service /etc/systemd/system/
systemctl restart plant-flask-server
systemctl restart plant-flask-client
systemctl enable plant-flask-server
systemctl enable plant-flask-client
```

其他依赖：
```
python3.7
vidgear
movidius
opencv
tensorflow
```

### 需要有摄像头
### 需要有NCS计算棒
 
