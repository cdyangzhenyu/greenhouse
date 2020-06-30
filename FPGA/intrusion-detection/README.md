### compile and install
```
g++ -std=c++11 -o intrusion_detection intrusion_detection.cc `pkg-config --cflags --libs opencv`
cp intrusion_detection /usr/bin/
cp intrusion_detection.service /etc/systemd/system/
systemctl restart intrusion_detection
systemctl enable intrusion_detection
```

