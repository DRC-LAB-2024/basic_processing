import {
  View,
  HStack,
  Button,
  Text,
  ButtonText,
  Image,
  Spinner
} from '@gluestack-ui/themed'
import { useState, useEffect } from 'react'
import { deviceChangeAsync } from '@/api/esp'
import {
  fetchControlDeviceAsync,
  updateStatusControlDeviceAsync
} from '@/api/device'
import ControlDeviceItem from './ControlDeviceItem'
import Toast from 'react-native-toast-message'

const ControlSection = ({ esp, navigation, isOnline, isFocused, appState }) => {
  const { esp32_id: deviceId, esp_name } = esp
  // store change of control device
  const [deviceChanges, setDeviceChanges] = useState({})
  // store control device list
  const [controlDevices, setControlDevices] = useState([])

  const [loadingStates, setLoadingStates] = useState({
    dataLoading: false,
    updateLoading: false
  })

  const toastify = isSucess => {
    Toast.show({
      type: isSucess ? 'success' : 'error',
      text1: isSucess ? 'Thành Công' : 'Thất bại',
      text2: isSucess ? 'Lưu thành công' : 'Lưu thất bại ? Hãy thử lại',
      position: 'top',
      bottomOffset: 100
    })
  }
  // load control device from database via device id
  const loadControlDevices = async () => {
    setLoadingStates(pre => ({ ...pre, dataLoading: true }))
    try {
      const response = await fetchControlDeviceAsync(deviceId)
      if (!response?.error) setControlDevices(response)
    } catch (error) {
      console.log(error)
    } finally {
      setLoadingStates(pre => ({ ...pre, dataLoading: false }))
    }
  }
  useEffect(() => {
    if (deviceId) loadControlDevices()
  }, [deviceId, isFocused, isOnline])

  const handleSave = async () => {
    // if no change return
    if (!Object.keys(deviceChanges).length) return
    setLoadingStates(pre => ({ ...pre, updateLoading: true }))
    try {
      const updateList = []
      for (const key in deviceChanges) {
        const { esp_id, signal } = deviceChanges[key]
        updateList.push(
          updateStatusControlDeviceAsync({ port: key, esp_id, signal })
        )
      }
      await Promise.all(updateList)
      await deviceChangeAsync(deviceId)
      setDeviceChanges({})
      toastify(true)
    } catch (error) {
      console.log(error)
    } finally {
      setLoadingStates(pre => ({ ...pre, updateLoading: false }))
    }
  }
  return (
    <View borderRadius="$md" marginVertical="$2" padding="$2" bgColor="$white">
      <HStack alignItems="center" justifyContent="space-between">
        <Text color="#2a2a2a" bold textAlign="center">
          Nguồn : <Text color="blue">{esp_name}</Text>
        </Text>
        {controlDevices?.length < 2 && (
          <Button
            variant="link"
            onPress={() =>
              navigation.navigate('AddOutputDevice', {
                controlDevices,
                deviceId
              })
            }
          >
            <ButtonText>Thêm</ButtonText>
          </Button>
        )}
      </HStack>
      {controlDevices?.length === 0 && loadingStates.dataLoading === false ? (
        <View flex={1} alignItems="center">
          <Image
            size="sm"
            alt="no output device connected"
            source={require('../../assets/empty-state.png')}
          />
          <Text>Không có thiết bị kết nối</Text>
        </View>
      ) : (
        <>
          <View marginTop="$1">
            {controlDevices?.map((device, index) => (
              <ControlDeviceItem
                appState={appState}
                deviceChanges={deviceChanges}
                isFocused={isFocused}
                setDeviceChanges={setDeviceChanges}
                setControlDevices={setControlDevices}
                key={index}
                deviceInfo={device}
              />
            ))}
          </View>
          <HStack justifyContent="center">
            <Button
              onPress={handleSave}
              bgColor="$blue700"
              borderRadius="$3xl"
              paddingHorizontal="$10"
              marginTop="$2"
              isDisabled={loadingStates.updateLoading}
            >
              {loadingStates.updateLoading ? (
                <Spinner />
              ) : (
                <ButtonText>Lưu</ButtonText>
              )}
            </Button>
          </HStack>
        </>
      )}
    </View>
  )
}
export default ControlSection
