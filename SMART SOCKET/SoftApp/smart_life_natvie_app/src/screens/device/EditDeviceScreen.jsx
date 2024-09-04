import {
  View,
  HStack,
  ChevronLeftIcon,
  Button,
  ButtonIcon,
  ButtonText,
  FormControl,
  FormControlLabel,
  Input,
  FormControlLabelText,
  InputField,
  Spinner,
  Heading
} from '@gluestack-ui/themed'
import { useState } from 'react'
import { Alert } from 'react-native'
import { StatusBar } from 'expo-status-bar'
import { useDispatch } from 'react-redux'

import { updateSocket } from '@/redux/socketSlice'
import { updateInforDeviceAsync } from '@/api/esp'

const EditDeviceScreen = ({ route, navigation }) => {
  const dispatch = useDispatch()
  const [loading, setLoading] = useState(false)
  const [editedDeviceName, setEditedDeviceName] = useState('')
  const [editedPositionDevice, setEditedPositionDevice] = useState('')
  const {
    esp32_id: deviceId,
    esp_name: deviceName,
    pos: devicePosition
  } = route.params

  const handleBack = () => {
    navigation.goBack()
  }
  const handleSubmit = async () => {
    setLoading(true)
    try {
      if (deviceId && (editedDeviceName || editedPositionDevice)) {
        const updateData = {}
        if (editedDeviceName) updateData.esp_name = editedDeviceName
        if (editedPositionDevice) updateData.pos = editedPositionDevice
        const res = await updateInforDeviceAsync(deviceId, updateData)
        if (res.success) {
          dispatch(updateSocket({ id: deviceId, updateData }))
          navigation.goBack()
        } else {
          Alert.alert('Thất bại', 'Vui lòng thử lại')
        }
      }
    } catch (error) {
      console.log(error)
    } finally {
      setLoading(false)
    }
  }
  return (
    <View flex={1} paddingTop="$10" paddingHorizontal="$5">
      <StatusBar style="dark" />
      <HStack alignItems="centrer">
        <Button variant="link" onPress={handleBack}>
          <ButtonIcon color="$black" size="xl" as={ChevronLeftIcon} />
          <ButtonText color="$black">Trở lại</ButtonText>
        </Button>
      </HStack>
      <Heading textAlign="center">Cập nhật thiết bị nguồn</Heading>
      <View marginTop="$4">
        <FormControl
          marginBottom="$2"
          size="md"
          isDisabled={false}
          isInvalid={false}
          isReadOnly={false}
          isRequired={false}
        >
          <FormControlLabel mb="$1">
            <FormControlLabelText>Tên gợi nhớ</FormControlLabelText>
          </FormControlLabel>
          <Input>
            <InputField
              onChangeText={text => setEditedDeviceName(text)}
              type="text"
              placeholder="Nhập tên thiết bị"
              name="editedDeviceName"
              defaultValue={deviceName}
            />
          </Input>
        </FormControl>
        <FormControl marginBottom="$4" size="md">
          <FormControlLabel>
            <FormControlLabelText>Vị trí đặt thiết bị</FormControlLabelText>
          </FormControlLabel>
          <Input>
            <InputField
              placeholder="Vị trí đặt thiết bị (Ex: phòng khách)"
              onChangeText={text => setEditedPositionDevice(text)}
              type="text"
              name="editedPositionDevice"
              defaultValue={devicePosition}
            />
          </Input>
        </FormControl>
        <Button
          backgroundColor="$blue700"
          onPress={handleSubmit}
          marginTop="$5"
          isDisabled={loading}
        >
          {loading ? <Spinner /> : <ButtonText>Lưu</ButtonText>}
        </Button>
        {/* <Button variant="link" marginTop="$5" onPress={handleRemoveSocket}> */}
        {/*   <ButtonText color="$red500">Gỡ Thiết Bị</ButtonText> */}
        {/* </Button> */}
      </View>
    </View>
  )
}
export default EditDeviceScreen
