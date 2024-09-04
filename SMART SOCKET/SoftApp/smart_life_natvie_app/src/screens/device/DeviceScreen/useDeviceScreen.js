import { useEffect, useState, useCallback } from 'react'
import { useSelector, useDispatch } from 'react-redux'
import { authSelector } from '@/redux/authSlice'
import {
  socketSelector,
  fetchListSocket,
  fetchListSocketStart
} from '@/redux/socketSlice'
import { appStatusSelector, reloadScreenDevices } from '@/redux/appSlice'
import { fetchDeviceAsync } from '@/api/esp'

const useDeviceScreen = () => {
  const dispatch = useDispatch()
  // user from redux store
  const user = useSelector(authSelector.selectUser)
  // check network status
  const isOnline = useSelector(appStatusSelector.selectStatusNetwork)
  // store pin device
  const [batteryLevelGauges, setBatteryLevelGauges] = useState([])
  // store monitor device
  const [monitorDevices, setMonitorDevices] = useState([])
  // store smart socket
  const sockets = useSelector(socketSelector.selectListSocket)
  // loading state from redux store
  const isFetchingListSocket = useSelector(
    socketSelector.selectListSocketLoading
  )
  // trigger reload screen after add new device
  const isReloadScreen = useSelector(
    appStatusSelector.selectReloadScreenDevices
  )
  // pull to refresh state
  const [refreshing, setRefreshing] = useState(false)
  //fetch data from database
  const loadData = useCallback(
    async type => {
      if (type === 'init') dispatch(fetchListSocketStart())
      try {
        if (isOnline) {
          const batteryLevelGaugesDB = await fetchDeviceAsync({
            table: 'PinValue',
            modify: { UserID: user?.id }
          })
          if (!batteryLevelGaugesDB?.error) {
            setBatteryLevelGauges(batteryLevelGaugesDB)
          }

          const monitorDevicesDB = await fetchDeviceAsync({
            table: 'iot_monitor',
            modify: { user_id: user?.id }
          })
          if (!monitorDevicesDB?.error) {
            setMonitorDevices(monitorDevicesDB)
          }

          const socketsDB = await fetchDeviceAsync({
            table: 'esp32_v2',
            modify: { user_id: user?.id }
          })

          if (!socketsDB?.error) {
            dispatch(fetchListSocket(socketsDB))
          }
        }
      } catch (error) {
        console.error(error)
      }
    },
    [isOnline]
  )

  // pull to refresh : get latest data from server
  const onRefresh = useCallback(async () => {
    setRefreshing(true)
    try {
      if (!isOnline) return
      loadData('refresh')
    } catch (error) {
      console.log(error)
    } finally {
      setRefreshing(false)
    }
  }, [])

  useEffect(() => {
    if (isReloadScreen) {
      loadData('refresh')
      dispatch(reloadScreenDevices(null))
    } else if (isReloadScreen === false) {
      loadData('init')
    }
  }, [loadData, isReloadScreen])

  return {
    batteryLevelGauges,
    isFetchingListSocket,
    sockets,
    refreshing,
    onRefresh,
    monitorDevices
  }
}

export default useDeviceScreen
