import { View, Heading, ScrollView, Image } from '@gluestack-ui/themed'
import { StatusBar } from 'expo-status-bar'
import { useState, useEffect } from 'react'
import { useSelector } from 'react-redux'
import { useIsFocused } from '@react-navigation/native'
import { AppState } from 'react-native'

import { Header } from '@/components/shared'
import { socketSelector } from '@/redux/socketSlice'
import { appStatusSelector } from '@/redux/appSlice'
import { ControlSection } from '@/components/control'

const ControlScreen = ({ navigation }) => {
  // store current app state to check if app is in backggroun or foreground
  const [appState, setAppState] = useState()
  const sockets = useSelector(socketSelector.selectListSocket)
  const isFocused = useIsFocused()
  const isOnline = useSelector(appStatusSelector.selectStatusNetwork)

  useEffect(() => {
    // listen to app state change
    const subscription = AppState.addEventListener('change', nextAppState => {
      setAppState(nextAppState)
    })
    // clean up function
    return () => {
      subscription.remove()
    }
  }, [])

  return (
    <>
      <Header />
      <StatusBar style="light" />
      <View flex={1}>
        <View paddingTop="$5">
          <Heading
            size="sm"
            color="#2a2a2a"
            paddingHorizontal="$5"
            textAlign="left"
          >
            Điều khiển thiết bị
          </Heading>
        </View>
        {sockets?.length > 0 ? (
          <ScrollView flex={1} paddingHorizontal="$5">
            {sockets.map(esp => (
              <ControlSection
                appState={appState}
                isFocused={isFocused}
                key={esp.esp32_id}
                esp={esp}
                isOnline={isOnline}
                navigation={navigation}
              />
            ))}
          </ScrollView>
        ) : (
          <View mt="$2" alignItems="center">
            <Image
              size="2xl"
              objectFit="scale-down"
              alt="no output device connected"
              source={require('../../assets/empty-state.png')}
            />
          </View>
        )}
      </View>
    </>
  )
}

export default ControlScreen
