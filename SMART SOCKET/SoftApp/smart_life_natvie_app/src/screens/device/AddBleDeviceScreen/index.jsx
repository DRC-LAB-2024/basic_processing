import React, { useState } from 'react'
import {
  View,
  HStack,
  VStack,
  Heading,
  Spinner,
  Text,
  Image,
  Button,
  ButtonText,
  Input,
  InputIcon,
  InputField,
  InputSlot,
  ChevronLeftIcon,
  ButtonIcon
} from '@gluestack-ui/themed'
import { FlatList, Modal } from 'react-native'
import Icon from 'react-native-vector-icons/AntDesign'
import useBleManager from './useBleManager.js'
import { Header } from '@/components/shared'
import { StatusBar } from 'expo-status-bar'
import SpinnerOverlay from 'react-native-loading-spinner-overlay'
import base64 from 'react-native-base64'

const AddBleDeviceScreen = ({ navigation }) => {
  const {
    isScanning,
    devices,
    selectedDevice,
    isPairing,
    statusWifi,
    startScanning,
    connectToDevice,
    setStatusWifi
  } = useBleManager(navigation)

  return (
    <>
      <Header />
      <StatusBar />
      <View flex={1}>
        <HStack alignItems="center" justifyContent="center" position="relative">
          <Button
            position="absolute"
            top={10}
            left={20}
            variant="link"
            onPress={() => navigation.goBack()}
          >
            <ButtonIcon color="$black" size="xl" as={ChevronLeftIcon} />
          </Button>
          <Heading
            paddingTop="$5"
            size="sm"
            paddingHorizontal="$5"
            textAlign="left"
          >
            Thêm thiết bị
          </Heading>
        </HStack>
        <View paddingTop="$5" paddingHorizontal="$5">
          {isScanning && (
            <HStack
              bgColor="$white"
              borderRadius="$xl"
              space="sm"
              paddingVertical="$5"
              paddingHorizontal="$3"
              marginBottom="$2"
            >
              <Spinner />
              <Text size="sm">Đang tìm kiếm thiết bị lân cận...</Text>
            </HStack>
          )}
          {isScanning === false && devices.length === 0 && (
            <VStack alignItems="center" space="xl">
              <Text textAlign="center" size="sm" color="$trueGray500">
                Không tìm thấy thiết bị để ghép nối {'\n'} Hãy chắc chắc thiết
                bị của bạn đang bật bluetooth.
              </Text>
              <Button
                borderRadius="$3xl"
                w="$32"
                bgColor="$blue700"
                onPress={startScanning}
              >
                <ButtonText>Thử lại</ButtonText>
              </Button>
            </VStack>
          )}
          {devices.length > 0 && (
            <FlatList
              data={devices}
              keyExtractor={item => item.id}
              renderItem={({ item }) => (
                <HStack
                  marginVertical="$2"
                  borderRadius="$xl"
                  bgColor="$white"
                  alignItems="center"
                  paddingVertical="$2"
                  paddingHorizontal="$2"
                >
                  <Image
                    alt="socket"
                    size="md"
                    style={{
                      objectFit: 'contain'
                    }}
                    //TODO: add new image for monitor
                    source={
                      item.name.includes('ORC Pin')
                        ? require('../../../assets/devices/pin.png')
                        : item.name.includes('Monitor')
                          ? require('../../../assets/devices/moniter.png')
                          : require('../../../assets/devices/socket.png')
                    }
                  />
                  <Text> {item.name || 'Unknown'}</Text>
                  <Button
                    bgColor="$blue700"
                    borderRadius="$3xl"
                    size="sm"
                    onPress={() => connectToDevice(item)}
                    style={{ marginLeft: 'auto' }}
                  >
                    <ButtonText>Thêm</ButtonText>
                  </Button>
                  <SpinnerOverlay animation="fade" visible={isPairing} />
                </HStack>
              )}
            />
          )}
          {selectedDevice && (
            <ModalSendCredentials
              statusWifi={statusWifi}
              setStatusWifi={setStatusWifi}
              selectedDevice={selectedDevice}
              isOpen={selectedDevice !== null}
            />
          )}
        </View>
      </View>
    </>
  )
}

const ModalSendCredentials = ({
  isOpen,
  selectedDevice,
  statusWifi,
  setStatusWifi
}) => {
  const [ssid, setSsid] = useState('')
  const [password, setPassword] = useState('')
  const [isSending, setIsSending] = useState(false)

  const sendCredentials = async () => {
    if (!selectedDevice) {
      console.log('No device connected')
      return
    }
    setIsSending(true)
    const serviceUUID = '4fafc201-1fb5-459e-8fcc-c5c9c331914b'
    const characteristicUUID = 'beb5483e-36e1-4688-b7f5-ea07361b26a8'

    try {
      const encoder = new TextEncoder()
      const data = encoder.encode(`${ssid},${password}`)
      const base64Data = base64.encode(String.fromCharCode(...data))
      const isConnected = await selectedDevice.isConnected()

      if (isConnected) {
        await selectedDevice.writeCharacteristicWithResponseForService(
          serviceUUID,
          characteristicUUID,
          base64Data
        )
        setIsSending(false)
      }
    } catch (error) {
      setIsSending(false)
      setStatusWifi(false)
      console.log('Error sending credentials', error)
    }
  }

  return (
    <Modal animationType="slide" transparent={true} visible={isOpen}>
      <View flex={1} style={{ backgroundColor: 'rgba(0,0,0,0.5)' }} />
      <View style={{ backgroundColor: 'rgba(0,0,0,0.5)' }}>
        <View
          bgColor="$white"
          borderTopLeftRadius="$2xl"
          borderTopRightRadius="$2xl"
          paddingHorizontal="$10"
          paddingBottom="$16"
        >
          <Heading size="xl" paddingTop="$16">
            Vui lòng nhập thông tin Wifi
          </Heading>
          <Text marginTop="$2" color="$trueGray500">
            Chọn Wi-Fi và nhập mật khẩu
          </Text>
          <VStack paddingVertical="$5" space="xl">
            <Input variant="underlined" on>
              <InputSlot mr="$3">
                <InputIcon as={Icon} name="wifi" color="$trueGray500" />
              </InputSlot>
              <InputField
                placeholder="Tên Wi-Fi"
                value={ssid}
                type="text"
                onChangeText={val => setSsid(val)}
              />
            </Input>
            <Input variant="underlined">
              <InputSlot mr="$3">
                <InputIcon as={Icon} name="lock" color="$trueGray500" />
              </InputSlot>
              <InputField
                onChangeText={val => setPassword(val)}
                type="text"
                placeholder="Mật khẩu Wifi"
                value={password}
              />
            </Input>
            {statusWifi === false && (
              <Text color="$red500">
                Kết nối wifi thất bại vui lòng thử lại!!
              </Text>
            )}
            <View paddingTop="$5">
              <Button
                onPress={sendCredentials}
                borderRadius="$xl"
                bgColor="$blue700"
                isDisabled={isSending}
              >
                {isSending ? <Spinner /> : <ButtonText>Hoàn thành</ButtonText>}
              </Button>
            </View>
          </VStack>
        </View>
      </View>
    </Modal>
  )
}

export default AddBleDeviceScreen
