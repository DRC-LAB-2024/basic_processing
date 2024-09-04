import { createNativeStackNavigator } from '@react-navigation/native-stack'
import ControlScreen from '@/screens/control/ControlScreen'
import AddOutputDevice from '@/screens/control/AddSubDeviceScreen'

const ControlStack = createNativeStackNavigator()

const ControlStackScreen = () => {
  return (
    <ControlStack.Navigator>
      <ControlStack.Screen
        name="Control"
        options={{ headerShown: false }}
        component={ControlScreen}
      />
      <ControlStack.Screen
        name="AddOutputDevice"
        options={{ headerShown: false }}
        component={AddOutputDevice}
      />
    </ControlStack.Navigator>
  )
}

export default ControlStackScreen
