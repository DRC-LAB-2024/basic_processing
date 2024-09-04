import { HStack, Image, View, Heading, Text } from '@gluestack-ui/themed'
import { useState, useEffect } from 'react'
import { fetchCurrentParameterOfMonitorDevice } from '../../api/device'

const DeviceMonitorItem = props => {
  const {
    iot_id,
    analog_1,
    analog_2,
    digital_1,
    digital_2,
    humidity,
    temperature
  } = props

  const [deviceParameter, setDeviceParameter] = useState({
    analog_1,
    analog_2,
    digital_1,
    digital_2,
    humidity,
    temperature
  })

  useEffect(() => {
    const loadDeviceParameter = async () => {
      const response = await fetchCurrentParameterOfMonitorDevice({ iot_id })
      if (response) setDeviceParameter(response)
    }
    const intervalId = setInterval(() => {
      loadDeviceParameter()
    }, 2000)
    return () => {
      clearInterval(intervalId)
    }
  }, [])

  return (
    <HStack
      marginVertical="$2"
      paddingHorizontal="$3"
      borderRadius="$lg"
      backgroundColor="$white"
      alignItems="center"
    >
      <HStack alignItems="center">
        <Image
          width="$20"
          alt="pin"
          size="xl"
          style={{ objectFit: 'contain' }}
          source={require('../../assets/devices/moniter.png')}
        />
        <View>
          <Heading size="sm">{iot_id}</Heading>
          <Text bold>{deviceParameter?.temperature} °C</Text>
          <Text size="sm">độ ẩm : {deviceParameter.humidity}%</Text>
          <HStack flex={1} justifyContent="space-around">
            <View>
              <Text size="sm">analog_1 : {deviceParameter.analog_1}</Text>
              <Text size="sm">analog_2 : {deviceParameter.analog_2}</Text>
            </View>
            <View ml="$5">
              <Text size="sm">digital_1 : {deviceParameter.digital_1}</Text>
              <Text size="sm">digital_2 : {deviceParameter.digital_2}</Text>
            </View>
          </HStack>
        </View>
      </HStack>
    </HStack>
  )
}

export default DeviceMonitorItem
