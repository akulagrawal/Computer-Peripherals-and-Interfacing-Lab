//--------------- Global Variables -----------------------------------
const int analogPin1 = A0;  // first analog input
const int analogPin2 = A1;  // second analog input

const int add_pin = 4;    // To show addition of input
const int sub_pin = 8;    // To show subtraction of input
                  
int val1 = 0;               // variable to store the value read1
int val2 = 0;               // variable to store the value read2

int addval = 0;
int subval = 0;
//------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);              //  setup serial
  
  // initialize input pins.
  pinMode(analogPin1, INPUT);
  pinMode(analogPin2, INPUT);

  // initialize output pins.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(add_pin, OUTPUT);
  pinMode(sub_pin, OUTPUT);
}
//------------------------------------------------------------------
void loop()
{
  val1 = analogRead(analogPin1);     // read the input pin
  val2 = analogRead(analogPin2);     // read the input pin
  
  addval = val1 + val2;
  subval = val1 - val2;

//  Serial.println(val1);               // debug value
//  Serial.println(val2);               // debug value
    Serial.println(addval);             // debug value
//  Serial.println(subval);             // debug value
  
  analogWrite(add_pin, map(addval, 0, 1023, 0, 255));
  analogWrite(sub_pin, map(subval, 0, 1023, 0, 255));

  analogWrite(LED_BUILTIN, map(val1, 0, 1023, 0, 255));
  delay(100);                           // Delay in ms
}
//--------------------------- End -----------------------------------
