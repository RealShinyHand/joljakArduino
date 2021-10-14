//I2C LCD GND 핀을 GND에
//I2C LCD VCC 핀을 5V 핀에
//I2C LCD SDA 핀을 아날로그 A4에
//I2C LCD SCL 핀을 아날로그 A5에

/*스케치 - 라이브러리 포함하기 - 라이브러리 관리 실행 후 LiquidCrystal I2C by Frank de Brabander 라이브러리 설치*/

// LCD를 쉽게 제어하기 위한 라이브러리를 추가
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//0x3F I2C 주소를 가지고 있는 16x2 LCD객체 생성(I2C 주소는 LCD에 맞게 수정)
//0x27일 수도 있고 0x3F일 수도 있다
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
	lcd.init();
}


void loop() {

	humidity = 현재습도;
	temperature = 현재온도;

	lcd.backlight();	//	lcd 백라이트 on
	lcd.display();		//	lcd 내용 표시

	//0번째 줄 0번째 셀부터 입력하게 함
	lcd.setCursor(0, 0);
	lcd.print("TEMP:     ");
	lcd.print(temprature);

	//1번째 줄 0번째 셀부터 입력하게 함
	lcd.setCursor(0, 1);
	lcd.print("HUMIDITY: ");
	lcd.print(humidity);

	// 1초간 대기
	delay(1000);
	// LCD의 모든 내용을 삭제
	lcd.clear();
}