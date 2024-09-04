import { createBottomTabNavigator } from '@react-navigation/bottom-tabs'
import Icon from 'react-native-vector-icons/Ionicons'
import DeviceStacksScreen from './DeviceStack'
import ProfileScreen from '../ProfileScreen'
import ControlStackScreen from './ControlStack'

const Tab = createBottomTabNavigator()

const MainTabNavigator = () => {
  return (
    <Tab.Navigator
      screenOptions={({ route }) => ({
        tabBarHideOnKeyboard: true,
        tabBarStyle: {
          borderTopRightRadius: 20,
          borderTopLeftRadius: 20
        },
        tabBarIcon: ({ focused, color, size }) => {
          let iconName
          if (route.name === 'DeviceStack') {
            iconName = focused ? 'home' : 'home-outline'
          } else if (route.name === 'ControlStack') {
            iconName = focused ? 'settings' : 'settings-outline'
          } else if (route.name === 'Monitor') {
            iconName = focused ? 'stats-chart' : 'stats-chart-outline'
          } else if (route.name === 'Signal') {
            iconName = focused ? 'barcode' : 'barcode-outline'
          } else if (route.name === 'Profile') {
            iconName = focused ? 'person' : 'person-outline'
          }
          return <Icon name={iconName} size={size} color={color} />
        },
        tabBarActiveTintColor: 'blue'
      })}
    >
      <Tab.Screen
        options={{ headerShown: false, title: 'Thiết bị' }}
        name="DeviceStack"
        component={DeviceStacksScreen}
      />
      <Tab.Screen
        name="ControlStack"
        component={ControlStackScreen}
        options={{ headerShown: false, title: 'Điều khiển' }}
      />
      <Tab.Screen
        name="Profile"
        component={ProfileScreen}
        options={{ headerShown: false, title: 'Tài khoản' }}
      />
    </Tab.Navigator>
  )
}

export default MainTabNavigator
