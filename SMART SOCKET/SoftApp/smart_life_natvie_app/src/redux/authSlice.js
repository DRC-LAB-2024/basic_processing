import { createSlice } from '@reduxjs/toolkit'

const authSlice = createSlice({
  name: 'auth',
  initialState: {
    loading: false,
    user: null,
    isAuthenticated: null,
    message: null,
    isError: false
  },
  reducers: {
    loginStart: state => {
      state.loading = true
    },
    loginSuccess: (state, action) => {
      state.loading = false
      state.user = action.payload
      state.isAuthenticated = true
    },
    loginFailer: (state, action) => {
      state.loading = false
      state.message = action.payload
      state.isError = true
    },
    logOut: state => {
      state.message = null
      state.isAuthenticated = null
      state.isError = false
      state.loading = false
      state.user = null
    }
  }
})

export const authSelector = {
  selectIsAuthenticated: state => state.auth.isAuthenticated,
  selectUser: state => state.auth.user,
  selectLoading: state => state.auth.loading,
  selectMessage: state => state.auth.message,
  selectIsError: state => state.auth.isError
}
export const { loginStart, loginSuccess, loginFailer, logOut } =
  authSlice.actions

export default authSlice.reducer
