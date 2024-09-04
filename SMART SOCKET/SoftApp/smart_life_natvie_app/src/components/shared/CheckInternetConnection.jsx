import { useFocusEffect } from '@react-navigation/native'
import NetInfo from '@react-native-community/netinfo'
import { useDispatch, useSelector } from 'react-redux'
import { appStatusSelector } from '@/redux/appSlice'
import { useCallback } from 'react'
import { View, Text, Button, ButtonText } from '@gluestack-ui/themed'
import { Modal, Platform, Linking } from 'react-native'

import { status } from '@/redux/appSlice'

const CheckInternetConnection = () => {
  const dispatch = useDispatch()
  const isOnline = useSelector(appStatusSelector.selectStatusNetwork)
  useFocusEffect(
    useCallback(() => {
      const netInfoSubscription = NetInfo.addEventListener(state => {
        dispatch(status(state.isConnected))
      })
      return () => {
        netInfoSubscription()
      }
    }, [])
  )

  const openSettingWifi = () => {
    if (Platform.OS === 'android') {
      Linking.openSettings()
    } else if (Platform.OS === 'ios') {
      Linking.openURL('App-Prefs:root=WIFI')
    }
  }

  return (
    <Modal
      visible={!isOnline}
      animation="fade"
      transparent
      statusBarTranslucent={true}
    >
      <View
        flex={1}
        position="absolute"
        top={0}
        right={0}
        bottom={0}
        left={0}
        style={{ backgroundColor: 'rgba(0,0,0,0.5)' }}
        justifyContent="center"
        paddingHorizontal="$5"
      >
        <View
          bgColor="$white"
          borderRadius="$xl"
          h="$32"
          alignItems="center"
          justifyContent="center"
        >
          <Text textAlign="center">
            Thiết bị ngoại tuyến. Vui lòng kiểm tra lại đường truyển
          </Text>
          <Button
            onPress={openSettingWifi}
            bgColor="$black"
            borderRadius="$3xl"
            w="$32"
            mt="$2"
          >
            <ButtonText color="$white">Cài đặt</ButtonText>
          </Button>
        </View>
      </View>
    </Modal>
  )
}

export default CheckInternetConnection
