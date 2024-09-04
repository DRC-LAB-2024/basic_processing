import {
  Box,
  Heading,
  Text,
  HStack,
  Avatar,
  AvatarImage
} from '@gluestack-ui/themed'

const Header = () => {
  return (
    <HStack
      paddingVertical="$12"
      paddingHorizontal="$5"
      alignItems="center"
      justifyContent="space-between"
      backgroundColor="$blue700"
      borderBottomRightRadius="$3xl"
    >
      <Box>
        <Text color="$white">Hello,</Text>
        <Heading color="$white">Smart Life</Heading>
      </Box>
      <Avatar>
        <AvatarImage
          backgroundColor="$white"
          source={require('../../assets/logo.png')}
          alt="this invidual picture"
        />
      </Avatar>
    </HStack>
  )
}

export default Header
