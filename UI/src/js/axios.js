import axios from "axios";
axios.interceptors.request.use(config => {
  const token = localStorage.getItem("token");
  if (token) {
    config.headers["X-Authorization"] = "Bearer " + token;
  }
  return config;
});
axios.interceptors.response.use(res => {
  if (res.status === 200) {
    return res.data;
  }
});
export default axios;
