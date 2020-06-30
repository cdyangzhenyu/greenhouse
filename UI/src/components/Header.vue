<template>
  <div class="header">
    <div class="left"></div>
    <div class="center">
      <div class="center-top"></div>
      <span class="center-text">{{ name }}</span>
      <div class="center-bottom">
        <span class="center-left"><i></i></span>
        <span class="center-center">
          <i class="center-center-border border-left"></i>
          <i class="center-center-border border-right"></i>
        </span>
        <span class="center-right"><i></i></span>
      </div>
    </div>
    <div class="right">
      <span>{{ time | getTime(true) }}</span>
    </div>
  </div>
</template>

<script>
export default {
  name: "VHeader",
  props: ["name"],
  data() {
    return {
      time: new Date()
    };
  },
  created() {
    this.getTime();
  },
  methods: {
    getTime() {
      setInterval(() => {
        this.time = new Date();
      }, 1000);
    }
  }
};
</script>

<style lang="less" scoped>
.header {
  height: 10%;
  min-height: 70px;
  display: flex;
  .left,
  .right {
    width: 35%;
    height: calc(100% - 20px);
    padding: 0 70px;
    display: flex;
    align-items: center;
    justify-content: flex-end;
    border-bottom: 2px solid #013f8c;
    background: rgba(4, 26, 67, 0.4);
    font-size: 0.9rem;
  }
  .center {
    width: 30%;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    position: relative;
    font-size: 2rem;
    font-weight: 500;
    .center-top {
      width: 100%;
      height: calc(100% - 22px);
      position: absolute;
      top: 0;
      left: 0;
      background: rgba(4, 26, 67, 0.4);
      z-index: 1;
    }
    .center-text {
      z-index: 10;
    }
    .center-center {
      .center-center-border {
        display: inline-block;
        width: 100px;
        height: 100%;
        border-bottom: 2px solid #016de8;
        position: absolute;
        bottom: -1px;
        &.border-left {
          left: 0;
        }
        &.border-right {
          right: 0;
        }
      }
    }
    .center-bottom {
      position: absolute;
      width: 100%;
      height: 22px;
      left: 0;
      bottom: 0;
      > span {
        display: inline-block;
        height: 20px;
        position: absolute;
      }
      .center-left,
      .center-right {
        width: 25px;
        overflow: hidden;
        i {
          display: inline-block;
          width: 34px;
          height: 20px;
          position: absolute;
          top: 0;
          left: 0;
          border-bottom: 2px solid #013f8c;
          background: rgba(4, 26, 67, 0.4);
        }
      }
      .center-left {
        left: 0;
        top: 0;
        i {
          transform: translate(0px, -8px) rotate(38deg);
        }
      }
      .center-right {
        top: 0;
        right: 0;
        i {
          transform: translate(-9px, -8px) rotate(-38deg);
        }
      }
      .center-center {
        width: calc(100% - 50px);
        left: 25px;
        top: 0;
        border-bottom: 2px solid #013f8c;
        background: rgba(4, 26, 67, 0.4);
        .center-center-border {
          position: absolute;
          display: inline-block;
        }
      }
    }
  }
}
</style>
