import { useState } from 'react'
import {
  Modal,
  ModalBackdrop,
  ModalContent,
  ModalHeader,
  ModalCloseButton,
  ModalBody,
  ModalFooter,
  CloseIcon,
  Icon as GlueIcon,
  FormControl,
  Input,
  InputField,
  Spinner,
  Heading,
  FormControlLabel,
  FormControlLabelText,
  Button,
  ButtonText
} from '@gluestack-ui/themed'
import { updateInfoControlDeviceAsync } from '@/api/device'
import Toast from 'react-native-toast-message'

const ModalEditSubDevice = ({
  port,
  isOpen,
  esp_id,
  onClose,
  device_name,
  setControlDevices
}) => {
  const [nameDevice, setNameDevice] = useState(device_name)
  const [updateLoading, setUpdateLoading] = useState(false)
  const handleSave = async () => {
    if (nameDevice === device_name) return
    setUpdateLoading(true)
    try {
      const req = {
        port,
        esp_id,
        values: {
          device_name: nameDevice
        }
      }
      const response = await updateInfoControlDeviceAsync(req)
      if (response?.success) {
        Toast.show({
          type: 'success',
          text1: 'Thành Công',
          text2: 'Cập nhật thành công',
          position: 'top'
        })
        onClose()
        setControlDevices(pre => {
          const newDevs = pre.slice()

          const editDevicePos = newDevs.findIndex(
            dev => dev.esp_id === esp_id && dev.port === port
          )
          if (editDevicePos !== -1) {
            newDevs[editDevicePos] = {
              ...newDevs[editDevicePos],
              device_name: nameDevice
            }
          }
          return newDevs
        })
      }
    } catch (error) {
      Toast.show({
        type: 'error',
        text1: 'Thất bại',
        text2: 'Cập nhật thất bại vui lòng thử lại',
        position: 'top'
      })
      console.log(error)
    } finally {
      setUpdateLoading(false)
    }
  }
  return (
    <Modal isOpen={isOpen} onClose={onClose} useRNModal>
      <ModalBackdrop />
      <ModalContent>
        <ModalHeader>
          <Heading size="lg">Đổi tên thiết bị</Heading>
          <ModalCloseButton>
            <GlueIcon as={CloseIcon} />
          </ModalCloseButton>
        </ModalHeader>
        <ModalBody>
          <FormControl marginBottom="$4" size="md">
            <FormControlLabel>
              <FormControlLabelText>Tên thiết bị</FormControlLabelText>
            </FormControlLabel>
            <Input>
              <InputField
                placeholder="Nhập tên thiết bị Ex(: Máy quạt)"
                name="device_name"
                type="text"
                onChangeText={text => setNameDevice(text)}
                value={nameDevice}
              />
            </Input>
          </FormControl>
        </ModalBody>
        <ModalFooter>
          <Button
            variant="outline"
            size="sm"
            action="secondary"
            mr="$3"
            paddingHorizontal="$5"
            borderRadius="$3xl"
            onPress={onClose}
          >
            <ButtonText>Hủy</ButtonText>
          </Button>
          <Button
            onPress={handleSave}
            borderRadius="$3xl"
            size="sm"
            paddingHorizontal="$5"
            action="positive"
            borderWidth="$0"
            bgColor="$blue700"
            isDisable={updateLoading}
          >
            {updateLoading ? (
              <Spinner paddingHorizontal="$1" />
            ) : (
              <ButtonText>Lưu</ButtonText>
            )}
          </Button>
        </ModalFooter>
      </ModalContent>
    </Modal>
  )
}

export default ModalEditSubDevice
