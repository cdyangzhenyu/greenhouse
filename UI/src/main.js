import Vue from "vue";
import App from "./App.vue";
import store from "./store";
import filters from "./js/filter";
import "./css/index.less";
Vue.config.productionTip = false;
Object.keys(filters).map(_f => {
  Vue.filter(_f, filters[_f]);
});
new Vue({
  store,
  render: h => h(App)
}).$mount("#app");
