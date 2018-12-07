const int ledpin = 13;
void setup()
{
  pinMode(ledpin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  while(Serial.available() < 19);
  
  String readstr = String();
  for(int i = 0; i < 19; ++i)
  {
    char c = Serial.read();
    readstr += c;
  }

  for(int i = 0; i < readstr.length(); ++i)
  {
    Serial.write(readstr.charAt(i));   
  }
}
