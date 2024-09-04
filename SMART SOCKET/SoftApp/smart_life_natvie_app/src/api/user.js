import { select, update } from '../../modules/react-native-mysql'
import { loginStart, loginFailer, loginSuccess } from '@/redux/authSlice'
import AsyncStorage from '@react-native-async-storage/async-storage'

export const login = async ({ username, password, dispatch }) => {
  dispatch(loginStart())
  try {
    const response = await select({
      table: 'users_v2',
      attrs: ['id', 'username'],
      modify: { username, password }
    })
    if (response?.length > 0) {
      dispatch(loginSuccess(response[0]))
      const ONE_MONTH_IN_MS = 30 * 24 * 60 * 60 * 1000
      const expirationTime = new Date().getTime() + ONE_MONTH_IN_MS
      await AsyncStorage.setItem('user', JSON.stringify(response[0]))
      await AsyncStorage.setItem('expirationTime', expirationTime.toString())
      return
    }
    dispatch(loginFailer('Người dùng không tồn tại'))
  } catch (error) {
    console.log(error)
    dispatch(loginFailer('Đăng nhập thất bại'))
  }
}

export const verifyUserAsync = async ({ username, password }) => {
  try {
    const response = await select({
      table: 'users_v2',
      attrs: ['id', 'username'],
      modify: { username, password }
    })
    if (response?.length > 0) {
      return { status: 'ok', data: response[0] }
    }
    return { status: 'error', message: 'Người dùng không tồn tại' }
  } catch (error) {}
}

export const changePasswordAsync = async (userId, password) => {
  const response = await update({
    table: 'users_v2',
    attrs: ['password'],
    modify: { id: userId },
    values: { password }
  })
  return response
}
