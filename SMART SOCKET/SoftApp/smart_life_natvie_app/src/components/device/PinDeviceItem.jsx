import { Heading, HStack, Image, View, Text } from '@gluestack-ui/themed'
import { useState, useEffect } from 'react'
import { fetchCurrentStatusOfPin } from '@/api/device'

const DELAY_TIME_TO_FETCH_STATUS = 2000

const PinDeviceItem = props => {
  const { PinID, Voltage, UpdateTime } = props
  const [currentStatusOfPin, setCurrentStatusOfPin] = useState({
    Voltage,
    UpdateTime
  })

  useEffect(() => {
    const fetchStatus = async () => {
      try {
        const response = await fetchCurrentStatusOfPin({ PinID })
        if (response) setCurrentStatusOfPin(response)
      } catch (error) {
        console.log(error)
      }
    }
    const intervalId = setInterval(fetchStatus, DELAY_TIME_TO_FETCH_STATUS)
    return () => clearInterval(intervalId)
  }, [PinID])

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
          alt="pin"
          size="xl"
          style={{ objectFit: 'contain' }}
          source={require('../../assets/devices/pin.png')}
        />
        <View>
          <Heading size="sm">{PinID}</Heading>
          <Text>{currentStatusOfPin?.Voltage}</Text>
          <Text mt="$2">
            <Text size="sm">{currentStatusOfPin?.UpdateTime}</Text>
          </Text>
        </View>
      </HStack>
    </HStack>
  )
}

export default PinDeviceItem
