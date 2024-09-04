import { configureStore } from '@reduxjs/toolkit'
import socketReducer from './socketSlice'
import authReducer from './authSlice'
import appReducer from './appSlice'

export const store = configureStore({
  reducer: {
    socket: socketReducer,
    auth: authReducer,
    app: appReducer
  }
})
