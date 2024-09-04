import {
  View,
  Heading,
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
  Spinner
} from '@gluestack-ui/themed'
import { useState } from 'react'
import { Alert } from 'react-native'
import { addDeviceAsync, verifyDeviceAsync } from '@/api/esp'
import { StatusBar } from 'expo-status-bar'
import { useSelector, useDispatch } from 'react-redux'
import { authSelector } from '@/redux/authSlice'
import { addSocket } from '@/redux/socketSlice'

const AddManualDeviceScreen = ({ navigation }) => {
  const [addLoading, setAddLoading] = useState(false)
  const [deviceId, setDeviceId] = useState('')
  const [deviceName, setDeviceName] = useState('')
  const [devicePosition, setDevicePosition] = useState('')

  const userLogin = useSelector(authSelector.selectUser)

  const dispatch = useDispatch()

  const handleBack = () => {
    navigation.goBack()
  }

  const handleSubmit = async () => {
    setAddLoading(true)
    try {
      if (!deviceId || !deviceName || !devicePosition) return
      // check if device is valid or not then add to database
      const isValid = await verifyDeviceAsync(deviceId)
      if (!isValid) {
        Alert.alert('Thiết bị không tồn tại')
        return
      }
      const formData = {
        user_id: userLogin.id,
        esp_name: deviceName,
        pos: devicePosition
      }
      const res = await addDeviceAsync({
        table: 'esp32_v2',
        attrs: ['user_id', 'esp_name', 'pos'],
        modify: { esp32_id: deviceId },
        values: { ...formData }
      })
      if (res.success) {
        // add socket to redux store
        dispatch(addSocket({ ...formData, esp32_id: deviceId }))
        navigation.goBack()
      } else {
        Alert.alert(res.data.message ?? 'Thất bại')
      }
    } catch (err) {
      console.log(err)
    } finally {
      setAddLoading(false)
    }
  }
  return (
    <View flex={1} paddingTop="$10" paddingHorizontal="$5">
      <StatusBar style="dark" />
      <HStack alignItems="center">
        <Button variant="link" onPress={handleBack}>
          <ButtonIcon color="$black" size="xl" as={ChevronLeftIcon} />
          <ButtonText color="$black">Trở lại</ButtonText>
        </Button>
      </HStack>
      <Heading textAlign="center">Thêm thiết bị nguồn</Heading>
      <View marginTop="$4">
        <FormControl marginBottom="$4" size="md">
          <FormControlLabel>
            <FormControlLabelText>Serial Number</FormControlLabelText>
          </FormControlLabel>
          <Input>
            <InputField
              placeholder="Nhập IP của thiết bị"
              onChangeText={text => setDeviceId(text)}
              name="deviceId"
              value={deviceId}
              type="text"
            />
          </Input>
        </FormControl>
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
              onChangeText={text => setDeviceName(text)}
              value={deviceName}
              type="text"
              placeholder="Nhập tên thiết bị (Ex : Ổ cắm 1)"
              name="deviceName"
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
              onChangeText={text => setDevicePosition(text)}
              value={devicePosition}
              type="text"
              name="devicePosition"
            />
          </Input>
        </FormControl>
        <Button
          backgroundColor="$blue700"
          onPress={handleSubmit}
          marginTop="$5"
          isDisabled={addLoading}
        >
          {addLoading ? <Spinner /> : <ButtonText>Thêm</ButtonText>}
        </Button>
      </View>
    </View>
  )
}
export default AddManualDeviceScreen
