import { select, insert, update } from '../../modules/react-native-mysql'

export const fetchControlDeviceAsync = async esp_id => {
  const response = await select({
    table: 'control_v2_clone',
    modify: { esp_id }
  })
  return response
}

export const addControlDeviceAsync = async formData => {
  const response = await insert({
    table: 'control_v2_clone',
    attrs: ['esp_id', 'signal', 'port', 'device_name'],
    values: { ...formData }
  })
  return response
}
export const updateInfoControlDeviceAsync = async req => {
  let attrs = ['device_name']
  if (req?.type === 'time_on' || req?.type === 'time_off') attrs = [req.type]
  const response = await update({
    table: 'control_v2_clone',
    attrs,
    modify: { port: req.port, esp_id: req.esp_id },
    values: { ...req.values }
  })
  return response
}

export const updateStatusControlDeviceAsync = async ({
  port,
  esp_id,
  signal
}) => {
  const response = await update({
    table: 'control_v2_clone',
    attrs: ['signal'],
    modify: { port, esp_id },
    values: { signal }
  })
  return response
}

export const addNewBatteryLevelGauge = async ({ UserID, PinID }) => {
  const response = await update({
    table: 'PinValue',
    attrs: ['UserID'],
    values: { UserID },
    modify: { PinID }
  })
  return response
}

export const fetchCurrentStatusOfPin = async ({ PinID }) => {
  const response = await select({
    table: 'PinValue',
    attrs: ['Voltage', 'UpdateTime'],
    modify: { PinID }
  })
  if (response?.length > 0) return response[0]
}

export const fetchCurrentParameterOfMonitorDevice = async ({ iot_id }) => {
  const response = await select({
    table: 'iot_monitor',
    attrs: [
      'analog_1',
      'analog_2',
      'digital_1',
      'digital_2',
      'humidity',
      'temperature'
    ],
    modify: { iot_id }
  })
  if (response?.length > 0) return response[0]
}
