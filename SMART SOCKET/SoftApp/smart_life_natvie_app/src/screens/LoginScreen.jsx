import {
  Heading,
  Spinner,
  VStack,
  Button,
  Text,
  Input,
  InputField,
  ButtonText,
  FormControl,
  FormControlError,
  FormControlErrorText,
  Image
} from '@gluestack-ui/themed'
import { useState } from 'react'
import { StatusBar } from 'expo-status-bar'
import { useSelector, useDispatch } from 'react-redux'

import { login } from '@/api/user'
import { authSelector } from '@/redux/authSlice'

const LoginScreen = () => {
  const dispatch = useDispatch()
  // get loading, message, isError from redux store
  const loading = useSelector(authSelector.selectLoading)
  const message = useSelector(authSelector.selectMessage)
  const isError = useSelector(authSelector.selectIsError)

  const [username, setUsername] = useState('')
  const [password, setPassword] = useState('')

  const handleSubmit = async () => {
    if (!username || !password) return
    await login({ username, password, dispatch })
  }

  return (
    <VStack
      space="xl"
      flex={1}
      alignItems="center"
      justifyContent="center"
      paddingHorizontal="$5"
    >
      <StatusBar style="dark" />
      <Image
        alt="logo company"
        size="xl"
        source={require('../assets/logo.png')}
      />
      <Heading bold size="2xl" color="$blue700">
        Login
      </Heading>
      <Text
        mb="$5"
        fontSize="$lg"
        fontWeight="bold"
        color="$black"
        textAlign="center"
      >
        Welcome back you've {'\n'} been missed!
      </Text>
      <VStack w="$full" space="md">
        <FormControl isInvalid={isError}>
          <Input
            variant="outline"
            size="xl"
            backgroundColor="$primary0"
            borderWidth="$0"
          >
            <InputField
              onChangeText={text => setUsername(text)}
              value={username}
              placeholder="Username"
              type="text"
            />
          </Input>
          <Input
            marginTop="$5"
            size="xl"
            backgroundColor="$primary0"
            borderWidth="$0"
          >
            <InputField
              value={password}
              onChangeText={text => setPassword(text)}
              borderRadius="$xl"
              paddingVertical="$2"
              fontSize="$xl"
              placeholder="Password"
              type="password"
            />
          </Input>
          <FormControlError>
            <FormControlErrorText marginLeft="$2">
              {message}
            </FormControlErrorText>
          </FormControlError>
        </FormControl>
      </VStack>
      <Button
        onPress={handleSubmit}
        isDisabled={loading}
        marginTop="$5"
        shadowColor="$blue700"
        borderRadius="$md"
        shadowOpacity="0.2"
        size="xl"
        shadowOffset={{ width: 1, height: 13 }}
        backgroundColor="$blue700"
        w="$full"
      >
        {loading ? <Spinner /> : <ButtonText>Login</ButtonText>}
      </Button>
    </VStack>
  )
}

export default LoginScreen
