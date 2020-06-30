<template>
  <div class="menu">
    <slot></slot>
  </div>
</template>
<script>
export default {
  name: "VMenu"
};
</script>

<style lang="less" scoped>
.menu {
  background: rgba(4, 26, 67, 0.4);
  border-top: 1px solid transparent;
  border-bottom: 1px solid transparent;
  border-image: linear-gradient(
      to right,
      rgba(4, 26, 67, 0.4),
      #23b7cb,
      rgba(4, 26, 67, 0.4)
    )
    1 1;
}
</style>
