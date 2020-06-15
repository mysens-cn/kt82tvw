/*
   Documentation: http://www.mysensors.org
   Support Forum: http://forum.mysensors.org
*/

// Enable debug prints to serial monitor
// #define MY_DEBUG
//#define MY_DISABLE_REMOTE_RESET

#define MY_RADIO_NRF5_ESB
#define MY_RF24_PA_LEVEL RF24_PA_MAX //MINI LOW HIGH MAX


#define MY_BAUD_RATE 9600 //115200 19200 9600 =8MHZ  4800 =1MHZ
// #define MY_TRANSPORT_OPENLINK_CHECK_DISABLED
// #define MY_TRANSPORT_WAIT_READY_MS 2000  //without gateway after 5 seconds
// #define MY_OTA_FIRMWARE_FEATURE
// #define MY_REPEATER_FEATURE //注释下列启用中继功能

//#define MY_NODE_ID 30

char SKETCH_NAME[] = "KT82 Covers";
char SKETCH_VERSION[] = "1.0";
/*
  55 FE FE 03 02 F9 25     关   00 00
  55 FE FE 03 01 B9 24    开 00 00
  55 FE FE 03 03 38 E5    停 00 00
  55 FE FE 01 02 01 4D 43 96 位置值

  55 FE FE 03 08 79 22 出厂设置
  55 FE FE 01 05 01 7D F2 43 电机状态
  55 FE FE 01 04 01 ED A3 EF 手拉使能状态
  55 FE FE 02 04 01 00 9A 26 42 手拉使能开
  55 FE FE 02 04 01 01 9A 27 D2 手拉使能关
*/
byte requesclose[] = { 0x55, 0xFE, 0xFE, 0x03, 0x02, 0xF9, 0x25 };//关
byte requestopen[] = { 0x55, 0xFE, 0xFE, 0x03, 0x01, 0xB9, 0x24 };//开
byte requeststop[] = { 0x55, 0xFE, 0xFE, 0x03, 0x03, 0x38, 0xE5 };//停
byte requestlevel[] = {0x55, 0xFE, 0xFE, 0x01, 0x02, 0x01, 0x4D, 0x43, 0x96};//位置请求
byte requestreset[] = {0x55, 0xFE, 0xFE, 0x03, 0x09, 0x01, 0xBD};//重置

float _t15 = 8000000 / 9600;

unsigned short bufCnt = 0;
const unsigned long eotTimeoutMs = 35;    // if no char received for 400 ms, declare end of transmission
const unsigned int MAXBUF = 20;
char buffer[MAXBUF];
int pos;

int lastpos = 0;
int poscount;
char posmsg;
char gwhex;

#include <MySensors.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>

SoftwareSerial mySerial(3, 4); // RX, TX
// Actuators for moving the cover up and down respectively.
#define CHILD_ID 11
#define  LED_PIN  8  //battery  sensor

MyMessage OpenMessage(CHILD_ID, V_UP);
MyMessage CloseMessage(CHILD_ID, V_DOWN);
MyMessage StopMessage(CHILD_ID, V_STOP);
MyMessage PosMessage(CHILD_ID, V_PERCENTAGE);

void blinkity(uint8_t pulses, uint8_t repetitions) {
  for (int x = 0; x < repetitions; x++) {
    for (int i = 0; i < pulses; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(20);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
    delay(100);
  }
}

enum State {
  STOP,
  OPEN, // Window covering. Up.
  CLOSE, // Window covering. Down.
};

static int state = STOP;
static int status = 0; // 0=cover is down, 1=cover is up
static bool initial_state_sent = false;

// Internal representation of the cover state.


void sendState() {
  // Send current state and status to gateway.
  wait(100);
  send(OpenMessage.set(state == OPEN));
  wait(100);
  send(CloseMessage.set(state == CLOSE));
  wait(100);
  send(StopMessage.set(state == STOP));
  wait(100);
  char kstatus[1];
  itoa(status, kstatus, 10);
  send(PosMessage.set(kstatus));
  wait(100);
  posvalue();
}

void setup() {
  blinkity(2,3);
  mySerial.begin(9600);
  Serial.begin(9600);
  if (!initial_state_sent) {
    sendState();
    initial_state_sent = true;
  }
}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);
  wait(100);
  present(CHILD_ID, S_COVER, "KT82 Covers");
  wait(100);
}

void loop() {
  posloop();
  posvalue();
}

void posloop() {
  unsigned long lastCharTime = 0;
  if (mySerial.available() > 0) {            // enter "receiving" state if something is waiting to be read()
    buffer[bufCnt++] = mySerial.read();    // read first char
    lastCharTime = millis();             // timer start
    while ((mySerial.available() > 0) || (millis() - lastCharTime < eotTimeoutMs)) {
      if (mySerial.available() > 0) {
        buffer[bufCnt++] = mySerial.read();
        lastCharTime = millis();             // timer restart
      }
    }
    // "close" buffer
    buffer[bufCnt] = 0;
    //    Serial.println(buffer); //原始数据
    //    Serial.println(bufCnt);
  }
  if (bufCnt == 16 || bufCnt == 7 || bufCnt == 8) {
    poscount = 10 ;
  }
  bufCnt = 0;
}
void posvalue()
{
  if (poscount > 0) {
    char inData[9];
    wait(500);
    mySerial.write(requestlevel, sizeof(requestlevel));
    delayMicroseconds(100);
    mySerial.readBytes( (char*) inData, 9);
    delay(50);
    //    Serial.println(inData[6], HEX);
    pos =  (inData[6]);
    //    Serial.println(pos);
    if (pos >= 0 && pos <= 100 ) {
      if (lastpos != pos ) {
      	char kpos[4];
    	itoa(pos, kpos, 10);
    	wait(100);
        send(PosMessage.set(kpos));
        wait(100);
        lastpos = pos;
      }
      else {
        poscount = 0 ;
      }
    }
  }
}

void receive(const MyMessage & message) {
  if (message.type == V_UP) {
    mySerial.write(requestopen, sizeof(requestopen));
  }

  if (message.type == V_DOWN) {
    mySerial.write(requesclose, sizeof(requesclose));
  }

  if (message.type == V_STOP) {
    mySerial.write(requeststop, sizeof(requeststop));
  }

  if (message.type == V_PERCENTAGE ) {
    //    posmsg  = atoi( message.data)
    uint8_t buff[] = {0x55, 0xFE, 0xFE, 0x03, 0x04, 0x12};
    uint8_t pos[] = {0x55, 0xFE, 0xFE, 0x03, 0x04, 0x12, 0x00, 0x00};
    buff[5] =  atoi( message.data);
    pos[5] =  atoi( message.data);
    uint16_t buff_size = sizeof(buff) / sizeof(buff[0]);
    uint16_t crc = Get_CRC16(buff , buff_size);
    //    cout << +crc << endl;
    // 将 uint16_t 转为 uint8_t 数组
    uint8_t *crc8 = (uint8_t *) &crc;
    pos[6] = crc8[0];
    pos[7] = crc8[1];
    //    Serial.println(crc8[0], HEX);
    //    Serial.println(crc8[1], HEX);
    mySerial.write(pos, sizeof(pos));
  }
}

//

uint16_t CRC16_Tab[] =
{
  0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
  0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};

//--------------------------------------------------------------
// 函数名称: Calc_Crc16
// 功能描述: 计算单个字节的CRC16
// 输入参数: 输入字节,crc16初始值
// 返回参数: 无
// 修改记录:
//           2015-01-11, PiaoHong 创建函数
//---------------------------------------------------------------
uint16_t Calc_Crc16(uint8_t crcbuf, uint16_t crc_16)
{
  uint8_t temp = ((uint8_t)(crc_16 & 0x000F)) ^ (crcbuf & 0x0F);
  crc_16 >>= 4;
  crc_16 ^= CRC16_Tab[temp];
  temp = ((uint8_t)(crc_16 & 0x000F)) ^ (crcbuf >> 4);
  crc_16 >>= 4;
  crc_16 ^= CRC16_Tab[temp];
  return crc_16;
}


//--------------------------------------------------------------
// 函数名称: Get_CRC16_byGID
// 功能描述: 得到CRC16
// 输入参数: 待进行CRC校验计算的报文  待校验的报文长度
// 返回参数: CRC16
// 修改记录:
//           2015-01-11, PiaoHong 创建函数
//---------------------------------------------------------------

uint16_t Get_CRC16(uint8_t *pBuff, uint16_t len)
{
  uint8_t i;
  uint16_t crc16 = 0xFFFF;

  for (i = 0; i < len; i++)
  {
    crc16 = Calc_Crc16(*pBuff++, crc16);
  }

  return crc16;
}
