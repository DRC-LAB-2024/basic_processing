import {
  HStack,
  Heading,
  Text,
  View,
  VStack,
  Image,
  Box,
  Accordion,
  AccordionItem,
  AccordionHeader,
  AccordionTrigger,
  AccordionContent,
  ChevronUpIcon,
  FormControl,
  FormControlLabel,
  FormControlLabelText,
  InputField,
  Button,
  ButtonText,
  Input,
  AccordionIcon,
  ChevronDownIcon,
  Spinner
} from '@gluestack-ui/themed'
import Icon from 'react-native-vector-icons/MaterialCommunityIcons'
import { useState } from 'react'
import { StatusBar } from 'expo-status-bar'
import { Alert, TouchableWithoutFeedback } from 'react-native'
import AsyncStorage from '@react-native-async-storage/async-storage'
import { useDispatch, useSelector } from 'react-redux'

import { verifyUserAsync, changePasswordAsync } from '@/api/user'
import { authSelector, logOut } from '@/redux/authSlice'

const ProfileScreen = () => {
  const [loadingSignOut, setLoadingSignOut] = useState(false)
  const dispatch = useDispatch()
  const user = useSelector(authSelector.selectUser)
  const handleLogout = async () => {
    setLoadingSignOut(true)
    try {
      await new Promise(resolve => setTimeout(resolve, 500))
      await AsyncStorage.removeItem('user')
      await AsyncStorage.removeItem('expirationTime')
      await AsyncStorage.removeItem('sockets')
      dispatch(logOut())
    } catch (error) {
    } finally {
      setLoadingSignOut(false)
    }
  }
  return (
    <View flex={1} paddingTop="$10" position="relative">
      <StatusBar style="light" />
      <HStack alignItems="centrer" paddingHorizontal="$5">
        <Heading flex={1} textAlign="left" size="sm" color="$white">
          Trang cài đặt
        </Heading>
      </HStack>
      <VStack>
        <View paddingTop="$10" alignItems="center">
          <Image
            bgColor="$white"
            borderRadius="$full"
            alt="logo company"
            size="xl"
            source={require('../assets/logo.png')}
          />
        </View>
        <Heading textAlign="center" color="$white" size="sm">
          {user?.username}
        </Heading>
        <VStack space="md" paddingTop="$5" paddingHorizontal="$10">
          <TouchableWithoutFeedback onPress={handleLogout}>
            <HStack
              opacity={loadingSignOut ? '$60' : '$100'}
              padding="$2"
              alignItems="center"
              justifyContent="space-between"
              space="md"
              borderRadius="$md"
              bgColor="$white"
            >
              <HStack alignItems="center" space="md" paddingHorizontal="$3">
                <Box borderWidth="$1" borderRadius="$full" padding="$1">
                  <Icon name="logout" size={20} />
                </Box>
                <Text bold color="$black">
                  Đăng xuất
                </Text>
              </HStack>

              {loadingSignOut ? <Spinner /> : null}
            </HStack>
          </TouchableWithoutFeedback>
          <CollapseChangePassword user={user} />
        </VStack>
      </VStack>
      <View
        position="absolute"
        top="0"
        right={0}
        borderTopColor="$blue700"
        style={{
          width: 0,
          height: 0,
          backgroundColor: 'transparent',
          borderStyle: 'solid',
          borderRightWidth: 500,
          borderTopWidth: 500,
          borderRightColor: 'transparent',
          transform: [{ rotate: '90deg' }],
          zIndex: -1
        }}
      />
    </View>
  )
}
export default ProfileScreen

const CollapseChangePassword = ({ user }) => {
  const [loading, setLoading] = useState(false)
  const [oldPassword, setOldPassword] = useState('')
  const [newPassword, setNewPassword] = useState('')

  const handleSave = async () => {
    if (!(oldPassword && newPassword)) return
    setLoading(true)
    try {
      const user_db = await verifyUserAsync({
        username: user.username,
        password: oldPassword
      })
      if (user_db?.status === 'ok') {
        const res = await changePasswordAsync(user.id, newPassword)
        if (res.success) {
          Alert.alert('Đổi mật khẩu thành công')
        } else {
          Alert.alert('Đổi mật khẩu thất bại')
        }
      } else {
        Alert.alert('Đổi mật khẩu thất bại')
      }
    } catch (err) {
      Alert.alert('Có lỗi xảy ra ! Vui lòng thử lại')
    } finally {
      setLoading(false)
    }
  }
  return (
    <Accordion
      size="lg"
      variant="unfilled"
      type="single"
      isCollapsible={true}
      isDisabled={false}
      bgColor="$white"
      borderRadius="$md"
    >
      <AccordionItem value="a">
        <AccordionHeader>
          <AccordionTrigger>
            {({ isExpanded }) => {
              return (
                <>
                  <HStack alignItems="center" space="md">
                    <Box borderWidth="$1" borderRadius="$full" padding="$1">
                      <Icon name="onepassword" size={20} />
                    </Box>
                    <Text bold color="$black">
                      Đổi mật khẩu
                    </Text>
                  </HStack>
                  {isExpanded ? (
                    <AccordionIcon as={ChevronUpIcon} ml="$3" />
                  ) : (
                    <AccordionIcon as={ChevronDownIcon} ml="$3" />
                  )}
                </>
              )
            }}
          </AccordionTrigger>
        </AccordionHeader>
        <AccordionContent>
          <FormControl marginBottom="$4" size="md">
            <FormControlLabel>
              <FormControlLabelText>Mật Khẩu Cũ</FormControlLabelText>
            </FormControlLabel>
            <Input>
              <InputField
                placeholder="Nhập mật khẩu cũ"
                name="esp_id"
                type="password"
                value={oldPassword}
                onChangeText={text => setOldPassword(text)}
              />
            </Input>
          </FormControl>
          <FormControl marginBottom="$4" size="md">
            <FormControlLabel>
              <FormControlLabelText>Mật khẩu mới</FormControlLabelText>
            </FormControlLabel>
            <Input>
              <InputField
                placeholder="Nhập mật khẩu mới"
                value={newPassword}
                name="esp_id"
                type="password"
                onChangeText={text => setNewPassword(text)}
              />
            </Input>
          </FormControl>
          <HStack justifyContent="flex-end">
            <Button
              width="$24"
              borderRadius="$3xl"
              size="sm"
              bgColor="$blue700"
              onPress={handleSave}
              isDisabled={loading}
            >
              {loading ? <Spinner /> : <ButtonText>Lưu</ButtonText>}
            </Button>
          </HStack>
        </AccordionContent>
      </AccordionItem>
    </Accordion>
  )
}
