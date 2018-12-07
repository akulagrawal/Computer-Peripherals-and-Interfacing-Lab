const int ledpin = 13;
int n;
int m;
String readstr;

void setup()
{
  pinMode(ledpin, OUTPUT);
  Serial.begin(9600);
  n=7;
  m=5;
}

void loop()
{
  /* if(Serial.available())
  {
    n = Serial.read()-'0';
  }
  else
  {
    Serial.println("not available");
  } */
  
  while(Serial.available())
  {
    if(Serial.available()>0)
    {
      char c = Serial.read();
      readstr += c;
      n = c - '0';
    }
  }
  String first = readstr.substring(0,1); 
  String x_sub = readstr.substring(1,4);
  String y_sub = readstr.substring(4,7);
  String z_sub = readstr.substring(7,11);
  
  int x=0,y=0,z=0;
  x = x_sub.toInt();
  y = y_sub.toInt();
  z = z_sub.toInt();
  
  if(readstr.length() > 0)
  {
    Serial.println("Recieved " + first);
    //Serial.print(readstr);
    Serial.println("int x = " + (String)(x));
    Serial.println("int y = " + (String)(y));  
    Serial.println("int z = " + (String)(z));  
  }
  readstr = "";
  Serial.print(n);
  Serial.print("ab");
  Serial.println();
  delay(1000);
}
