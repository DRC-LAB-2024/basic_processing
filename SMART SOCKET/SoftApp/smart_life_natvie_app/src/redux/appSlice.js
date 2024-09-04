import { createSlice } from '@reduxjs/toolkit'

const appSlice = createSlice({
  name: 'app',
  initialState: {
    online: true,
    dbConnect: null,
    isReloadScreenDevices: false
  },
  reducers: {
    status: (state, action) => {
      state.online = action.payload
    },
    setDbConnect: (state, action) => {
      state.dbConnect = action.payload
    },
    reloadScreenDevices: (state, action) => {
      state.isReloadScreenDevices = action.payload
    }
  }
})

export const appStatusSelector = {
  selectStatusNetwork: state => state.app.online,
  selectStatusDBConnect: state => state.app.dbConnect,
  selectReloadScreenDevices: state => state.app.isReloadScreenDevices
}
export const { status, setDbConnect, reloadScreenDevices } = appSlice.actions
export default appSlice.reducer
