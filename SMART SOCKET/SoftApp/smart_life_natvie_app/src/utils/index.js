export const compareTime = inputTime => {
  if (!inputTime) return false
  //Parse the input time
  const [inputHours, inputMinutes] = inputTime.split(':').map(Number)

  // Get current time
  const now = new Date()
  const currentHours = now.getHours()
  const currentMinutes = now.getMinutes()

  // Compare time
  if (inputHours < currentHours) return false
  else if (inputHours === currentHours) return inputMinutes > currentMinutes
  else return true
}
export const formatTime = inputTime => {
  if (!inputTime) return
  const date = new Date(inputTime)
  const time = new Date(inputTime).toLocaleTimeString('en-GB')

  // Extracting the date components
  const day = String(date.getUTCDate()).padStart(2, '0')
  const month = String(date.getUTCMonth() + 1).padStart(2, '0') // Months are zero-based
  const year = date.getUTCFullYear()

  const [hours, minutes, ss] = time.split(':')

  // Formatting to dd/mm/yyyy - hh:mm
  return `${day}/${month}/${year} - ${hours}:${minutes}`
}
export const timeToDay = inputTime => {
  // Extract the date and time parts
  const [datePart, timePart] = inputTime.split('-')
  const [day, month, year] = datePart.split('/').map(Number)
  const [hours, minutes] = timePart.split(':').map(Number)

  // Create a Date object for the given time
  const givenTime = new Date(year, month - 1, day, hours, minutes)

  // Array of days of the week
  const daysOfWeek = [
    'Chủ nhật',
    'Thứ hai',
    'Thứ ba',
    'Thứ tư',
    'Thứ năm',
    'Thứ sáu',
    'Thứ bảy'
  ]

  // Get the day of the week
  const dayOfWeek = daysOfWeek[givenTime.getDay()]
  return [dayOfWeek, timePart, datePart]
}

export const compareTimes = inputTime => {
  const [datePart, timePart] = inputTime.split('-')
  const [day, month, year] = datePart.split('/')
  const [hours, minutes] = timePart.split(':')

  const givenTime = new Date(year, month - 1, day, hours, minutes)

  const currentTime = new Date()
  if (givenTime > currentTime) {
    return true
  } else if (givenTime < currentTime) {
    return false
  }
  return false
}
