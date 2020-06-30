<template>
  <div class="echarts" ref="echarts"></div>
</template>
<script>
import * as echarts from "echarts/lib/echarts";
import "echarts/lib/chart/line";
// 引入提示框组件、标题组件、工具箱组件。
import "echarts/lib/component/tooltip";
import "echarts/lib/component/title";
import "echarts/lib/component/toolbox";
export default {
  name: "VEchart",
  props: {
    option: Object
  },
  data() {
    return {
      config: {
        title: {
          text: `{text|${this.option.title}}`,
          textStyle: {
            rich: {
              text: {
                color: "#ffffff",
                fontWeight: "500",
                fontSize: 16,
                width: "100%",
                height: "50",
                padding: [0, 10, 0, 20]
              }
            }
          }
        },
        // 提示框
        tooltip: {
          show: false
        },
        grid: {
          left: 50,
          right: 30,
          bottom: 40
        },
        xAxis: {
          type: "category",
          axisTick: {
            show: false
          },
          axisLabel: {
            color: "#fff",
            fontSize: 14,
            padding: [8, 0],
            align: "center"
          },
          axisLine: {
            lineStyle: {
              color: "#233252"
            }
          },
          boundaryGap: false,
          data: []
        },
        yAxis: {
          type: "value",
          axisTick: {
            show: false
          },
          boundaryGap: false,
          splitNumber: 5,
          minInterval: 1,
          axisLine: {
            show: false
          },
          splitLine: {
            lineStyle: {
              color: "#233252"
            }
          },
          axisLabel: {
            color: "#fff",
            fontSize: 14,
            padding: [0, 5]
          }
        }
      },
      seriesOption: {
        type: "line",
        lineStyle: {
          color: "#00afe0",
          width: 3
        },
        symbol: "none",
        smooth: true,
        areaStyle: {
          color: "rgba(8, 26, 71, 0.8)"
        }
      },
      titleFontSize: 16,
      echartsData: [],
      echarts: null
    };
  },
  methods: {
    init() {
      this.echarts = echarts.init(this.$refs.echarts);
      this.config.title.textStyle.rich.text.fontSize = this.titleFontSize;
      this.echarts.setOption(this.config);
    },
    update(option) {
      this.config.xAxis.data = [];
      option.data = option.data.map(_d => {
        return Object.assign({}, this.seriesOption, _d);
      });
      this.config.xAxis.data = option.xAxis;
      this.config.series = option.data;
      this.echarts.setOption(this.config);
    },
    getFontSize() {
      const screenWidth = window.screen.width;
      if (screenWidth >= 1900) {
        this.titleFontSize = 18;
      } else if (screenWidth >= 1700) {
        this.titleFontSize = 16;
      } else if (screenWidth >= 1400) {
        this.titleFontSize = 14;
      } else {
        this.titleFontSize = 13;
      }
      this.init();
    }
  },
  mounted() {
    this.getFontSize();
  }
};
</script>
<style lang="less" scoped>
.echarts {
  width: 100%;
  height: 100%;
}
</style>
