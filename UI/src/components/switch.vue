<template>
  <div class="switch">
    <div class="switch-backgroud" :class="{ sActive: isOpen }">
      <span class="switch-icon"></span>
    </div>
  </div>
</template>
<script>
export default {
  props: {
    active: Boolean
  },
  computed: {
    isOpen() {
      return this.active;
    }
  }
};
</script>
<style lang="less" scoped>
.switch {
  display: inline-block;
  width: 3.75rem;
  height: 1.8rem;
  border-radius: 1rem;
  overflow: hidden;
  margin: 0 0.5rem;
  cursor: pointer;
  .switch-backgroud {
    width: 100%;
    height: 100%;
    background: #d64551;
    box-shadow: 0px 0px 5px 0px #d64551;
    transition: all 0.3s;
    position: relative;
    .switch-icon {
      display: inline-block;
      width: 1.6rem;
      height: 1.6rem;
      border-radius: 50%;
      background: #ffffff;
      position: absolute;
      transition: all 0.3s;
      z-index: 100;
      top: 0.1rem;
      left: 0;
    }
    &.sActive {
      background: #14bd9e;
      box-shadow: 0px 0px 5px 0px #14bd9e;
      .switch-icon {
        left: 100%;
        margin-left: -1.6rem;
      }
    }
  }
}
</style>
