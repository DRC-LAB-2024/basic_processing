import {
  ScrollView,
  View,
  Heading,
  Button,
  ButtonIcon,
  AddIcon,
  HStack,
  Image,
  Text,
  RefreshControl,
  Menu,
  MenuItem,
  MenuItemLabel
} from '@gluestack-ui/themed'
import { StatusBar } from 'expo-status-bar'
import { Header } from '@/components/shared'
import { DeviceScreenLoadingSkeleton } from '@/components/loading-skeleton'
import {
  DeviceItem,
  PinDeviceItem,
  DeviceMonitorItem
} from '@/components/device'
import useDeviceScreen from './useDeviceScreen'

const DeviceScreen = ({ navigation }) => {
  const {
    batteryLevelGauges,
    sockets,
    monitorDevices,
    isFetchingListSocket,
    refreshing,
    onRefresh
  } = useDeviceScreen()

  return (
    <>
      <Header />
      <View flex={1}>
        <StatusBar style="light" />
        <View paddingTop="$5" flex={1}>
          <HStack justifyContent="space-between" alignItems="center">
            <Heading size="sm" paddingHorizontal="$5" textAlign="center">
              Tất cả thiết bị
            </Heading>
            <Menu
              placement="bottom end"
              trigger={({ ...triggerProps }) => {
                return (
                  <Button
                    {...triggerProps}
                    backgroundColor="#f1f1f1"
                    borderWidth="$1"
                    borderRadius="$2xl"
                    borderColor="$transparent"
                  >
                    <ButtonIcon as={AddIcon} mr="$1" color="$black" />
                  </Button>
                )
              }}
            >
              <MenuItem
                key="edit"
                textValue="edit"
                onPress={() => navigation.navigate('AddSocket')}
              >
                <MenuItemLabel size="sm">Thêm thủ công</MenuItemLabel>
              </MenuItem>
              <MenuItem
                key="time"
                textValue="setTime"
                onPress={() => navigation.navigate('BleScanner')}
              >
                <MenuItemLabel size="sm">Bluetooth</MenuItemLabel>
              </MenuItem>
            </Menu>
          </HStack>
          {isFetchingListSocket ? (
            <DeviceScreenLoadingSkeleton />
          ) : sockets?.length === 0 &&
            batteryLevelGauges.length === 0 &&
            isFetchingListSocket === false ? (
            <View flex={1} alignItems="center">
              <Image
                size="2xl"
                objectFit="scale-down"
                alt="no output device connected"
                source={require('../../../assets/empty-state.png')}
              />
              <Text color="$trueGray500" mt="$2">
                Không có thiết bị kết nối
              </Text>
            </View>
          ) : (
            <ScrollView
              refreshControl={
                <RefreshControl refreshing={refreshing} onRefresh={onRefresh} />
              }
              paddingHorizontal="$5"
              paddingBottom="$6"
              flex={1}
            >
              {sockets &&
                sockets?.map((device, index) => (
                  <DeviceItem device={device} key={index} />
                ))}
              {batteryLevelGauges.map((device, index) => (
                <PinDeviceItem key={index} {...device} />
              ))}
              {monitorDevices.map((device, index) => (
                <DeviceMonitorItem key={index} {...device} />
              ))}
            </ScrollView>
          )}
        </View>
      </View>
    </>
  )
}

export default DeviceScreen
