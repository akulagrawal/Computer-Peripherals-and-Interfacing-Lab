/*
Kisaan: The Farm-Monitoring System
Arduino Source Code

by Group 4
Abhishek Suryavanshi
Ameya Daigavane
Nitesh Jindal
Akul Agrawal
*/

#include "AirQuality.h"
#include "Arduino.h"
#include "DHT.h"

// DHT Sensor
const int DHTPIN = 2;
DHT dht(DHTPIN, DHT11);

// Air Quality Sensor 
AirQuality airqualitysensor;

// Ping Sensor Pins
const int trigPin = 49;
const int echoPin = 50;

// Physical pin numbers for motors and sensors. 
const uint8_t STEPPER_x1_PIN[4] = {22, 24, 26, 28};
const uint8_t STEPPER_x2_PIN[4] = {33, 35, 37, 39};
const uint8_t STEPPER_y_PIN[4]  = {42, 44, 46, 48};
const uint8_t STEPPER_z_PIN[4]  = {17, 18, 19, 20};
const uint8_t STOPPER_x_PIN = 51;
const uint8_t STOPPER_y_PIN = 53;

// Coordinate limits for the automatic mode traversal - calibrated according to the motor and frame dimensions.
const int x_automatic_step = 500;
const int y_automatic_step = 300;
const int gridlimits[3] = {2500, 1200, 500};

// Current coordinates.
int x_current = 0;
int y_current = 0;
int z_current = 0;

// Closest distance to the ground along the z-axis.
const uint8_t z_distance_limit = 4;

// Current mode - true represents automatic, false represents manual.
bool current_mode = true;

// Set pin modes.
void setup()
{
    // Set baudrate for serial communication.
    Serial.begin(9600);
    
    airqualitysensor.init(14);
    dht.begin();
    
    // Ping sensor pins.
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT);
    
    // Stepper motor pins.
    for (uint8_t i = 0; i < 4; i++)
    {
        pinMode(STEPPER_x1_PIN[i], OUTPUT);
        pinMode(STEPPER_x2_PIN[i], OUTPUT);
        pinMode(STEPPER_y_PIN[i], OUTPUT);
        pinMode(STEPPER_z_PIN[i], OUTPUT);
    }
    
    // Stopper pins to indicate when we reach grid boundaries.
    pinMode(STOPPER_x_PIN, INPUT);
    pinMode(STOPPER_y_PIN, INPUT);

    get_coordinates();
}


/*    Motor Movement          
   
      The functions x_move(), y_move(), z_move() cycle the respective stepper motors once.
      'pos' stores the position of the magnet to be set to HIGH. The other magnets are set to LOW.
      By cycling 'pos' through the 4 magnets, setting each pin separately, one cycle is completed.
*/

// Move in the x-direction by one step.
void x_move(int direction)
{
    // Update current x-coordinate.
    x_current += direction;

    for (uint8_t pos = 0; pos < 4; pos++)
    {
        // Set orientations of magnets through the pins.
        for(uint8_t pin = 0; pin < 4; ++pin)
        {
            // Set exactly one pin for each motor to be HIGH, depending on the direction. 
            if(pin == pos)
            {
                if (direction == 1)
                {
                    digitalWrite(STEPPER_x1_PIN[pin], HIGH);
                    digitalWrite(STEPPER_x2_PIN[3 - pin], HIGH);
                }
                else
                {
                    digitalWrite(STEPPER_x2_PIN[pin], HIGH);
                    digitalWrite(STEPPER_x1_PIN[3 - pin], HIGH);
                }
            }
            else
            {
                if (direction == 1)
                {
                    digitalWrite(STEPPER_x1_PIN[pin], LOW);
                    digitalWrite(STEPPER_x2_PIN[3 - pin], LOW);
                }
                else
                {
                    digitalWrite(STEPPER_x2_PIN[pin], LOW);
                    digitalWrite(STEPPER_x1_PIN[3 - pin], LOW);
                }
            }
        }
        
        // Delay for the stepper motor.
        delay (2);
    }
} 

// Move in the y-direction by one step.
void y_move(int direction)
{ 
    // Update current y-coordinate.
    y_current += direction;
    
    for (uint8_t pos = 0; pos < 4; pos++)
    {
        // Set orientations of magnets, through the pins.
        for(uint8_t pin = 0; pin < 4; ++pin)
        {
            // Set exactly one pin to be HIGH, depending on the direction. 
            if(pin == pos)
            {
                if (direction == 1)
                    digitalWrite(STEPPER_y_PIN[pin], HIGH);
                else
                    digitalWrite(STEPPER_y_PIN[3 - pin], HIGH);
            }
            else
            {
                if (direction == 1)
                    digitalWrite(STEPPER_y_PIN[pin], LOW);
                else
                    digitalWrite(STEPPER_y_PIN[3 - pin], LOW);
            }
        }
        
        // Delay for the stepper motor.
        delay (2);
    }
}

// Move in the z-direction by one step.
void z_move(int direction)
{
    // Update z-coordinate.
    z_current += direction;
    
    for (uint8_t pos = 0; pos < 4; pos++)
    {
        // Set orientations of magnets, through the pins.
        for(uint8_t pin = 0; pin < 4; ++pin)
        {
            // Set exactly one pin to be HIGH, depending on the direction. 
            if(pin == pos)
            {
                if (direction == 1)
                    digitalWrite(STEPPER_z_PIN[pin], HIGH);
                else
                    digitalWrite(STEPPER_z_PIN[3 - pin], HIGH);
            }
            else
            {
                if (direction == 1)
                    digitalWrite(STEPPER_z_PIN[pin], LOW);
                else
                    digitalWrite(STEPPER_z_PIN[3 - pin], LOW);
            }
        }
        
        // Delay for the stepper motor.
        delay (2);
    }
}

// Make the probe go to coordinates (x_final, y_final, z_final).
void go_to_coordinates (int x_final, int y_final, int z_final)
{
    while ( x_current != x_final || y_current != y_final || z_current != z_final )
    {
        // If we get new data, we need to update.
        if (Serial.available() >= 17)
            break;

        // Decide steps in the 3 dimensions.
        // Step in the x-direction.
        if (x_current < x_final)
            x_move (1);
        else if (x_current > x_final)
            x_move (-1);
            
        // Step in the y-direction.
        if (y_current < y_final)
            y_move (1);
        else if (y_current > y_final)
            y_move (-1);

        // Step in the z-direction.
        if (z_current < z_final)
            z_move (1);
        else if (z_current > z_final)
            z_move (-1);
    }
}

// Go to one end, and exit as soon as we get a signal from the stopper button.
void go_to_start()
{
    while (true)
    {
        // Reached x-axis boundary. 
        if (digitalRead (STOPPER_x_PIN) == HIGH)
            break;
        
        x_move(-1);
    }
    while (true)
    {
        // Reached y-axis boundary.
        if (digitalRead (STOPPER_y_PIN) == HIGH)
            break;
        
        y_move(-1);
    }
    
    // Reset current coordinates.
    x_current = 0;
    y_current = 0;
    z_current = 0;
}


/* Serial Communication Code. */
// Get the old coordinates of controller serially.
void get_coordinates()
{
    // Message to ping.
    Serial.println ("getcoords");
    
    // Wait for data to arrive
    while(Serial.available() < 17);
    
    bool mode = Serial.readStringUntil(',').charAt(0) - '0';
    
    // Update current coordinates.    
    x_current = Serial.readStringUntil(',').toInt();
    y_current = Serial.readStringUntil(',').toInt();
    z_current = Serial.readStringUntil('\n').toInt();
    
    Serial.print("Arduino currently at ");
    send_coordinates();
    Serial.println();
}

// Send the current coordinates serially.
void send_coordinates()
{
    Serial.print (x_current);
    Serial.print (",");
    Serial.print (y_current);
    Serial.print (",");
    Serial.print (z_current);
    Serial.print (",");
}

// Wrapper for getting and printing data.
void read_sensors_and_send()
{
    uint8_t attempt_number;
    for(attempt_number = 0; attempt_number < 4; ++attempt_number)
    {
      send_coordinates();
      read_DHT();
      read_AQS();
      Serial.println();
    }
}

/* Sensor Code. */
// Air Quality Sensor
void read_AQS()
{
    airqualitysensor.slope();
}

ISR(TIMER1_OVF_vect)
{
  // Set 1 second as a detected duty
  if(airqualitysensor.counter==60)
  {
      airqualitysensor.last_vol=airqualitysensor.first_vol;
      airqualitysensor.first_vol=analogRead(A0);     
      airqualitysensor.counter=0;
      airqualitysensor.timer_index=1;
      PORTB=PORTB^0x20;
  } 
  else
  {
      airqualitysensor.counter++;
  }
}

// Get values from the DHT sensor and serially send data.
void read_DHT()
{
    // Read values.
    int humidity = dht.readHumidity();
    int temp = dht.readTemperature();
    
    // Check if values read are valid.
    if (!isnan(temp) and !isnan(humidity)) 
    {
        // Send data serially. 
        Serial.print(humidity);
        Serial.print(",");
        Serial.print(temp);
        Serial.print(",");
    }
}

// Get values from the ping sensor.
int read_Ping()
{
  // Pulse trigPin 
  // Sets trigPin on LOW for 10 microseconds.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets trigPin on HIGH for 10 microseconds.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  
  // Back to LOw.
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in centimetres rounded down - using the known speed.
  int distance = duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  return distance;
}


/*  Automatic Grid Traversal

    The system moves in the x-direction by one step.
    Keeping the x-coordinate fixed, it spans through the y-dimension, step-by-step.
    At each of these y-steps, we stop to take sensor readings by dropping the probe in the z-direction, all the way down.
    We repeat the above until the grid is traversed entirely, alternating the y-direction everytime.
*/
void automatic_traversal()
{
    bool y_direction = true;
    int y_start;
    int y_end;
    int y_actual_step;
    for (int x = x_current; x < gridlimits[0]; x += x_automatic_step)
    {
        go_to_coordinates (x, y_current, z_current);
        // go_to_coordinates (x_current, y_current, z_current);
         
        if(y_direction)
        {
           y_start = y_current;
           y_end = gridlimits[1];
           y_actual_step = y_automatic_step;
        }
        else
        {
           y_start = gridlimits[1] - y_automatic_step;
           y_end = -y_automatic_step;
           y_actual_step = -y_automatic_step;
        }
        
        for (int y = y_start; y != y_end; y += y_actual_step)
        {
              // Go to next y-position
              go_to_coordinates (x_current, y, 0);
              // go_to_coordinates (x_current, y_current, 0);


              // Monitor with probe, stopping at the bottom for 2 seconds
              monitor_current_point(2);
              
              // Return if we have new serial data
              if(Serial.available() >= 16)
                 return;
        }
        
        // Reverse direction for the next x-position.
        y_direction = not y_direction;
    }
}

/*  Manual Mode - Point Monitoring 
    
    Send the probe down at this position.
    Take readings, which are sent serially.
    Bring the probe back up.
*/

void monitor_current_point(uint8_t seconds)
{     
  // Send probe down. 
  send_down();
  
  // Delay to collect data and send serially.
  delay (1000*(int)seconds);
  read_sensors_and_send();

  // Get probe back up.
  send_up();     
}

// Send probe down until distance limit reached.
void send_down()
{
    // Keep checking ping sensor values when descending
    while(read_Ping() > z_distance_limit and z_current < gridlimits[2])
    {
        z_move(1);
    }
}

// Bring probe up.
void send_up()
{
    go_to_coordinates (x_current, y_current, 0);
}


void loop()
{
    // Read serial data to decide automatic or manual traversal.
    // Serial data will be sent as (automatic_mode, x, y, z)
    
    while (Serial.available() < 17);

    bool automatic_mode = Serial.readStringUntil(',').charAt(0) - '0';
        
    int x_input = Serial.readStringUntil(',').toInt();
    int y_input = Serial.readStringUntil(',').toInt();
    int z_input = Serial.readStringUntil('\n').toInt();
   
    current_mode = automatic_mode;

    Serial.print("Sent Arduino coordinates: x = ");
    Serial.print(x_input);
    Serial.print(" y = ");
    Serial.print(y_input);
    Serial.print(" z = ");
    Serial.print(z_input);
    Serial.println();
    
    if(current_mode)
    { 
        // Traverse the entire grid automatically.
        automatic_traversal();
        
        // Go back to the start.
        go_to_coordinates(0, 0, 0);
    }
    else
    {   // Check if input is in valid range.
        if ((x_input >= 0 and x_input <= gridlimits[0]) and (y_input >= 0 and y_input <= gridlimits[1]) and (z_input >= 0 and z_input <= gridlimits[2]))
        {
            // Go to the specified coordinates.
            go_to_coordinates(x_input, y_input, z_input);
            
            // Send probe down, take readings and send them back.
            monitor_current_point(2);
        }
        else
        {
            Serial.println("Invalid range.");
        }
    }
}
