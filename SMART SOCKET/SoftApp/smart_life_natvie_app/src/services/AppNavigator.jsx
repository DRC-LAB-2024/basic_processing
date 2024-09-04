import { View } from 'react-native'
import { createNativeStackNavigator } from '@react-navigation/native-stack'
import { useEffect, useState } from 'react'
import LoginScreen from '@/screens/LoginScreen'
import MainTabNavigator from '@/screens/navigation/MainTabNavigator'
import AsyncStorage from '@react-native-async-storage/async-storage'
import TimeScreen from '@/screens/control/TimerScreen'
import { Image } from '@gluestack-ui/themed'
import { useSelector, useDispatch } from 'react-redux'
import { authSelector, loginSuccess, logOut } from '@/redux/authSlice'
import {
  connectToDatabase,
  disconnectToDatabase
} from '../../modules/react-native-mysql'

const Stack = createNativeStackNavigator()

const AppNavigator = () => {
  const [isAuthenticating, setIsAuthenticating] = useState(true)
  const dispatch = useDispatch()
  const isAuthenticate = useSelector(authSelector.selectIsAuthenticated)
  useEffect(() => {
    const checkLoginStatus = async () => {
      setIsAuthenticating(true)
      try {
        const storedExpirationTime =
          await AsyncStorage.getItem('expirationTime')
        const storedUserInfo = await AsyncStorage.getItem('user')
        if (storedExpirationTime && storedUserInfo) {
          const currentTime = new Date().getTime()
          if (currentTime < parseInt(storedExpirationTime, 10)) {
            dispatch(loginSuccess(JSON.parse(storedUserInfo)))
          } else {
            dispatch(logOut())
            await AsyncStorage.removeItem('expirationTime')
            await AsyncStorage.removeItem('user')
          }
        }
      } catch (error) {
        console.log(error)
      } finally {
        setIsAuthenticating(false)
      }
    }
    checkLoginStatus()
  }, [])

  useEffect(() => {
    const establishConnectionToDb = async () => {
      try {
        const res = await connectToDatabase()
        if (!res?.success) {
          // retry to connect to db
          await connectToDatabase()
        }
      } catch (error) {
        console.error(error)
      }
    }
    establishConnectionToDb()
    return () => {
      disconnectToDatabase()
    }
  }, [])

  if (isAuthenticating)
    return (
      <View style={{ flex: 1, justifyContent: 'center', alignItems: 'center' }}>
        <Image source={require('../assets/logo.png')} size="2xl" alt="logo" />
      </View>
    )

  return (
    <Stack.Navigator>
      {isAuthenticate ? (
        <>
          <Stack.Screen
            options={{ headerShown: false }}
            component={MainTabNavigator}
            name="home"
          />
          <Stack.Screen
            name="Time"
            component={TimeScreen}
            options={{ headerShown: false }}
          />
        </>
      ) : (
        <Stack.Screen
          options={{ headerShown: false }}
          name="Login"
          component={LoginScreen}
        />
      )}
    </Stack.Navigator>
  )
}

export default AppNavigator
