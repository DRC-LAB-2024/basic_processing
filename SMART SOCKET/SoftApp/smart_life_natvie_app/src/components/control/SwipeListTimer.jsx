import React, { useState } from 'react'
import { View, Text, StyleSheet, TouchableOpacity } from 'react-native'
import { SwipeListView } from 'react-native-swipe-list-view'
import Spinner from 'react-native-loading-spinner-overlay'

import { timeToDay } from '@/utils'
import { updateInfoControlDeviceAsync } from '@/api/device'
import { deviceChangeAsync } from '@/api/esp'

const SwipeListTimer = ({ setListTimer, list, device_name, port, esp_id }) => {
  const [loading, setLoading] = useState(false)

  const deleteRow = async rowType => {
    setLoading(true)
    try {
      const req = {
        port,
        esp_id,
        type: rowType,
        values: {
          [rowType]: 'NULL'
        }
      }
      await updateInfoControlDeviceAsync(req)
      await deviceChangeAsync(esp_id)
      setListTimer(prev => prev.filter(item => item.type !== rowType))
    } catch (error) {
      console.log(error)
    } finally {
      setLoading(false)
    }
  }
  const renderItem = data => {
    const [dayOfWeek, timePart, datePart] = timeToDay(data.item.time)
    return (
      <View style={styles.rowFront}>
        <Text style={{ fontSize: 30, textAlign: 'left' }}>{timePart}</Text>
        <Text style={{ fontSize: 14, marginLeft: 10, color: 'gray' }}>
          {dayOfWeek} - {datePart}
        </Text>
        <Text style={{ fontSize: 14, marginLeft: 10, color: 'gray' }}>
          {device_name} : {data.item.type == 'time_on' ? 'BẬT' : 'TẮT'}
        </Text>
      </View>
    )
  }

  const renderHiddenItem = data => (
    <View style={styles.rowBack}>
      <TouchableOpacity
        style={[styles.backRightBtn, styles.backRightBtnRight]}
        onPress={() => deleteRow(data.item.type)}
      >
        <Text style={styles.backTextWhite}>Xóa</Text>
      </TouchableOpacity>
    </View>
  )

  return (
    <View style={styles.container}>
      <Spinner visible={loading} animation="fade" />
      <SwipeListView
        data={list}
        renderItem={renderItem}
        renderHiddenItem={renderHiddenItem}
        rightOpenValue={-75}
      />
    </View>
  )
}

const styles = StyleSheet.create({
  container: {
    backgroundColor: 'white',
    flex: 1
  },
  rowFront: {
    backgroundColor: '#FFF',
    borderBottomColor: 'black',
    justifyContent: 'center',
    height: 100,
    gap: 2
  },
  rowBack: {
    alignItems: 'center',
    backgroundColor: '#FFF',
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'space-between',
    paddingLeft: 15
  },
  backRightBtn: {
    alignItems: 'center',
    bottom: 0,
    justifyContent: 'center',
    position: 'absolute',
    top: 0,
    width: 75
  },
  backRightBtnRight: {
    backgroundColor: 'red',
    right: 0
  },
  backTextWhite: {
    color: 'white'
  }
})

export default SwipeListTimer
