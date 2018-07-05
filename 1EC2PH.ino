/* VernierLibTutorialAnalogRead (v2017)
 * This sketch reads a data point from a Vernier Analog (BTA) 
 * sensor once every half second and prints the sensor reading 
 * with units to the Serial Monitor.
 * 
 * Plug the sensor into the Analog 1 port on the Vernier Arduino 
 * Interface Shield or into an Analog Protoboard Adapter wired 
 * to Arduino pin A0.
 */


#include "VernierLib.h" //include Vernier functions in this sketch
VernierLib Vernier; //create an instance of the VernierLib library
float sensorReading; //create global variable to store sensor reading

#include <SimpleTimer.h>
SimpleTimer timer;

const int PHPin = A2;

void setup() {
  Serial.begin(9600); //setup communication to display
  Vernier.autoID(); //identify the sensor being used
  pinMode(12, OUTPUT); //12번 핀을 양액주입용 솔레노이드 밸브 제어장치로 지정함
  pinMode(13, OUTPUT); //13번 핀을 질산주입용 솔레노이드 밸브 제어장치로 지정함

  timer.setInterval(21600000, valancer);
}
 
void loop() {
  timer.run();
  sensorReading = Vernier.readSensor(); //read one data value
  Serial.print("EC: ");
  Serial.print(sensorReading); //print data value 
  Serial.print(" "); //print a space
  Serial.println("uS/cm"); //print units and skip to next line
  Serial.print("EC: ");
  Serial.print(0.641*sensorReading);
  Serial.println("ppm");

  float voltage, pH; //Declare 2 floating point variables
  voltage = getVoltage(PHPin); //Measure the voltage at the analog pin
  pH = ((1.75 - voltage) / .25) + 7 ; // Convert the voltage to pH levels
  Serial.print("PH: ");
  Serial.print(pH);

  if(sensorReading > 1000 || pH > 7.5 || pH <3){
    Serial.println("ALERT, need to change water");//모든 밸브 닫고 폐기 신호 보내기
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    delay(100000000);
  }
  
  delay(1000); //wait half second
}

float getVoltage(int pin)   //Function to read and return
                            //floating-point value (true voltage)
                            //on analog pin 
{
    return (analogRead(pin) * 0.004882814); 
    // This equation converts the 0 to 1023 value that analogRead()
    // returns, into a 0.0 to 5.0 value that is the true voltage
    // being read at that pin.
}

void valancer(){
  Serial.println("Checking");
  float ec_ppm_val = Vernier.readSensor();
  while (ec_ppm_val < 800){
    Serial.println("EC is low! open the valve");
    ec_ppm_val =Vernier.readSensor();
    Serial.print("going to 800, now: ");
    Serial.println(ec_ppm_val);
    digitalWrite(12, LOW);
    delay(1000);
    digitalWrite(12, HIGH);
    delay(3000);
  }
  float voltage = getVoltage(PHPin); //Measure the voltage at the analog pin
  float pH = ((1.75 - voltage) / .25) + 7 ; // Convert the voltage to pH levels
  while(pH > 4) {
    Serial.println("PH is high! open the valve");
    voltage = getVoltage(PHPin);
    pH = ((1.75 - voltage) / .25) + 7 ;
    Serial.print("going to 4, now: ");
    Serial.println(pH);
    digitalWrite(13, LOW);
    delay(1000);
    digitalWrite(13, HIGH);
    delay(3000);
  }
}


