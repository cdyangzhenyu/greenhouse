<template>
  <div class="border">
    <span class="icon top-left"></span>
    <span class="icon top-right"></span>
    <span class="icon bottom-left"></span>
    <span class="icon bottom-right"></span>
    <div class="centent">
      <slot></slot>
    </div>
  </div>
</template>
<script>
export default {
  name: "VBorder"
};
</script>
<style lang="less" scoped>
.border {
  position: relative;
  .icon {
    display: inline-block;
    width: 20px;
    height: 20px;
    position: absolute;
    &.top-left {
      top: 0;
      left: 0;
      border-top: 2px solid #016de8;
      border-left: 2px solid #016de8;
    }
    &.top-right {
      top: 0;
      right: 0;
      border-top: 2px solid #016de8;
      border-right: 2px solid #016de8;
    }
    &.bottom-left {
      bottom: 0;
      left: 0;
      border-bottom: 2px solid #016de8;
      border-left: 2px solid #016de8;
    }
    &.bottom-right {
      bottom: 0;
      right: 0;
      border-bottom: 2px solid #016de8;
      border-right: 2px solid #016de8;
    }
  }
  .centent {
    position: absolute;
    top: 1px;
    left: 1px;
    right: 1px;
    bottom: 1px;
    margin: auto;
    background: rgba(4, 26, 67, 0.4);
  }
}
</style>
