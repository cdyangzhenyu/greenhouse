<template>
  <div id="app">
    <VHeader :name="greenhouse"></VHeader>
    <div class="main">
      <div class="main-left">
        <VBorder class="main-left-top">
          <VEcharts
            v-if="showEcharts"
            key="1"
            :option="insectInfo"
            ref="insect"
          ></VEcharts>
        </VBorder>
        <VBorder class="main-left-bottom">
          <VEcharts
            v-if="showEcharts"
            class="police-echart"
            key="2"
            :option="policeInfo"
            ref="police"
          ></VEcharts>
          <div class="police-image">
            <h6>闯入抓拍</h6>
            <div class="image">
              <img
                v-if="urlInfo.picUrl"
                v-bind:src="urlInfo.picUrl"
                height="100%"
                width="100%"
              />
            </div>
          </div>
        </VBorder>
      </div>
      <div class="main-center">
        <div class="main-center-top">
          <VMenu
            class="main-center-menu"
            v-for="(_m, _mId) in menuTopList"
            :key="_mId"
          >
            <div class="main-center-menu-icon">
              <span class="icon" :class="[_m.icon]">
                <i></i>
              </span>
            </div>
            <div class="main-center-menu-text">
              <div class="text-top">
                <span>{{ dataInfo[_m.value] | getValue }}</span>
                <span class="tip">{{ _m.tip }}</span>
              </div>
              <div class="text-bottom">{{ _m.text }}</div>
            </div>
          </VMenu>
        </div>
        <div class="main-center-top">
          <VMenu
            class="main-center-menu"
            v-for="(_m, _mId) in menusCenterList"
            :key="_mId"
          >
            <div class="main-center-menu-icon">
              <span class="icon" :class="[_m.icon]">
                <i></i>
              </span>
            </div>
            <div class="main-center-menu-text active">
              <div class="text-top">
                <span>{{ dataInfo[_m.value] | getValue }}</span>
                <span class="tip">{{ _m.tip }}</span>
              </div>
              <div class="text-bottom">{{ _m.text }}</div>
            </div>
          </VMenu>
        </div>
        <div class="main-center-center">
          <VBorder class="greenhouse-info">
            <div class="title">{{ greenhouseInfo.name }}</div>
            <div class="text">
              <span class="label">项目ID: </span>
              <span>{{ greenhouseInfo.id }}</span>
            </div>
            <div class="text">
              <span class="label">大棚类型: </span>
              <span>{{ greenhouseInfo.type }}</span>
            </div>
            <div class="text">
              <span class="label">种植类型: </span>
              <span>{{ greenhouseInfo.plantingType }}</span>
            </div>
          </VBorder>
          <div class="greenhouse greenhouse-left">
            <span class="line">
              <i class="line-spot"></i>
              <i class="line-line"></i>
            </span>
            <span class="camera" @click="openCamera"></span>
            <span class="camera-line" v-show="showVideo">
              <i class="line-spot"></i>
              <i class="line-line"></i>
            </span>
          </div>
          <div class="greenhouse greenhouse-right"></div>
          <VBorder class="video" v-if="showVideo">
            <div class="video-centent">
              <img
                v-if="urlInfo.videoUrl"
                v-bind:src="urlInfo.videoUrl"
                height="100%"
                width="100%"
              />
            </div>
          </VBorder>
        </div>
        <VMenu class="main-center-bottom">
          <div
            class="main-center-bottom-menu"
            v-for="(_m, _mid) in menuBottomList"
            :key="_mid"
            @click="setSwitchRpc(_m)"
          >
            <span class="text">{{ _m.text }}</span>
            <v-switch :active="_m.isOpen"></v-switch>
          </div>
        </VMenu>
      </div>
      <div class="main-right">
        <VBorder class="main-right-top">
          <VEcharts
            v-if="showEcharts"
            key="5"
            :option="temperatureInfo"
            ref="temperature"
          ></VEcharts>
        </VBorder>
        <VBorder class="main-right-center">
          <VEcharts
            v-if="showEcharts"
            key="6"
            :option="humidityInfo"
            ref="humidity"
          ></VEcharts>
        </VBorder>
        <VBorder class="main-right-bottom">
          <VEcharts
            v-if="showEcharts"
            key="7"
            :option="IlluminationOnfo"
            ref="illumination"
          ></VEcharts>
        </VBorder>
      </div>
    </div>
  </div>
</template>

<script>
import VHeader from "./components/Header.vue";
import VBorder from "./components/Border.vue";
import VMenu from "./components/Menu.vue";
import VEcharts from "./components/Echarts.vue";
import VSwitch from "./components/switch.vue";
import { GetTime } from "./js/utils";
import {
  Login,
  GetDataInfo,
  GetTemperature,
  GetConfig,
  GetImgUrl,
  SwitchRpc
} from "./js/api";
export default {
  name: "App",
  data() {
    return {
      greenhouse: "",
      config: {},
      menuTopList: [
        {
          icon: "temperature",
          key: "temperature",
          text: "空气温度",
          value: "",
          tip: "℃"
        },
        {
          icon: "humidity",
          key: "humidity",
          text: "空气湿度",
          value: "",
          tip: "%"
        },
        {
          icon: "illumination",
          key: "illumination",
          text: "光照强度",
          value: "",
          tip: "lx"
        },
        {
          icon: "co2",
          key: "co2",
          text: "二氧化碳",
          value: "",
          tip: "%"
        },
        {
          icon: "pressure",
          key: "pressure",
          text: "大气压",
          value: "",
          tip: "kPa"
        }
      ],
      menusCenterList: [
        {
          icon: "temperature",
          key: "soiltemperature",
          text: "土壤温度",
          value: "",
          tip: "℃"
        },
        {
          icon: "humidity",
          key: "soilhumidity",
          text: "土壤湿度",
          value: "",
          tip: "%"
        },
        {
          icon: "ph",
          key: "ph",
          text: "土壤PH值",
          value: ""
        },
        {
          icon: "electrolyte",
          key: "electrolyte",
          text: "电导率",
          value: "",
          tip: "s/m"
        },
        {
          icon: "saltcontent",
          key: "saltcontent",
          text: "含盐量",
          value: "",
          tip: "%"
        }
      ],
      menuBottomList: [
        {
          text: "通风",
          key: "Ventilated",
          isOpen: false
        },
        {
          text: "灌溉",
          key: "Watering",
          isOpen: false
        },
        {
          text: "遮光",
          key: "Shading",
          isOpen: false
        },
        {
          text: "照明",
          key: "Lighting",
          isOpen: false
        },
        {
          text: "加温",
          key: "Wetting",
          isOpen: false
        }
      ],
      greenhouseInfo: {
        index: "",
        name: "",
        id: "",
        type: "",
        plantingType: ""
      },
      insectInfo: {
        title: "病害检测（次）",
        xAxis: [],
        data: [
          {
            data: []
          }
        ]
      },
      policeInfo: {
        title: "闯入报警 (次)",
        xAxis: [],
        image: "./assets/0001.png",
        data: [
          {
            smooth: false,
            symbol: "circle",
            symbolSize: 8,
            itemStyle: {
              color: "#00c8de"
            },
            data: []
          }
        ]
      },
      temperatureInfo: {
        title: "空气/土壤温度 (℃)",
        xAxis: [],
        data: [
          {
            areaStyle: {
              color: "none"
            },
            lineStyle: {
              color: "#e1a254",
              width: 3
            },
            data: []
          },
          {
            areaStyle: {
              color: "none"
            },
            lineStyle: {
              color: "#00afe0",
              width: 3
            },
            data: []
          }
        ]
      },
      humidityInfo: {
        title: "空气/土壤湿度 (%)",
        xAxis: [],
        data: [
          {
            areaStyle: {
              color: "none"
            },
            lineStyle: {
              color: "#e1a254",
              width: 3
            },
            data: []
          },
          {
            areaStyle: {
              color: "none"
            },
            lineStyle: {
              color: "#00afe0",
              width: 3
            },
            data: []
          }
        ]
      },
      IlluminationOnfo: {
        title: "光照 (lx)",
        xAxis: [],
        data: [
          {
            data: []
          }
        ]
      },
      showVideo: true,
      timer: null,
      temperatureTimer: null, // 温度定时器
      humidityTimer: null, // 湿度定时器
      insectTimer: null, // 病虫数
      policeTimer: null, // 闯入报警
      illuminationTimer: null, // 光照
      lognTimer: null,
      urlTimer: null,
      switchRpcTimer: null,
      switchRpcSeleced: "",
      dataInfo: {},
      urlInfo: {
        videoKey: "",
        videoUrl: "",
        picKey: "",
        picUrl: ""
      },
      showEcharts: false
    };
  },
  async created() {
    localStorage.clear();
    this.config = await GetConfig();
    this.greenhouse = this.config.title;
    this.menuTopList.forEach(_m => {
      _m.value = this.config.atmosphereKeys[_m.key];
    });
    this.menusCenterList.forEach(_m => {
      _m.value = this.config.atmosphereKeys[_m.key];
    });
    this.urlInfo.videoUrl = this.config.video_url;
    this.urlInfo.picUrl = this.config.pic_url;
    this.greenhouseInfo.index = this.config.index;
    this.greenhouseInfo.name = this.config.name;
    this.greenhouseInfo.id = this.config.id;
    this.greenhouseInfo.type = this.config.type;
    this.greenhouseInfo.plantingType = this.config.plantingType;
    this.login();
    this.init();
  },
  components: {
    VHeader,
    VBorder,
    VMenu,
    VEcharts,
    VSwitch
  },
  methods: {
    async login(type) {
      let res = await Login(this.config);
      if (res) {
        localStorage.setItem("token", res.token);
        if (type) {
          this.init();
          clearInterval(this.lognTimer);
          this.lognTimer = setInterval(() => {
            this.login();
          }, 600000);
        }
      }
    },
    init() {
      if (!localStorage.getItem("token")) {
        this.lognTimer = setInterval(() => {
          this.login(true);
        }, 3000);
      } else {
        this.getData();
        this.getTemperature();
        this.getHumidity();
        this.getInsect();
        this.getPolice();
        this.getIllumination();
        this.getSwitchRpc();
        this.timer = setInterval(() => {
          this.getData();
        }, this.config.speed);
        this.temperatureTimer = setInterval(() => {
          this.getTemperature();
        }, this.config.speed);
        this.humidityTimer = setInterval(() => {
          this.getHumidity();
        }, this.config.speed);
        this.insectTimer = setInterval(() => {
          this.getInsect();
        }, this.config.speed);
        this.policeTimer = setInterval(() => {
          this.getPolice();
        }, this.config.speed);
        this.illuminationTimer = setInterval(() => {
          this.getIllumination();
        }, this.config.speed);
        this.switchRpcTimer = setInterval(() => {
          this.getSwitchRpc();
        }, this.config.rpcSpeed);
      }
    },
    // 获取温室当前数据
    async getData() {
      const res = await GetDataInfo(this.config);
      Object.keys(res).forEach(_k => {
        this.$set(this.dataInfo, _k, res[_k][0].value);
      });
    },
    // 病虫数
    async getInsect() {
      const end = new Date().getTime();
      const start = end - this.config.time * 60 * 60 * 1000;
      const insectParams = {
        interval: this.config.interval,
        agg: this.config.insect.agg,
        keys: this.config.insect.key,
        startTs: start,
        endTs: end
      };
      let insectRes;
      if (this.config.insect.api) {
        insectRes = await GetTemperature(insectParams, this.config);
      }
      let insectList = [];
      let xAxis = [];
      if (insectRes) {
        insectRes[this.config.insect.key].forEach(_m => {
          insectList.push(_m.value);
          xAxis.push(GetTime(_m.ts));
        });
        this.insectInfo.data[0].data = insectList;
        this.insectInfo.xAxis = xAxis;
        this.$refs.insect.update(this.insectInfo);
      }
    },
    // 警报
    async getPolice() {
      const end = new Date().getTime();
      const start = end - this.config.time * 60 * 60 * 1000;
      const policeParams = {
        interval: this.config.interval,
        agg: this.config.police.agg,
        keys: this.config.police.key,
        startTs: start,
        endTs: end
      };
      let policeRes;
      if (this.config.police.api) {
        policeRes = await GetTemperature(policeParams, this.config);
      }
      let policeList = [];
      let xAxis = [];
      if (policeRes) {
        policeRes[this.config.police.key].forEach(_m => {
          policeList.push(_m.value);
          xAxis.push(GetTime(_m.ts));
        });
        this.policeInfo.data[0].data = policeList;
        this.policeInfo.xAxis = xAxis;
        this.$refs.police.update(this.policeInfo);
      }
    },
    // 光照
    async getIllumination() {
      const end = new Date().getTime();
      const start = end - this.config.time * 60 * 60 * 1000;
      const illuminationParams = {
        interval: this.config.interval,
        agg: this.config.illumination.agg,
        keys: this.config.illumination.key,
        startTs: start,
        endTs: end
      };
      let illuminationRes;
      if (this.config.illumination.api) {
        illuminationRes = await GetTemperature(illuminationParams, this.config);
      }
      let illuminationList = [];
      let xAxis = [];
      if (illuminationRes) {
        illuminationRes[this.config.illumination.key].forEach(_m => {
          illuminationList.push(_m.value);
          xAxis.push(GetTime(_m.ts));
        });
        this.IlluminationOnfo.data[0].data = illuminationList;
        this.IlluminationOnfo.xAxis = xAxis;
        this.$refs.illumination.update(this.IlluminationOnfo);
      }
    },
    // 温度度数据
    async getTemperature() {
      const end = new Date().getTime();
      const start = end - this.config.time * 60 * 60 * 1000;
      // 空气温度
      const temperatureParams = {
        interval: this.config.interval,
        agg: this.config.temperature.agg,
        keys: this.config.temperature.atmosphere,
        startTs: start,
        endTs: end
      };
      // 土壤温度
      const soilParams = {
        interval: this.config.interval,
        agg: this.config.temperature.agg,
        keys: this.config.temperature.soil,
        startTs: start,
        endTs: end
      };
      let temperatureRes, soilRes;
      if (this.config.temperature.atmosphereApi) {
        temperatureRes = await GetTemperature(temperatureParams, this.config);
      }
      if (this.config.temperature.soilApi) {
        soilRes = await GetTemperature(soilParams, this.config);
      }
      let temperatureList = [];
      let soilList = [];
      let xAxis = [];
      if (temperatureRes) {
        temperatureRes[this.config.temperature.atmosphere].forEach(_m => {
          temperatureList.push(_m.value);
          xAxis.push(GetTime(_m.ts));
        });
      }
      if (soilRes) {
        soilRes[this.config.temperature.soil].forEach(_m => {
          soilList.push(_m.value);
        });
      }
      this.temperatureInfo.data[0].data = temperatureList;
      this.temperatureInfo.data[1].data = soilList;
      this.temperatureInfo.xAxis = xAxis;
      this.$refs.temperature.update(this.temperatureInfo);
    },
    // 湿度数据
    async getHumidity() {
      const end = new Date().getTime();
      const start = end - this.config.time * 60 * 60 * 1000;
      // 空气湿度
      const temperatureParams = {
        interval: this.config.interval,
        agg: this.config.humidity.agg,
        keys: this.config.humidity.atmosphere,
        startTs: start,
        endTs: end
      };
      // 土壤湿度
      const soilParams = {
        interval: this.config.interval,
        agg: this.config.humidity.agg,
        keys: this.config.humidity.soil,
        startTs: start,
        endTs: end
      };
      let temperatureRes, soilRes;
      if (this.config.humidity.atmosphereApi) {
        temperatureRes = await GetTemperature(temperatureParams, this.config);
      }
      if (this.config.humidity.soilApi) {
        soilRes = await GetTemperature(soilParams, this.config);
      }
      let temperatureList = [];
      let soilList = [];
      let xAxis = [];
      if (temperatureRes) {
        temperatureRes[this.config.humidity.atmosphere].forEach(_m => {
          temperatureList.push(_m.value);
          xAxis.push(GetTime(_m.ts));
        });
      }
      if (soilRes) {
        soilRes[this.config.humidity.soil].forEach(_m => {
          soilList.push(_m.value);
        });
      }
      this.humidityInfo.data[0].data = temperatureList;
      this.humidityInfo.data[1].data = soilList;
      this.humidityInfo.xAxis = xAxis;
      this.$refs.humidity.update(this.humidityInfo);
    },
    // 开关状态
    async getSwitchRpc() {
      const res = await GetImgUrl(this.config);
      if (res) {
        this.menuBottomList.map(_m => {
          let status = res.find(_r => _r.key === _m.key.toLowerCase());
          if (status && status.key !== this.switchRpcSeleced.toLowerCase()) {
            _m.isOpen = status.value === "True" ? true : false;
          }
        });
        this.switchRpcSeleced = "";
      }
    },
    openCamera() {
      this.showVideo = !this.showVideo;
    },
    async setSwitchRpc(item) {
      item.isOpen = !item.isOpen;
      this.switchRpcSeleced = item.key;
      await SwitchRpc(
        {
          method: `set${item.key}`,
          params: item.isOpen
        },
        this.config
      );
    }
  },
  mounted() {
    this.showEcharts = true;
    let timer = null;
    window.onresize = () => {
      this.showEcharts = false;
      clearTimeout(timer);
      timer = setTimeout(() => {
        this.showEcharts = true;
      }, 500);
    };
  },
  destroyed() {
    clearInterval(this.timer);
    clearInterval(this.temperatureTimer);
    clearInterval(this.humidityTimer);
    clearInterval(this.insectTimer);
    clearInterval(this.policeTimer);
    clearInterval(this.illuminationTimer);
    clearInterval(this.lognTimer);
    clearInterval(this.urlTimer);
    clearInterval(this.switchRpcTimer);
  },
  filters: {
    getValue(value) {
      if (value) {
        return value;
      } else {
        return "0";
      }
    }
  }
};
</script>
<style lang="less">
#app {
  width: 100%;
  height: 100%;
  background: url(./assets/bg.jpg) no-repeat;
  background-size: 100% 100%;
  color: #ffffff;
  .main {
    height: 90%;
    padding: 0.3rem 0.95rem 0.625rem 0.95rem;
    display: flex;
    .main-left,
    .main-right {
      width: 25%;
      min-width: 18.75rem;
      padding: 0 0.625rem;
    }
    .main-left {
      .main-left-top {
        height: 35%;
        margin-bottom: 1.25rem;
      }
      .main-left-bottom {
        height: calc(65% - 1.25rem);
        .police-echart,
        .police-image {
          width: 100%;
          height: 50%;
        }
        .police-image {
          h6 {
            height: 2rem;
            font-size: 1.25rem;
            padding: 0 1.5rem;
            font-weight: 500;
          }
          .image {
            height: calc(100% - 2rem);
            display: flex;
            padding: 0.75rem 1.25rem 1rem 1.25rem;
            justify-content: center;
            align-items: center;
            overflow: hidden;
            img {
              width: 100%;
              height: 100%;
            }
          }
        }
      }
    }
    .main-center {
      width: 50%;
      padding: 0.625rem 0.3rem;
      display: flex;
      flex-flow: column;
      .main-center-top {
        flex: 0 0 6.5rem;
        display: flex;
        .main-center-menu {
          margin: 0.5rem;
          flex: 1;
          display: flex;
          .main-center-menu-icon {
            width: 50%;
            height: 100%;
            display: flex;
            justify-content: center;
            align-items: center;
            .icon {
              width: 3.75rem;
              height: 3.75rem;
              border-radius: 50%;
              i {
                display: inline-block;
                width: 100%;
                height: 100%;
              }
              &.temperature {
                background: #0070ff;
                i {
                  background: url(./assets/temperature.png) no-repeat;
                  background-position: center;
                }
              }
              &.humidity {
                background: #11adc6;
                i {
                  background: url(./assets/humidity.png) no-repeat;
                  background-position: center;
                }
              }
              &.illumination {
                background: #f2ab62;
                i {
                  background: url(./assets/Illumination.png) no-repeat;
                  background-position: center;
                }
              }
              &.co2 {
                background: #d94754;
                i {
                  background: url(./assets/co2.png) no-repeat;
                  background-position: center;
                }
              }
              &.pressure {
                background: #8a215a;
                i {
                  background: url(./assets/pressure.png) no-repeat;
                  background-position: center;
                }
              }
              &.ph {
                background: #f2ab62;
                i {
                  background: url(./assets/ph.png) no-repeat;
                  background-position: center;
                }
              }
              &.electrolyte {
                background: #d94754;
                i {
                  background: url(./assets/electrolyte.png) no-repeat;
                  background-position: center;
                }
              }
              &.saltcontent {
                i {
                  background: url(./assets/tds.png) no-repeat;
                  background-size: 100% 100%;
                  background-position: center;
                }
              }
            }
          }
          .main-center-menu-text {
            width: 50%;
            height: 100%;
            text-align: right;
            padding: 0.6rem 0.6rem 0.6rem 0;
            &.active {
              padding-right: 1rem;
            }
            .text-top {
              span {
                font-size: 2rem;
                &.tip {
                  font-size: 0.75rem;
                }
              }
            }
            .text-bottom {
              font-size: 1rem;
              margin-top: 0.625rem;
            }
          }
        }
      }
      .main-center-center {
        flex: 1;
        position: relative;
        .greenhouse-info {
          width: 18.25rem;
          height: 12rem;
          position: absolute;
          top: 2rem;
          left: 4rem;
          .title {
            width: 100%;
            font-size: 1.4rem;
            font-weight: 600;
            padding: 1rem 1rem 1rem 1.25rem;
            overflow: hidden;
          }
          .text {
            width: 100%;
            font-size: 1.075rem;
            font-weight: 500;
            padding: 0.4rem 0.4rem 0.4rem 1.25rem;
            overflow: hidden;
            .label {
              margin-right: 0.5rem;
            }
          }
        }
        .greenhouse {
          width: 31.25rem;
          height: 18.75rem;
          position: absolute;
          background: url(./assets/greenhouse.png) no-repeat;
          background-position: center;
          background-size: 100%;
          &.greenhouse-left {
            top: 14rem;
            left: 7rem;
            bottom: 4rem;
            z-index: 10;
            > span {
              display: inline-block;
              position: absolute;
              &.camera {
                width: 2.2rem;
                height: 2.2rem;
                top: 5rem;
                left: 14rem;
                cursor: pointer;
                background: url(./assets/camera.png) no-repeat;
                background-position: center;
                background-size: 100%;
              }
              &.line {
                width: 7px;
                height: 2.35rem;
                top: -5px;
                left: 6.25rem;
                position: relative;
                > i {
                  display: inline-block;
                  &.line-spot {
                    width: 7px;
                    height: 7px;
                    position: absolute;
                    top: 0;
                    left: 0;
                    border-radius: 50%;
                    background: #0975fc;
                  }
                  &.line-line {
                    width: 1px;
                    height: calc(100% - 7px);
                    position: absolute;
                    top: 7px;
                    left: 3px;
                    background: #0975fc;
                  }
                }
              }
              &.camera-line {
                width: 3rem;
                height: 7px;
                left: 15.5rem;
                top: 4.25rem;
                position: relative;
                transform: translate(0px, -10px) rotate(-5deg);
                > i {
                  display: inline-block;
                  &.line-spot {
                    width: 7px;
                    height: 7px;
                    position: absolute;
                    top: 0;
                    right: 0;
                    border-radius: 50%;
                    background: #0975fc;
                  }
                  &.line-line {
                    width: calc(100% - 7px);
                    height: 1px;
                    position: absolute;
                    top: 3px;
                    left: 0;
                    background: #0975fc;
                  }
                }
              }
            }
          }
          &.greenhouse-right {
            top: 8rem;
            left: 22rem;
            background-size: 90%;
            z-index: 1;
          }
        }
        .video {
          width: 40rem;
          height: 30rem;
          position: absolute;
          left: 26rem;
          top: 4rem;
          z-index: 100;
          .centent {
            background: rgba(4, 26, 67, 0.8);
          }
          .video-centent {
            width: 100%;
            height: 100%;
            padding: 0.75rem;
          }
        }
      }
      .main-center-bottom {
        flex: 0 0 5rem;
        width: 100%;
        display: flex;
        width: 100%;
        .main-center-bottom-menu {
          flex: 1;
          padding: 0 1rem;
          display: flex;
          justify-content: center;
          align-items: center;
          cursor: pointer;
          .text {
            font-size: 1.25rem;
          }
          .icon {
            display: inline-block;
            width: 0.725rem;
            height: 0.725rem;
            border-radius: 50%;
            margin-left: 1rem;
            background: #d64551;
            border: 1px solid #d64551;
            box-shadow: 0px 0px 5px 0px #d64551;
            &.active {
              background: #14bd9e;
              border: 1px solid #14bd9e;
              box-shadow: 0px 0px 5px 0px #14bd9e;
            }
          }
        }
      }
    }
    .main-right {
      .main-right-top,
      .main-right-center,
      .main-right-bottom {
        height: calc((100% - 1.25rem) / 3);
      }
      .main-right-center {
        margin: 0.625rem 0;
      }
    }
  }
}
</style>
