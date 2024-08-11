//-------------PRODUCER NAME-----------
String PRODUCER_NAME = "OPTIMA ROBOTICS";
String PRODUCT_VERSION = "FW: ORC_DRC_24.08.01";
String APP_VERSION = "APP: SRC_24.08.01";

#define POINT_MEMORY_INDEX_LEN_ALL 8
#define POINT_MEMORY_INDEX_MODE 0
#define POINT_MEMORY_INDEX_LAT 1
#define POINT_MEMORY_INDEX_LONG 2
#define POINT_MEMORY_INDEX_Z 3
#define POINT_MEMORY_INDEX_TIME 4
#define POINT_MEMORY_INDEX_LEN 5
#define POINT_MEMORY_INDEX_INDEX 6
#define POINT_MEMORY_INDEX_CHECKSUM 7

//--------------HEADER & FOOT OF UART DATA
#define UART_DATA_HEADER '*'
#define UART_DATA_FOOTER '#'

//-------------BLUETOOTH ID CUA ESP 32-----------
String ESP32_BLUETOOTHID = "...";

//định nghĩa oled
#define SCREEN_WIDTH 128 // Chiều rộng màn hình OLED
#define SCREEN_HEIGHT 64 // Chiều cao màn hình OLED
#define OLED_RESET    -1 // Reset pin (không dùng)

// định nghĩa nút nhấn, led và switch
#define ButtonStart_PIN 6 //3
#define ButtonStop_PIN  32 //32 //2
#define ButtonEmer_PIN  36 //4
#define ButtonMenu_PIN  33
#define DKBTN_VALUE_DONOTHING 0
#define DKBTN_VALUE_START 1
#define DKBTN_VALUE_STOP 2
#define DKBTN_VALUE_EMER 8

#define JoyLeftX_PIN A10
#define JoyLeftY_PIN A11
#define JoyRightX_PIN A8
#define JoyRightY_PIN A9

#define LedRFNhan 40 //LedY 40  //Đèn vàng sáng khi thiết lập được RF: sáng : kết nối được RF Drone, tắt: không kết nối được.
#define LedDKC 30//30// LedB 30//30 //5  //Đèn xanh , chế độ điều khiển: Start: đèn sáng, Stop: Đèn tắt, Emer: Nhấp nháy 500ms
#define LedCDBay 42//44//LedO 44//44 //Đèn cam , chế độ hoạt động: Manual (Vị trí): Đèn sáng, Manual(góc): Đèn tắt, Auto: Nhấp nháy 500ms  (Xem lại chan nay)
#define LedRF_ESP 44 //khi có tín hiệu rf gửi đến esp thì đèn sáng

#define AUTOMANUAL_MODE_PIN 38
#define MANUALPOS_ANL_MODE_PIN 39
#define MANUALMODE_ANGLE_SEND 4
#define MANUALMODE_POS_SEND 1
#define AUTOMODE_SINGLEPOINT_SEND 3
#define AUTOMODE_MULTIPLEPOINT_SEND 2
#define PUMP_MODE_PIN 7

//--------------SCALE VALUE DEFINITION-------------
#define rollscale_pin A12
#define rollscale_MAX 138
#define rollscale_MIN 50

#define pitchscale_pin A13
#define pitchscale_MAX 138
#define pitchscale_MIN 50

#define zscale_pin A15
#define zscale_MAX 5000
#define zscale_MIN 100

#define yawscale_pin A14
#define yawscale_MAX 180
#define yawscale_MIN 40
// #define SwiLe 30
// #define SwiRi 28

#define SerEsp Serial1

//---------VARIABLES FOR RF SENDING DATA------------------------
long ctimeRF, ptimeRF;
//---------END OF VARIABLES FOR RF SENDING DATA-----------------
//--------VARIABLES FOR PUMP STATUS
bool ManualPumpStatus = false;
long ManualPumpFlow = 2;
