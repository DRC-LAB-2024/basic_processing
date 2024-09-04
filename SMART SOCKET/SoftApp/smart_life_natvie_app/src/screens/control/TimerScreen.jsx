import {
  View,
  HStack,
  ButtonText,
  Text,
  Button,
  ButtonIcon,
  ChevronLeftIcon,
  Icon,
  ClockIcon,
  VStack,
  AddIcon,
  Modal,
  ModalBackdrop,
  ModalContent,
  ModalBody,
  ModalFooter,
  Menu,
  MenuItem,
  MenuItemLabel,
  CheckIcon,
  ChevronRightIcon,
  Pressable
} from '@gluestack-ui/themed'
import DatePicker from 'react-native-date-picker'
import { useState } from 'react'

import { SwiperListTimer } from '@/components/control'
import Toast from 'react-native-toast-message'
import { updateInfoControlDeviceAsync } from '@/api/device'
import { deviceChangeAsync } from '@/api/esp'
import Spinner from 'react-native-loading-spinner-overlay'
import { formatTime } from '@/utils'

const TimeScreen = ({ route, navigation }) => {
  // show modal to set time on or time off
  const [showModal, setShowModal] = useState(false)
  const { port, esp_id, time_off, time_on, device_name } = route.params
  // handle back to previous screen
  const handleBack = () => {
    navigation.goBack()
  }
  // show list timer of device socket if have time on or time off
  const [listTimer, setListTimer] = useState(() => {
    const list = []
    if (time_off !== 'NULL' && time_off)
      list.push({ time: time_off, type: 'time_off', key: 'time_off' })
    if (time_on !== 'NULL' && time_on)
      list.push({ time: time_on, type: 'time_on', key: 'time_on' })
    return list
  })
  const onClose = () => {
    setShowModal(false)
  }
  return (
    <View flex={1}>
      <View
        flex={1}
        bgColor="$blue700"
        color="$white"
        paddingTop="$10"
        paddingHorizontal="$2"
      >
        <HStack alignItems="center" justifyContent="center" position="relative">
          <Button
            variant="link"
            onPress={handleBack}
            position="absolute"
            left={0}
          >
            <ButtonIcon color="$white" size="xl" as={ChevronLeftIcon} />
          </Button>
          <Text textAlign="center" size="md" color="$white">
            Hẹn Giờ
          </Text>
        </HStack>
        <HStack alignItems="center" justifyContent="center" flex={1}>
          <VStack alignItems="center" justifyContent="center">
            <Icon as={ClockIcon} color="$white" />
            <Text color="$white"> Hẹn Giờ</Text>
          </VStack>
        </HStack>
      </View>
      <View flex={4} position="relative">
        <View
          position="absolute"
          left={0}
          right={0}
          top={-30}
          bottom={10}
          zIndex={333}
          padding="$2"
        >
          <View
            bgColor="$white"
            style={{ elevation: 8 }}
            flex={1}
            shadowColor="$black"
            shadowOpacity={0.5}
            shadowOffset={{ width: -2, height: 4 }}
            borderRadius="$xl"
            justifyContent="space-between"
            paddingHorizontal="$4"
          >
            <VStack flex={1} w="$full">
              {listTimer.length > 0 ? (
                // using swiper to show list timer of device socket , swipe left to right to delete timer
                <SwiperListTimer
                  port={port}
                  esp_id={esp_id}
                  setListTimer={setListTimer}
                  device_name={device_name}
                  list={listTimer}
                />
              ) : (
                <View flex={1} alignItems="center" justifyContent="center">
                  <Text color="$trueGray500">Vui lòng thêm hẹn giờ</Text>
                </View>
              )}
            </VStack>
            <HStack
              justifyContent="center"
              borderTopWidth="$1"
              borderColor="$trueGray500"
            >
              <Button
                variant="solid"
                bgColor="$none"
                w="$full"
                h="$12"
                onPress={() => setShowModal(true)}
              >
                <ButtonIcon as={AddIcon} color="$trueGray700" />
                <ButtonText ml="$1" color="$trueGray700">
                  Thêm hẹn giờ
                </ButtonText>
              </Button>
            </HStack>
          </View>
        </View>
      </View>
      <ModalSetTime
        port={port}
        esp_id={esp_id}
        isOpen={showModal}
        onClose={onClose}
        device_name={device_name}
        setListTimer={setListTimer}
      />
    </View>
  )
}

const ModalSetTime = ({
  isOpen,
  onClose,
  device_name,
  port,
  esp_id,
  setListTimer
}) => {
  const [date, setDate] = useState(new Date())
  const [selectedItem, setSelectedItem] = useState('on')
  const [loading, setLoading] = useState(false)

  const handleUpdateTime = async () => {
    if (!date) return
    const type = selectedItem === 'on' ? 'time_on' : 'time_off'
    const timeFormated = formatTime(date)
    setLoading(true)
    try {
      const req = {
        type,
        port,
        esp_id,
        values: {
          [type]: timeFormated
        }
      }
      const response = await updateInfoControlDeviceAsync(req)
      await deviceChangeAsync(esp_id)
      if (response.success) {
        Toast.show({
          type: 'success',
          text1: 'Thành Công',
          text2: 'Hẹn giờ thiết bị thành công',
          position: 'top'
        })
        setListTimer(prev => {
          const isExist = prev.some(item => item.type === type)
          if (isExist) {
            return prev.map(item => {
              if (item.type === type) {
                return { ...item, time: timeFormated }
              }
              return item
            })
          }
          return [...prev, { time: timeFormated, type }]
        })
        onClose()
      }
    } catch (error) {
      Toast.show({
        type: 'error',
        text1: 'Thất bại',
        text2: 'Hẹn giờ thất bại vui lòng thử lại',
        position: 'top'
      })
      console.log(error)
    } finally {
      setLoading(false)
    }
  }

  return (
    <Modal isOpen={isOpen} onClose={onClose} size="lg">
      <Spinner animation="fade" visible={loading} />
      <ModalBackdrop />
      <ModalContent
        bgColor="$blue700"
        borderRadius="$none"
        borderTopRightRadius="$xl"
        borderTopLeftRadius="$xl"
      >
        <ModalBody>
          <View>
            <DatePicker
              date={date}
              locale="vi-VN"
              mode="datetime"
              is24hourSource="locale"
              title="Hẹn Giờ Thiết Bị"
              theme="dark"
              onDateChange={state => {
                setDate(state)
              }}
            />
          </View>
        </ModalBody>
      </ModalContent>
      <ModalContent
        borderRadius="$none"
        borderBottomRightRadius="$xl"
        borderBottomLeftRadius="$xl"
      >
        <ModalBody>
          <HStack
            paddingVertical="$3"
            paddingHorizontal="$5"
            alignItems="center"
            justifyContent="space-between"
          >
            <Text color="$black">{device_name}</Text>
            <Menu
              placement="bottom end"
              offset={20}
              trigger={({ ...triggerProps }) => {
                return (
                  <Pressable {...triggerProps}>
                    <HStack alignItems="center" space="sm">
                      <Text color="$black">
                        {selectedItem === 'on' ? 'BẬT' : 'TẮT'}
                      </Text>
                      <Icon as={ChevronRightIcon} />
                    </HStack>
                  </Pressable>
                )
              }}
            >
              <MenuItem
                onPress={() => setSelectedItem('on')}
                textValue="Turn on"
              >
                <Icon
                  opacity={selectedItem === 'on' ? '$100' : '$0'}
                  as={CheckIcon}
                  mr="$2"
                />
                <MenuItemLabel>Bật</MenuItemLabel>
              </MenuItem>
              <MenuItem
                textValue="Turn Off"
                onPress={() => setSelectedItem('off')}
              >
                <Icon
                  mr="$2"
                  opacity={selectedItem === 'off' ? '$100' : '$0'}
                  as={CheckIcon}
                />
                <MenuItemLabel>Tắt</MenuItemLabel>
              </MenuItem>
            </Menu>
          </HStack>
          <ModalFooter>
            <Button onPress={onClose} variant="outline" borderRadius="$3xl">
              <ButtonText>Hủy</ButtonText>
            </Button>
            <Button
              onPress={handleUpdateTime}
              borderRadius="$3xl"
              bgColor="$blue700"
              ml="$3"
            >
              <ButtonText>Thêm</ButtonText>
            </Button>
          </ModalFooter>
        </ModalBody>
      </ModalContent>
    </Modal>
  )
}
export default TimeScreen
