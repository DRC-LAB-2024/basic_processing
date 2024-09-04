import {
  View,
  Heading,
  Button,
  ButtonText,
  FormControl,
  FormControlLabel,
  Input,
  FormControlLabelText,
  InputField,
  Select,
  SelectTrigger,
  SelectBackdrop,
  SelectInput,
  SelectIcon,
  ChevronDownIcon,
  Icon,
  SelectPortal,
  SelectContent,
  SelectDragIndicatorWrapper,
  SelectDragIndicator,
  SelectItem,
  HStack,
  Spinner,
  ButtonIcon,
  ChevronLeftIcon
} from '@gluestack-ui/themed'
import { useState } from 'react'
import { Alert } from 'react-native'

import { addControlDeviceAsync } from '@/api/device'

// current socket have 2 port , update for more
const PORT = [
  { port: 18, name: 'Cổng kết nối số 1' },
  { port: 19, name: 'Cổng kết nốt số 2' }
]

const AddOutputDevice = ({ route, navigation }) => {
  const { deviceId, controlDevices } = route.params
  // state for control device name and port
  const [controlDeviceName, setControlDeviceName] = useState('')
  const [port, setPort] = useState('')
  // loading state
  const [loading, setLoading] = useState(false)

  const handleSubmit = async () => {
    setLoading(true)
    try {
      if (!controlDeviceName || !port) return
      const formData = {
        device_name: controlDeviceName,
        port,
        signal: 0,
        esp_id: deviceId
      }
      // add control device to database
      const response = await addControlDeviceAsync(formData)
      if (response?.success) {
        navigation.goBack()
      }
    } catch (error) {
      console.log(error)
      Alert.alert(
        'Thao tác thất bại',
        error.response.data.message ?? 'Thất bại'
      )
    } finally {
      setLoading(false)
    }
  }

  return (
    <View flex={1} paddingTop="$10" paddingHorizontal="$5">
      <HStack alignItems="center">
        <Button variant="link" onPress={() => navigation.goBack()}>
          <ButtonIcon color="$black" size="xl" as={ChevronLeftIcon} />
          <ButtonText color="$black">Trở lại</ButtonText>
        </Button>
      </HStack>
      <Heading textAlign="center">Thêm thiết bị</Heading>
      <View marginTop="$4">
        <FormControl marginBottom="$4" size="md">
          <FormControlLabel>
            <FormControlLabelText>Tên thiết bị</FormControlLabelText>
          </FormControlLabel>
          <Input>
            <InputField
              placeholder="Nhập tên thiết bị Ex(: Máy quạt)"
              name="device_name"
              type="text"
              onChangeText={text => setControlDeviceName(text)}
              value={controlDeviceName}
            />
          </Input>
        </FormControl>
        <FormControlLabel mb="$1" mt="$4">
          <FormControlLabelText>Cổng kết nối</FormControlLabelText>
        </FormControlLabel>
        <Select onValueChange={val => setPort(val)}>
          <SelectTrigger variant="outline" size="md">
            <SelectInput placeholder="Chọn cổng kết nối" />
            <SelectIcon mr="$3">
              <Icon as={ChevronDownIcon} />
            </SelectIcon>
          </SelectTrigger>
          <SelectPortal>
            <SelectBackdrop />
            <SelectContent>
              <SelectDragIndicatorWrapper>
                <SelectDragIndicator />
              </SelectDragIndicatorWrapper>
              {PORT.map((port, index) => {
                const isExist = controlDevices.some(
                  device => parseInt(device.port) === port.port
                )
                if (isExist) return null
                return (
                  <SelectItem
                    label={port.name}
                    key={index}
                    value={parseInt(port.port)}
                  />
                )
              })}
            </SelectContent>
          </SelectPortal>
        </Select>
        <HStack alignItems="center" justifyContent="center" space="lg">
          <Button
            borderRadius="$3xl"
            marginTop="$5"
            paddingHorizontal="$10"
            variant="outline"
            borderColor="#2a2a2a"
            isDisabled={loading}
            onPress={() => navigation.goBack()}
          >
            <ButtonText color="$black">Hủy</ButtonText>
          </Button>
          <Button
            borderRadius="$3xl"
            backgroundColor="$blue700"
            paddingHorizontal="$10"
            marginTop="$5"
            onPress={handleSubmit}
            isDisabled={loading}
          >
            {loading ? (
              <Spinner paddingHorizontal="$3" color="#fff" />
            ) : (
              <ButtonText>Thêm</ButtonText>
            )}
          </Button>
        </HStack>
      </View>
    </View>
  )
}
export default AddOutputDevice
