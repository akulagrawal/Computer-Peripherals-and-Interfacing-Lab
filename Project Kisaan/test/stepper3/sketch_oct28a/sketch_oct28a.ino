const uint8_t STEPPER_x1_PIN[4] = {22, 24, 26, 28};
const uint8_t STEPPER_x2_PIN[4] = {33, 35, 37, 39};
const uint8_t STEPPER_y_PIN[4]  = {42, 44, 46, 48};
const uint8_t STEPPER_z_PIN[4]  = {17, 18, 19, 20};
const uint8_t STOPPER_x_PIN = 51;
const uint8_t STOPPER_y_PIN = 53;

// Calibration of these need to be done according to the mesh size and motor rotation
const int x_step = 5;
const int y_step = 5;
int grid [3] = {100, 100, 10};

int x_current = 0;
int y_current = 0;
int z_current = 0;
bool automatic_mode = false;


// void get_context ()
// {
//     // Get the old coordinates of controller
//
// }
void save_context ()
{
    Serial.print (x_current);
    Serial.print (y_current);
    Serial.print (z_current);
    Serial.println();
}

void x_move (int step)
{
    x_current += step;

    for (int pos = 0; pos < 4; pos++)
    {
        // Set orientations of magnets
        for(int pin = 0; pin < 4; ++pin)
        {
            if(pin == pos)
            {
                if (step == 1)
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
                if (step == 1)
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
        delay (2);
    }
}
void y_move (int step)
{
    y_current += step;

    for (int pos = 0; pos < 4; pos++)
    {
        // Set orientations of magnets
        for(int pin = 0; pin < 4; ++pin)
        {
            if(pin == pos)
            {
                if (step == 1)
                    digitalWrite(STEPPER_y_PIN[pin], HIGH);
                else
                    digitalWrite(STEPPER_y_PIN[3 - pin], HIGH);
            }
            else
            {
                if (step == 1)
                    digitalWrite(STEPPER_y_PIN[pin], LOW);
                else
                    digitalWrite(STEPPER_y_PIN[3 - pin], LOW);
            }
        }
        delay (2);
    }
}
void z_move (int step)
{
    z_current += step;

    for (int pos = 0; pos < 4; pos++)
    {
        // Set orientations of magnets
        for(int pin = 0; pin < 4; ++pin)
        {
            if(pin == pos)
            {
                if (step == 1)
                    digitalWrite(STEPPER_z_PIN[pin], HIGH);
                else
                    digitalWrite(STEPPER_z_PIN[3 - pin], HIGH);
            }
            else
            {
                if (step == 1)
                    digitalWrite(STEPPER_z_PIN[pin], LOW);
                else
                    digitalWrite(STEPPER_z_PIN[3 - pin], LOW);
            }
        }
        delay (2);
    }
}
void go_to_coordinates (int x_final, int y_final, int z_final)
{
    while ( x_current != x_final ||
            y_current != y_final ||
            z_current != z_final )
    {
        if (Serial.available())
            break;

        if (x_current < x_final)
            x_move (1);
        else if (x_current > x_final)
            x_move (-1);

        if (y_current < y_final)
            y_move (1);
        else if (y_current > y_final)
            y_move (-1);

        if (z_current < z_final)
            z_move (1);
        else if (z_current > z_final)
            z_move (-1);

        save_context();
    }
}
void go_to_start ()
{
    while (true)
    {
        // Reached boundary - x axis
        if (digitalRead (STOPPER_x_PIN) == HIGH)
            break;
        else
            x_move(-1);
    }
    while (true)
    {
        // Reached boundary - y axis
        if (digitalRead (STOPPER_y_PIN) == HIGH)
            break;
        else
            y_move(-1);
    }
    x_current = 0;
    y_current = 0;
    z_current = 0;
}
void automatic_traversal ()
{
    /*
    *   y axis motion [0 to extreme, step wise] :
    *       In each step of y, z axis motion [0 to extreme] (collect data and come back)
    *   x axis motion [1 step]
    *   y axis motion [extreme to 0, step wise] :
    *       In each step of y, z axis motion [0 to extreme] (collect data and come back)
    *   x axis motion [1 step]
    *   Repeat
    */

    bool pos_direction = true;
    for (int x = 0; x < grid[0]; x += x_step)
    {
        go_to_coordinates (x, y_current, z_current);
        for (int y = 0; y < grid[1]; y += y_step)
        {
            if(pos_direction)
                go_to_coordinates (x_current, y, z_current);
            else
                go_to_coordinates (x_current, grid[1] - y - y_step, z_current);

            go_to_coordinates (x_current, y_current, grid[2]);
            /* Perform collection of data here */
            delay (5000);
            go_to_coordinates (x_current, y_current, 0);
        }
    }
}
void setup()
{
    for (uint8_t i = 0; i < 4; i++)
    {
        pinMode(STEPPER_x1_PIN[i], OUTPUT);
        pinMode(STEPPER_x2_PIN[i], OUTPUT);
        pinMode(STEPPER_y_PIN[i], OUTPUT);
        pinMode(STEPPER_z_PIN[i], OUTPUT);
    }
    pinMode(STOPPER_x_PIN, INPUT);
    pinMode(STOPPER_y_PIN, INPUT);

    // get_context ();
    // Go to start position
    // go_to_start();
}
/* ********************
    Two mode :
        Manual Mode
        Automatic Mode
* *********************/
void loop()
{
    // Serial data will come in order of (automatic_mode, x, y, z)
    // Read serial data
    while (Serial.available() < 4);

    automatic_mode = Serial.parseInt();
    int x_input = Serial.parseInt();
    int y_input = Serial.parseInt();
    int z_input = Serial.parseInt();

    if (automatic_mode)
    {
        automatic_traversal ();
        // Just to stop the controller on the other end
        delay (100000);
    }
    else
    {
        go_to_coordinates(x_input, y_input, z_input);
    }
}
