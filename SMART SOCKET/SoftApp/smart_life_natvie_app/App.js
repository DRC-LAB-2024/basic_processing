import { GluestackUIProvider } from '@gluestack-ui/themed'
import { config } from '@gluestack-ui/config'
import { NavigationContainer } from '@react-navigation/native'
import Toast from 'react-native-toast-message'
import { createContext } from 'react'
import { Provider } from 'react-redux'
import { store } from '@/redux/store'
import { CheckInternetConnection } from '@/components/shared'
import AppNavigator from '@/services/AppNavigator'

export const AppConnectContext = createContext()

export default function App() {
  return (
    <GluestackUIProvider config={config}>
      <Provider store={store}>
        <NavigationContainer>
          <AppNavigator />
          <CheckInternetConnection />
          <Toast />
        </NavigationContainer>
      </Provider>
    </GluestackUIProvider>
  )
}
