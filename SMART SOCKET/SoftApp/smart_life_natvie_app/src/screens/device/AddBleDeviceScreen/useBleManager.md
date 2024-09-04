## Tài liệu sử dụng useBleManager.js

`bleManager`: Cấu hình và khởi tạo đối tượng **BleManager** từ thư viện **react-native-ble-plx**.
`FILTER_NAME_DEVICE` ; Chuỗi lọc tên thiết bị Bluetooth để chỉ chọn thiết bị phù hợp.
`WIFI_CONNECT_FAILED` : Mã trạng thái WiFi kết nối thất bại.

## Tham số đầu vào

`navigation`: Đối tượng điều hướng từ React Navigation để điều hướng người dùng.

## Các state

- `isScanning`: Biến trạng thái cho biết liệu thiết bị có đang quét hay không.
- `devices`: Danh sách các thiết bị Bluetooth tìm thấy.
- `selectedDevice`: Thiết bị Bluetooth đang được kết nối.
- `isPairing`: Biến trạng thái cho biết liệu thiết bị có đang được ghép nối không.
- `statusWifi`: Trạng thái kết nối WiFi.

## Các hàm

- `useEffect`: Theo dõi và xử lý thay đổi trạng thái của Bluetooth. Nếu Bluetooth được bật, bắt đầu quét thiết bị. Nếu Bluetooth bị tắt, yêu cầu người dùng cấp quyền sử dụng Bluetooth và bật Bluetooth nếu cần.

- `startScanning`: Bắt đầu quét các thiết bị Bluetooth. Quét trong vòng 5 giây và dừng lại. Thêm thiết bị vào danh sách devices nếu tên thiết bị chứa FILTER_NAME_DEVICE.

- `connectToDevice`: Kết nối đến một thiết bị Bluetooth đã chọn. Khi kết nối thành công, giám sát đặc tính của thiết bị và xử lý các phản hồi từ thiết bị. Dựa vào phản hồi từ thiết bị, thực hiện các hành động như cập nhật cơ sở dữ liệu hoặc thêm thiết bị vào Redux store.

## Giá trị trả về

- `isScanning`: Trạng thái quét hiện tại.
- `devices`: Danh sách thiết bị Bluetooth đã tìm thấy.
- `selectedDevice`: Thiết bị Bluetooth đang kết nối.
- `isPairing`: Trạng thái ghép nối hiện tại.
- `statusWifi`: Trạng thái kết nối WiFi.
- `startScanning`: Hàm bắt đầu quét các thiết bị Bluetooth.
- `connectToDevice`: Hàm kết nối đến một thiết bị Bluetooth.
- `setStatusWifi`: Hàm cập nhật trạng thái kết nối WiFi.

## Cách sử dụng

1. Nhập hook vào component:

```jsx
import useBleManager from './path/to/useBleManager'
```

2. Sử dụng hook trong component:

```jsx
const {
  isScanning,
  devices,
  selectedDevice,
  isPairing,
  statusWifi,
  startScanning,
  connectToDevice,
  setStatusWifi
} = useBleManager(navigation)
```
