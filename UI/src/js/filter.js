import { getValue } from "./utils";
const GetTime = (date, type) => {
  if (!date) return;
  let d = new Date(date);
  let year = d.getFullYear();
  let month = getValue(d.getMonth() + 1);
  let day = getValue(d.getDate());
  let hour = getValue(d.getHours());
  let min = getValue(d.getMinutes());
  let sec = getValue(d.getSeconds());
  let newDate = year + "年" + month + "月" + day + "日";
  let newTime = newDate + " " + hour + ":" + min + ":" + sec;
  return type ? newTime : newDate;
};
export default {
  getTime: GetTime
};
