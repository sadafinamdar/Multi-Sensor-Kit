#include <LCD5110_Graph.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <ClickEncoder.h>
#include<Wire.h>
#include <TimerOne.h>
//Alcohol
constintAOUTpin=6;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the
arduino
constintDOUTpin=7;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the
arduino
constintledPin=8;//the anode of the LED connects to digital pin D13 of the arduino
//Fire Sensor
intflame_sensor_pin = 6 ; // initializing pin 7 as the sensor output pin
intflame_pin = HIGH; // state of sensor
int buzzer1 = 10;
int buzzer2 = 12;
//Watel Level Sensor 40mm
constintreadw = A7; //Sensor AO pin to Arduino pin A0
int value1; //Variable to store the incomming data
//heart rate
intoutput_Pin = A3; // initializing the sensor output pin
double alpha = 0.75;
int period = 200;
double change = 0.0;
static double oldValue = 0;
static double oldChange = 0;
//gyro
constintMPU_addr=0x68;
int16_tAcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
intminVal=265;
intmaxVal=402;
int x;
double y;
double z;
//lcd
//creating menu items
intmenuitem = 1;
int frame = 1;
int page = 1;
intlastMenuItem = 1;
String menuItem1 = "Alcohol";
String menuItem2 = "FLAME";
String menuItem3 = "Water";
String menuItem4 = "Heart rate";
String menuItem5 = "GYROSCOPE";
boolean backlight = true;
boolean up = false;
boolean down = false;
boolean middle = false;
ClickEncoder *encoder;
int16_t last, value;
LCD5110 lcd(13,11,5,3,4);
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 5, 4, 3); //pin declaration for lcd
extern uint8_t pi[];
void setup() {
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
pinMode(DOUTpin, INPUT);
pinMode (flame_sensor_pin , INPUT );
pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
display.begin();
pinMode(7, OUTPUT);
encoder = new ClickEncoder(A1, A0, A2);
encoder->setAccelerationEnabled(false);
display.begin();
display.clearDisplay();
display.setContrast(50);
Timer1.initialize(1000);
Timer1.attachInterrupt(timerIsr);
last = encoder->getValue();
EPD();
}
void loop() {
drawMenu();
readRotaryEncoder();
//rotation of menu
ClickEncoder::Button b = encoder->getButton();
if (b != ClickEncoder::Open) {
switch (b) {
caseClickEncoder::Clicked:
middle = true;
break;
 }
 }
if (up && page == 1 ) {
up = false;
if (menuitem == 2 && frame == 2)
 {
frame--;
 }
if (menuitem == 4 && frame == 4)
 {
frame--;
 }
if (menuitem == 3 && frame == 3)
 {
frame--;
 }
lastMenuItem = menuitem;
menuitem--;
if (menuitem == 0)
 {
menuitem = 1;
 }
 } else if (page == 2 &&menuitem == 1 ) {
alcohol();
 }
else if ( page == 2 &&menuitem == 2 ) {
fire();
 }
else if (page == 2 &&menuitem == 3 ) {
water();
 }
else if (page == 2 &&menuitem == 4 ) {
heart();
 }
 else if (page == 2 &&menuitem==5 ) {
gyro();
 }
if (down && page == 1) //We have turned the Rotary Encoder Clockwise
 {
down = false;
if (menuitem == 3 &&lastMenuItem == 2)
 {
frame ++;
 } else if (menuitem == 4 &&lastMenuItem == 3)
 {
frame ++;
 }
else if (menuitem == 5 &&lastMenuItem == 4 && frame != 4)
 {
frame ++;
 }
 lastMenuItem = menuitem;
menuitem++;
if (menuitem == 7)

{
menuitem--
;

}
 } else if (page == 2 &&menuitem == 1) {
alcohol(); //call for alcohol sensor

}
else if ( page == 2 &&menuitem == 2) {
fire(); //call for flame sensor

}
else if ( page == 2 &&menuitem == 3 ) {
water(); //call for water sensor

}
else if (page == 2 &&menuitem == 4 ) {
heart(); //call for heart sensor

}
else if (page == 2 &&menuitem==5 ) {
gyro();

}
if (middle) //Middle Button is Pressed

{
middle = false;
if (page == 1 &&menuitem == 6) // Reset

{
resetDefaults();

}
else if (page == 1 &&menuitem<= 5) {
page = 2;

}
else if (page == 2)

{
page = 1;

}

}
}
voiddrawMenu() {
if (page == 1)

{
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(0, 0);
display.print("SENSOR MENU");
display.drawFastHLine(0, 10, 83, BLACK);
if (menuitem == 1 && frame == 1)

{
displayMenuItem(menuItem1, 15, true);
displayMenuItem(menuItem2, 25, false);
displayMenuItem(menuItem3, 35, false);

}
else if (menuitem == 2 && frame == 1)

{
displayMenuItem(menuItem1, 15, false);
displayMenuItem(menuItem2, 25, true);
displayMenuItem(menuItem3, 35, false);

}
else if (menuitem == 3 && frame == 1)

{
displayMenuItem(menuItem1, 15, false);
displayMenuItem(menuItem2, 25, false);
displayMenuItem(menuItem3, 35, true);

}
else if (menuitem == 4 && frame == 2)

{
displayMenuItem(menuItem2, 15, false);
displayMenuItem(menuItem3, 25, false);
displayMenuItem(menuItem4, 35, true);

}
else if (menuitem == 3 && frame == 2)

{
displayMenuItem(menuItem2, 15, false);
displayMenuItem(menuItem3, 25, true);
displayMenuItem(menuItem4, 35, false)
;

}
else if (menuitem == 2 && frame == 2)

{
displayMenuItem(menuItem2, 15, true);
displayMenuItem(menuItem3, 25, false);
displayMenuItem(menuItem4, 35, false);

}
else if (menuitem == 5 && frame == 3)

{
displayMenuItem(menuItem3, 15, false);
displayMenuItem(menuItem4, 25, false);
displayMenuItem(menuItem5, 35, true);

}
else if (menuitem == 6 && frame == 4)

{
displayMenuItem(menuItem4, 15, false);
displayMenuItem(menuItem5, 25, false);
}
else if (menuitem == 5 && frame == 4)

{
displayMenuItem(menuItem4, 15, false);
displayMenuItem(menuItem5, 25, true);

}
else if (menuitem == 4 && frame == 4)

{
displayMenuItem(menuItem4, 15, true);
displayMenuItem(menuItem5, 25, false);

}
else if (menuitem == 3 && frame == 3)

{
displayMenuItem(menuItem3, 15, true);
displayMenuItem(menuItem4, 25, false);
displayMenuItem(menuItem5, 35, false);

}
else if (menuitem == 2 && frame == 2)

{
displayMenuItem(menuItem2, 15, true);
displayMenuItem(menuItem3, 25, false);
displayMenuItem(menuItem4, 35, false);

}
else if (menuitem == 4 && frame == 3)

{
displayMenuItem(menuItem3, 15, false);
displayMenuItem(menuItem4, 25, true);
displayMenuItem(menuItem5, 35, false);

}
display.display();

}
else if (page == 2 &&menuitem == 1)
{
alcohol();

}
else if (page == 2 &&menuitem == 2)

{
fire();

}
else if (page == 2 &&menuitem == 3)

{
water();

}
else if (page == 2 &&menuitem == 4)

{
heart();
}
else if (page == 2 &&menuitem == 5)

{
gyro();

}
}
voidresetDefaults() {
backlight = true; }
voidsetContrast() {
display.setContrast(55); //controlling brightness of lcd display
display.display(); }
voidturnBacklightOn() {
digitalWrite(7, HIGH); }
voidturnBacklightOff() {
digitalWrite(7, HIGH); }
voidtimerIsr() {
encoder
->service();
}
voiddisplayIntMenuPage(String menuItem, int value) {
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem);
display.drawFastHLine(0, 10, 83, BLACK);
display.setCursor(5, 15);
display.setTextSize(2);
display.setCursor(5, 25);
display.setTextSize(2);
display.display(); }
voiddisplayfloatMenuPage(String menuItem, float distance) {
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem);
display.drawFastHLine(0, 10, 83, BLACK);
display.setCursor(5, 15);
display.setTextSize(2);
display.setCursor(5, 25);
display.print(distance);
display.setTextSize(2);
display.display(); }
voiddisplayMenuItem(String item, int position, boolean selected) {
if (selected)

{
display.setTextColor(WHITE, BLACK);
 } else

{
display.setTextColor(BLACK, WHITE);

}
display.setCursor(0, position);
display.print(">" + item); }
voidreadRotaryEncoder() {
value += encoder
->getValue();
if (value / 2 > last) {
last = value / 2;
down = true;
delay(150);
 } else if (value / 2 < last) {
last = value / 2;
up = true;
delay(150);

}
}
void displayIntMenuPage4(String menuItem, int value) {
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem5);
display.drawFastHLine(0, 10, 83, BLACK);
display.setCursor(0, 15);
display.print("OBJECT");
display.setTextSize(1);
display.setTextSize(1);
display.setTextColor(BLACK);
display.setCursor(0, 25);
display.display();
}
void displayIntMenuPage2(String menuItem, int value) {
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem4);
display.drawFastHLine(0, 10, 83, BLACK);
display.setCursor(0, 15);
display.print("MOISTURE");
display.setTextSize(1);
display.setTextSize(2);
display.setTextColor(BLACK);
display.setCursor(0, 25);
display.print("%");
display.display(); }
void displayIntMenuPage3(String menuItem, int value) {
	display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(5, 0);
display.print(menuItem1);
display.drawFastHLine(0, 10, 83, BLACK);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.setCursor(0, 20 );
display.print("X=");
display.setCursor(10, 20 );
display.display();
display.setTextColor(BLACK, WHITE);
display.setCursor(0, 30);
display.print("Y=");
display.setCursor(10, 30);
display.display();
display.setTextColor(BLACK, WHITE);
display.setCursor(0, 40);
display.print("Z=");
display.setCursor(10, 40);
display.display(); }
//starting screen display
void EPD() {
digitalWrite(7, HIGH);
lcd.clrScr();
lcd.drawBitmap(0,0,pi,84,48);
lcd.update();
delay(5000);
display.setTextSize(2);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(0, 0);
display.print("EPD LAB");
display.setCursor(25, 20);
display.setTextSize(2);
display.print("C2");
display.setCursor(5, 35);
display.setTextSize(1);
display.print("SENSOR KIT-2");
display.display();
delay(5000);
return;
}
//flame sensor loop
void fire()
{
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem2);
display.drawFastHLine(0,10,83,BLACK);
flame_pin = digitalRead( flame_sensor_pin ) ; // reading from the sensor
if (flame_pin == LOW ) // applying condition
 {
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.setCursor(0, 18);
display.print("FLAME,FLAME, FLAME");
display.setCursor(0, 39);
display.setTextSize(2);
display.print(char(6));
display.print(char(6));
display.print(char(6));
display.print(char(6));
display.print(char(6));
display.print(char(6));
display.print(char(6));
digitalWrite(buzzer2,HIGH); // otherwise turn it low
digitalWrite(buzzer1,LOW);
 }
 else
 {
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.setCursor(5, 26);
display.print (" NO FLAME! ");
digitalWrite ( buzzer1,LOW); // otherwise turn it low
digitalWrite ( buzzer2,LOW );
 }
display.display();
}
//alcohol sensor loop
void alcohol()
{
int value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's
AOUT pin
int limit= digitalRead(DOUTpin);//reads the digital value from the alcohol sensor's DOUT
pin
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem1);
display.drawFastHLine(0,10,83,BLACK);
display.setTextSize(1);
display.setTextColor(BLACK, WHITE);
display.setCursor(5, 17);
display.print("value:");
display.print(value);//prints the alcohol value
display.setCursor(5, 26);
display.display();
delay(100);
if (limit == HIGH){
digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator
}
else{
digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off
}
}
//water sensor loop
void water()
{
value1 = analogRead(readw); //Read data from analog pin and store it to value variable
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem3);
display.drawFastHLine(0,10,83,BLACK);
display.setTextSize(1);
display.drawFastHLine(0,10,83,BLACK);
display.setCursor(5, 17);
if (value1>0 && value1<=10)
 {
display.print("EMPTY");
display.print(char(176));

}
else if (value1>10 && value1<=250)
 {
display.print("Water level: LOW ");
display.print(char(177));
display.print(char(177));
display.print(char(177));

}
else if (value1>230 && value1<=260)
 {
display.print("Water level: MEDIUM ");
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));

}
else if (value1>265 && value1<=400)
 {
display.print("Water level: HIGH "); 
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));
display.print(char(177));

}
display.display();
delay(500); // Check for new value every 5 sec
}
//heart sensor loop
void heart()
{
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem4);
display.drawFastHLine(0,10,83,BLACK);
display.setTextSize(1);
display.drawFastHLine(0,10,83,BLACK);
display.setCursor(0, 16);
intrawValue = analogRead (output_Pin); // Reading the sensors
values
double value2 = alpha * oldValue + (1 - alpha) * rawValue; // calculating values using
the formula
 // printing the sensor output value on the screen
display.print("Put yor thumb or finger");
display.setCursor(0, 33);
display.print((value2)/7);
 // printing the heart beat value on the screen
display.print(" BPM ");
display.setTextSize(2);
display.print(char(3));
oldValue = value2;
delay (period);
display.display();
}
//gyroscope sensor loop
void gyro()
{
display.setTextSize(1);
display.clearDisplay();
display.setTextColor(BLACK, WHITE);
display.setCursor(15, 0);
display.print(menuItem5);
display.drawFastHLine(0,10,83,BLACK);
display.setTextSize(1);
display.drawFastHLine(0,10,83,BLACK);
display.setCursor(0, 16);
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
intxAng = map(AcX,minVal,maxVal,-90,90);
intyAng = map(AcY,minVal,maxVal,-90,90);
intzAng = map(AcZ,minVal,maxVal,-90,90);
 x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
display.print("AngleX= ");
display.setTextSize(2);
display.print(x);
display.print(char(0176));
delay(500);
display.display();
}