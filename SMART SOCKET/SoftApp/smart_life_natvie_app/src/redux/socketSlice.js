import { createSlice } from '@reduxjs/toolkit'

const socketSlice = createSlice({
  name: 'socket',
  initialState: {
    list: {
      sockets: [],
      loading: null
    }
  },
  reducers: {
    fetchListSocketStart: state => {
      state.list.loading = true
    },
    fetchListSocket: (state, action) => {
      state.list.sockets = action.payload
      state.list.loading = false
    },
    updateSocket: (state, action) => {
      const _sockets = state.list.sockets
      const { id, updateData } = action.payload
      state.list.sockets = _sockets.map(socket => {
        if (id === socket.esp32_id) return { ...socket, ...updateData }
        return socket
      })
    },
    addSocket: (state, action) => {
      state.list.sockets = [...state.list.sockets, action.payload]
    }
  }
})

export const {
  fetchListSocketStart,
  fetchListSocket,
  addSocket,
  updateSocket
} = socketSlice.actions

export const socketSelector = {
  selectListSocket: state => state.socket.list.sockets,
  selectListSocketLoading: state => state.socket.list.loading
}

export default socketSlice.reducer
