export const getValue = value => {
  return value < 10 ? "0" + value : value;
};
export const GetTime = date => {
  const d = new Date(date);
  const hour = getValue(d.getHours());
  const min = getValue(d.getMinutes());
  const newTime = hour + ":" + min;
  return newTime;
};
