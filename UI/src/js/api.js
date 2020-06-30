import Axios from "./axios";
export const Login = config => {
  const url = config.ip + "/api/auth/login";
  return Axios({
    url: url,
    method: "POST",
    timeout: 3000,
    data: {
      username: config.username,
      password: config.password
    }
  });
};
export const GetConfig = () => {
  const url = "/config.json";
  return Axios.get(url, {});
};
// 获取当前空气 温度湿度数据
export const GetDataInfo = config => {
  const url = `${config.ip}/api/plugins/telemetry/DEVICE/${config.device_id}/values/timeseries`;
  return Axios({
    url: url,
    method: "GET"
  });
};
// 获取一个时间段的温度数据
export const GetTemperature = (params, config) => {
  const url = `${config.ip}/api/plugins/telemetry/DEVICE/${config.device_id}/values/timeseries`;
  return Axios({
    url: url,
    method: "GET",
    params: params
  });
};
// 获取开关状态
export const GetImgUrl = config => {
  const url = `${config.ip}/api/plugins/telemetry/${config.entityType}/${config.rpc_id}/values/attributes`;
  return Axios({
    url: url,
    method: "GET"
  });
};
/* 按钮RPC控制，参数支持：
[
  {'method': 'getVentilated'}, //获取通风开关状态，初始化调用
  {'method': 'setVentilated', 'params': false}, //设置通风开关状态为关闭，返回结果如果是{'params': false}则认为已关闭，如果是true则是开着的状态。
  
  {'method': 'getWatering'}, //获取灌溉开关状态，初始化调用
  {'method': 'setWatering', 'params': false}, //设置灌溉开关状态为关闭，结果同上

  {'method': 'getShading'}, //获取遮光开关状态，初始化调用
  {'method': 'setShading', 'params': false}, //设置遮光开关状态为关闭，结果同上

  {'method': 'getLighting'}, //获取照明开关状态，初始化调用
  {'method': 'setLighting', 'params': false}, //设置照明开关状态为关闭，结果同上

  {'method': 'getWetting'}, //获取加湿开关状态，初始化调用
  {'method': 'setWetting', 'params': false}, //设置加湿开关状态为关闭，结果同上
]
*/
export const SwitchRpc = (params, config) => {
  const url = `${config.ip}/api/plugins/rpc/twoway/${config.rpc_id}`;
  return Axios({
    url: url,
    method: "POST",
    data: params
  });
};
