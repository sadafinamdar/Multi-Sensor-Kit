void setup()
{
Serial.begin(9600);
}
void loop()
{
int val = analogRead(A0); // read input value
Serial.print("Pin Value ");
Serial.println(val);
delay(2000);
}