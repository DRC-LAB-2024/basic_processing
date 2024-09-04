import {
  Box,
  Button,
  HStack,
  MenuItem,
  MenuItemLabel,
  Switch,
  Text,
  Menu
} from '@gluestack-ui/themed'
import { useEffect, useState } from 'react'
import { useNavigation } from '@react-navigation/native'
import Icon from 'react-native-vector-icons/Entypo'
import ModalEditSubDevice from './ModalEditSubDevice'

const ControlDeviceItem = ({
  appState,
  isFocused,
  deviceInfo,
  deviceChanges,
  setDeviceChanges,
  setControlDevices
}) => {
  const { device_name, signal, port, esp_id, time_off, time_on } = deviceInfo
  const navigation = useNavigation()
  const [toggle, setToggle] = useState(parseInt(signal) === 1 ? true : false)
  const [openModal, setOpenModal] = useState(false)

  useEffect(() => {
    if (JSON.stringify(deviceChanges) === '{}') return
    setToggle(parseInt(signal) === 1 ? true : false)
    setDeviceChanges({})
  }, [appState, isFocused])

  const handleToggle = port => {
    setDeviceChanges(pre => ({
      ...pre,
      [port]: { esp_id, signal: !toggle ? 1 : 0 }
    }))
    setToggle(!toggle)
  }
  const onClose = () => {
    setOpenModal(false)
  }

  return (
    <HStack
      flex={1}
      justifyContent="center"
      paddingHorizontal="$2"
      alignItems="center"
      marginVertical="$2"
    >
      <Text
        fontWeight="$semibold"
        size="sm"
        flex={2}
        flexWrap="wrap"
        color="#2a2a2a"
      >
        {device_name}
      </Text>
      <Box flex={3} alignItems="center" justifyContent="center">
        <HStack space="md" justifyContent="center" paddingRight="$5">
          <Text lineHeight="$xl">Off</Text>
          <Switch
            value={toggle}
            onToggle={() => handleToggle(port)}
            size="md"
          />
          <Text lineHeight="$xl">On</Text>
        </HStack>
      </Box>
      <Menu
        placement="bottom end"
        offset={-40}
        trigger={({ ...triggerProps }) => {
          return (
            <Button {...triggerProps} variant="link" borderRadius="$full">
              <Icon name="dots-three-horizontal" size={20} />
            </Button>
          )
        }}
      >
        <MenuItem
          key="edit"
          textValue="edit"
          onPress={() => setOpenModal(true)}
        >
          <MenuItemLabel size="sm">Đổi tên</MenuItemLabel>
        </MenuItem>
        <MenuItem
          key="time"
          textValue="setTime"
          onPress={() =>
            navigation.navigate('Time', {
              device_name,
              port,
              esp_id,
              time_off,
              time_on
            })
          }
        >
          <MenuItemLabel size="sm">Hẹn Giờ</MenuItemLabel>
        </MenuItem>
      </Menu>
      <ModalEditSubDevice
        isOpen={openModal}
        onClose={onClose}
        port={port}
        esp_id={esp_id}
        device_name={device_name}
        setControlDevices={setControlDevices}
      />
    </HStack>
  )
}

export default ControlDeviceItem
