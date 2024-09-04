import { select, update } from '../../modules/react-native-mysql'

export const fetchDeviceAsync = async ({ table, modify }) => {
  try {
    const response = await select({
      table,
      modify
    })
    if (!response?.error) {
      return response
    }
    return []
  } catch (error) {
    console.log(error)
  }
}
export const fetchBatteryLevelGauges = async ({ UserID }) => {
  const response = await select({
    table: 'PinValue',
    modify: { UserID }
  })
  return response
}

export const verifyDeviceAsync = async esp32_id => {
  const response = await select({
    table: 'esp32_v2',
    modify: { esp32_id }
  })
  // device id is invalid
  if (!response?.length) return false
  const [device] = response
  // device have already added by another user
  return device?.user_id ? false : true
}

// export const addDeviceAsync = async (esp_id, formData) => {
//   const response = await update({
//     table: 'esp32_v2',
//     attrs: ['user_id', 'esp_name', 'pos'],
//     modify: { esp32_id: esp_id },
//     values: { ...formData }
//   })
//   return response
// }
export const addDeviceAsync = async ({ table, attrs, modify, values }) => {
  const response = await update({
    table,
    attrs,
    modify,
    values
  })
  return response
}
export const removeEspFromUser = async esp_id => {
  const response = await update({
    table: 'esp32_v2',
    attrs: ['user_id'],
    modify: { esp32_id: esp_id },
    values: { user_id: 0 }
  })
  return response
}
export const updateInforDeviceAsync = async (esp_id, formData) => {
  const attrs = []
  for (let key in formData) attrs.push(key)
  const response = await update({
    table: 'esp32_v2',
    attrs,
    modify: { esp32_id: esp_id },
    values: { ...formData }
  })
  return response
}

//send signal to esp, have change in device
export const deviceChangeAsync = async esp_id => {
  const response = await update({
    table: 'esp32_v2',
    attrs: ['save_status'],
    modify: { esp32_id: esp_id },
    values: { save_status: 1 }
  })
  return response
}
