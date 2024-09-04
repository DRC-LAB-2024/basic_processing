import { View, HStack } from '@gluestack-ui/themed'
import { StatusBar } from 'expo-status-bar'

export const DeviceScreenLoadingSkeleton = () => {
  return (
    <View flex={1} paddingHorizontal="$5">
      <StatusBar style="light" />
      <HStack
        marginVertical="$2"
        h="$32"
        padding="$5"
        borderRadius="$lg"
        style={{ backgroundColor: '#CCCCCC' }}
      />
      <HStack
        marginVertical="$2"
        h="$32"
        padding="$5"
        borderRadius="$lg"
        style={{ backgroundColor: '#CCCCCC' }}
      />
    </View>
  )
}
