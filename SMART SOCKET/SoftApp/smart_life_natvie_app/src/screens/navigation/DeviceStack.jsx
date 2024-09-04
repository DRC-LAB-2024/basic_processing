import { createNativeStackNavigator } from '@react-navigation/native-stack'
import {
  DeviceScreen,
  AddManualDeviceScreen,
  EditDeviceScreen
} from '@/screens/device'
import AddBleDeviceScreen from '@/screens/device/AddBleDeviceScreen/index.jsx'

const DeviceStack = createNativeStackNavigator()

const DeviceStacksScreen = () => {
  return (
    <DeviceStack.Navigator>
      <DeviceStack.Screen
        name="Device"
        options={{ headerShown: false }}
        component={DeviceScreen}
      />
      <DeviceStack.Screen
        name="AddSocket"
        options={{ headerShown: false }}
        component={AddManualDeviceScreen}
      />
      <DeviceStack.Screen
        name="EditSocket"
        options={{ headerShown: false }}
        component={EditDeviceScreen}
      />
      <DeviceStack.Screen
        name="BleScanner"
        options={{ headerShown: false }}
        component={AddBleDeviceScreen}
      />
    </DeviceStack.Navigator>
  )
}
export default DeviceStacksScreen
