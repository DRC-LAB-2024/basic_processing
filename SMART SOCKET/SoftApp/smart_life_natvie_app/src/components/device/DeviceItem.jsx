import {
  Heading,
  HStack,
  Image,
  View,
  Text,
  Button,
  EditIcon,
  ButtonIcon
} from '@gluestack-ui/themed'
import { useNavigation } from '@react-navigation/native'

const DeviceItem = ({ device }) => {
  const navigation = useNavigation()
  const { esp32_id, esp_name, pos } = device
  const handleEdit = () => {
    navigation.navigate('EditSocket', {
      isEdit: true,
      esp32_id,
      esp_name,
      pos
    })
  }
  return (
    <HStack
      marginVertical="$2"
      paddingHorizontal="$3"
      borderRadius="$lg"
      backgroundColor="$white"
      alignItems="flex-start"
    >
      <HStack alignItems="center">
        <Image
          width="$20"
          alt="socket"
          size="xl"
          style={{ objectFit: 'contain' }}
          source={require('../../assets/devices/socket.png')}
        />
        <View>
          <Heading size="sm">{esp_name}</Heading>
          <Text mt="$2">
            <Text size="sm">Vị trí : {pos ? pos : 'Chưa xác định'}</Text>
          </Text>
        </View>
      </HStack>
      <Button
        style={{ marginLeft: 'auto' }}
        variant="link"
        onPress={handleEdit}
      >
        <ButtonIcon color="$black" as={EditIcon} />
      </Button>
    </HStack>
  )
}

export default DeviceItem
