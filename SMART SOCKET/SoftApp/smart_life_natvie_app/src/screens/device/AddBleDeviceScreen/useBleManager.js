import { useEffect, useState } from 'react'
import { useDispatch, useSelector } from 'react-redux'
import requestBluetoothPermission from './requestBluetoothPermission.js'
import { BleManager } from 'react-native-ble-plx'
import { authSelector } from '@/redux/authSlice'
import { Platform } from 'react-native'
import { addSocket } from '@/redux/socketSlice'
import { verifyDeviceAsync, addDeviceAsync } from '@/api/esp'
import { reloadScreenDevices } from '@/redux/appSlice'
import { select } from '../../../../modules/react-native-mysql'

const WIFI_CONNECT_FAILED = '0'

const FILTER_NAME_DEVICE = 'ORC'

const bleManager = new BleManager()

const useBleManager = navigation => {
  const dispatch = useDispatch()
  const [isScanning, setIsScanning] = useState()
  const [devices, setDevices] = useState([])
  const [selectedDevice, setSelectedDevice] = useState(null)
  const [isPairing, setIsPairing] = useState(false)
  const [statusWifi, setStatusWifi] = useState(null)

  const user = useSelector(authSelector.selectUser)

  useEffect(() => {
    if (Platform.OS === 'android') {
      requestBluetoothPermission()
        .then(granted => {
          if (granted) {
            const subscription = bleManager.onStateChange(state => {
              if (state === 'PoweredOn') {
                startScanning()
                subscription.remove()
              } else if (state === 'PoweredOff') {
                Alert.alert(
                  '"App" muốn sử dụng bluetooth của bạn.',
                  'App sử dụng Bluetooth để tìm kiếm thiết bị lân cận.',
                  [
                    ({
                      text: 'Không cho phép',
                      onPress: () => console.log('Cancel Pressed'),
                      style: 'cancel'
                    },
                    {
                      text: 'Cho phép',
                      onPress: () => {
                        bleManager.enable()
                        startScanning()
                      }
                    })
                  ]
                )
                subscription.remove()
              }
            }, true)

            return () => {
              bleManager.destroy()
            }
          }
        })
        .catch(err => console.log(err))
    } else {
      const subscription = bleManager.onStateChange(state => {
        if (state === 'PoweredOn') {
          startScanning()
          subscription.remove()
        }
      }, true)

      return () => {
        bleManager.destroy()
      }
    }
  }, [])
  const startScanning = () => {
    setIsScanning(true)
    bleManager.startDeviceScan(null, null, (error, device) => {
      if (error) {
        console.log(error)
        return
      }
      if (device.name && device.name.includes(FILTER_NAME_DEVICE)) {
        setDevices(prevDevices => {
          if (!prevDevices.find(d => d.id === device.id)) {
            return [...prevDevices, device]
          }
          return prevDevices
        })
      }
    })

    // stop scanning after 5 seconds
    setTimeout(() => {
      setIsScanning(false)
      bleManager.stopDeviceScan()
    }, 5000)
  }
  const connectToDevice = async device => {
    setIsPairing(true)
    try {
      const serviceUUID = '4fafc201-1fb5-459e-8fcc-c5c9c331914b'
      const characteristicUUID = 'beb5483e-36e1-4688-b7f5-ea07361b26a8'

      const connectedDevice = await bleManager.connectToDevice(device.id)
      await connectedDevice.discoverAllServicesAndCharacteristics()
      // function will call when device send data to phone
      connectedDevice.monitorCharacteristicForService(
        serviceUUID,
        characteristicUUID,
        async (error, characteristic) => {
          if (error) {
            console.log('Monitor error', error)
            return
          }
          const responseDevice = atob(characteristic.value)
          if (responseDevice) {
            switch (responseDevice) {
              case WIFI_CONNECT_FAILED:
                setStatusWifi(false)
                break
              default:
                setStatusWifi(true)
                setSelectedDevice(null)
                try {
                  // add device monitor pin value
                  if (responseDevice.includes('ORCpin')) {
                    const recordPins = await select({
                      table: 'PinValue',
                      modify: { PinID: responseDevice, UserID: user?.id }
                    })
                    if (!recordPins.length) {
                      const res = await addDeviceAsync({
                        table: 'PinValue',
                        attrs: ['UserID'],
                        values: { UserID: user?.id },
                        modify: { PinID: responseDevice }
                      })
                      if (res?.success) {
                        dispatch(reloadScreenDevices(true))
                      }
                    }
                    // add device monitor
                  } else if (responseDevice.includes('MONITOR')) {
                    const res = await addDeviceAsync({
                      table: 'iot_monitor',
                      attrs: ['user_id'],
                      values: { user_id: user?.id },
                      modify: { iot_id: responseDevice }
                    })
                    if (res?.success) {
                      dispatch(reloadScreenDevices(true))
                    }
                    // add socket
                  } else {
                    const reqData = {
                      user_id: user.id,
                      esp_name: device.name,
                      pos: 'Chưa xác định'
                    }
                    const isValidDevice =
                      await verifyDeviceAsync(responseDevice)
                    if (isValidDevice) {
                      const res = await addDeviceAsync({
                        table: 'esp32_v2',
                        attrs: ['user_id', 'esp_name', 'pos'],
                        modify: { esp32_id: responseDevice },
                        values: { ...reqData }
                      })
                      if (res.success) {
                        dispatch(
                          addSocket({
                            ...reqData,
                            esp32_id: responseDevice
                          })
                        )
                      }
                    }
                  }
                  navigation.goBack()
                } catch (error) {
                  console.log(error)
                }
                break
            }
          }
        }
      )
      setSelectedDevice(connectedDevice)
      setIsPairing(false)
    } catch (error) {
      console.log('Connection error', error)
    }
  }
  return {
    isScanning,
    devices,
    selectedDevice,
    isPairing,
    statusWifi,
    startScanning,
    connectToDevice,
    setStatusWifi
  }
}

export default useBleManager
