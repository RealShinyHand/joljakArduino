#include <Stepper.h>
#include "DHT.h"
/*스케치 - 라이브러리 포함하기 - 라이브러리 관리 실행 후 LiquidCrystal I2C by Frank de Brabander 라이브러리 설치*/

// LCD를 쉽게 제어하기 위한 라이브러리를 추가
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define HUM_TEMP_SENSOR 4 // 온습도 센서는 4번에 연결한다.
#define LED_PIN 5
const int SOUND_SENSOR = A0; //사운드 센서 입력핀 A0, 출력핀 부족 3번핀 출력으로
#define DHTTYPE DHT22 //DHT22 온습도 센서 사용
// 모터의 핀은 8,9,10,11

// 모터의 스텝 수 2048=1바퀴, 1024=반바퀴
const int steps = 200;

// myStepper 객체 생성(스텝 수, IN4, IN3, IN2, IN1)
// IN1~4까지 각각 아두이노 D8~D11번에 연결
Stepper myStepper(steps, 11, 9, 10, 8);


//0x3F I2C 주소를 가지고 있는 16x2 LCD객체 생성(I2C 주소는 LCD에 맞게 수정)
//0x27일 수도 있고 0x3F일 수도 있다
LiquidCrystal_I2C lcd(0x27, 16, 2);



DHT dht(HUM_TEMP_SENSOR, DHTTYPE); //DHT 설정(4,DHT22)
int soundValue = 0; // 사운드값
int maxSoundValue = 0;
int is_move = 0; // 모터 움직이고 있나요?
int is_ledOn = 0; //led 켜져있냐?
int is_cry = 0; // 아기가 울고 있는지 아닌지?
unsigned long loopTime = 0;
unsigned long addTime = 0;

/*
   A0 : sound sensor
   4 : soundsensor
   5: led
   스텝모터 연결
   8 <- blue -> IN1
   9<- white -> IN2
   10<-yellow-> IN3
   11<-orange-> IN4
   VCC와 VCC 연결
   GND와 GND 연결
*/



void setup() {
  // put your setup code here, 한번만 실행되는 초기화코드
  // Create 5 threads and add them to the main ThreadList:
  dht.begin();
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(3, HIGH);
  Serial.begin(9600); //통신속도 9600으로 통신 시작
  Serial.println("# arduino DHT22 test!"); //온습도센서 test
  Serial.println("# arduino Sound Sensor test!"); //사운드 센서 test
  myStepper.setSpeed(60); // 15RPM구동
  lcd.init();
  lcd.clear();
  lcd.display();
  lcd.setCursor(0, 0);
  lcd.print("qwert");

  delay(1000);

}

void loop() {
  // put your main code here, 계속해서 실행되는 메인 코드

  addTime = millis();
  soundValue = analogRead(SOUND_SENSOR);
  if (maxSoundValue < soundValue ) {
    maxSoundValue = soundValue;
  }

  if (loopTime >= 3000) {
    loopTime = 0;
    // 온습도 센서 부분
    float humidity = dht.readHumidity(); //습도값을 humidity에 저장
    float temparature = dht.readTemperature(); //온도값을 temparature에 저장
    printLED(temparature, humidity);
    if (isnan(humidity) || isnan(temparature)) {
      Serial.println("# arduino DHT22 ERROR");
    }
    Serial.println(temparature + String(" ") + humidity + String(" ") + is_move + String(" ") + maxSoundValue + String(" ") + is_ledOn);
    maxSoundValue = 0;
  }

  if (is_move != 0) {
    myStepper.step(100);
  }
  if (is_ledOn != 0) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  loopTime = loopTime + millis() - addTime;

  if (Serial.available() > 0) {
    //raspberry pi send format : b"11" == b"(모터)(led)(\n)"
    String msg = Serial.readStringUntil('\n');
    Serial.println("# " + msg);
    is_move = msg.charAt(0);
    is_ledOn = msg.charAt(1);


  }
}

void printLED(float temparature, float humidity) {
  lcd.backlight();	//	lcd 백라이트 on
  // LCD의 모든 내용을 삭제
  lcd.clear();
  //0번째 줄 0번째 셀부터 입력하게 함
  lcd.setCursor(0, 0);
  lcd.print(String("TEMP:     ") + temparature);

  //1번째 줄 0번째 셀부터 입력하게 함
  lcd.setCursor(0, 1);
  lcd.print(String("HUMIDITY: ") + humidity);
  lcd.print(humidity);
}
