//해야될 일: 수위 측정 센서로 손실될 물의 양 계산하기

const int WATER_SENSOR_PIN = A0;
 
#include <SimpleTimer.h>

SimpleTimer timer;

void setup(){
 
  pinMode(12, OUTPUT); //폐기 여부에 대하여 LED로 신호함(이부분 미완)
  pinMode(13, OUTPUT); //13번 핀을 물 공급용 솔레노이드 밸브 제어장치로 지정함

  timer.setInterval(21600000, control);
}

void loop(){
  timer.run();
}

void control(){

  int sensorVal = analogRead(WATER_SENSOR_PIN);
  
  //물 잃은 양 보정 방법: 최대 전압 1024에서 물에 잠기는 범위가 넓어질수록 줄어든다.
  float waterloss = sensorVal/1024 * (보정해야 하는 값);
  float need_wat = waterloss/2;
  int wat_open_time;
  digitalWrite(13,LOW);
  delay(wat_open_time); 
  digitalWrite(13,HIGH);
}
