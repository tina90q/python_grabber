#include <ZaberAscii.h>

/* This example program demonstrates doing a scan over a two dimensional grid,
 *  stopping briefly at each grid point and calling a user-defined function, which
 *  could be changed to operate some sort of third-party equipment such as a sensor
 *  or pipette.
 * 
 * The program does the traversal in boustrophedon order, meaning that each row 
 * reverses direction relative to the previous one. This minimizes total distance
 * travelled.
 * 
 * Edit the constants below to define the addressing information for the axes,
 * their unit conversion factors, and the start position and relative spacing
 * of the grid points. The example values given below assume you are using an
 * X-MCB2 controller at address 1 with an ASR100B120B two-axis stage attached.
 */

// Specify the device number and axis number for the X and Y axes of the scan.
const int X_DEVICE_ADDRESS = 1;
const int Y_DEVICE_ADDRESS = 1;
const int X_AXIS_NUMBER = 1;
const int Y_AXIS_NUMBER = 2; 

// For unit conversions, what is the microstep size in micrometers?
// This information can be found in your product manual.
const float X_MICROSTEP_SIZE = 0.15625;
const float Y_MICROSTEP_SIZE = 0.15625;

// Grid definition - what is the location of the first grid point relative to 
// the home positions of the two axes? Specified in millimeters.
const int X_FIRST_POINT = 10;
const int Y_FIRST_POINT = 10;

// Grid definition - what is the spacing of sample points in mm?
const int X_POINT_SPACING = 5;
const int Y_POINT_SPACING = 5;

// Grid definition - how many points do we want to sample on each axis?
const int X_NUM_POINTS = 4;
const int Y_NUM_POINTS = 3;

// Change this to false to do a raster scan instead of reversing direction.
const bool boustrophedon = true;


// Now the actual program.

ZaberShield shield(ZABERSHIELD_ADDRESS_AA);
ZaberAscii za(shield);

// Calculation helpers to figure out how many microsteps make up a millimeter,
// since this example does all measurements in millimeters.
#define X_uSTEPS(mm) (long int)((mm) * 1000.0 / X_MICROSTEP_SIZE)
#define Y_uSTEPS(mm) (long int)((mm) * 1000.0 / Y_MICROSTEP_SIZE)


// Helper to report errors.
bool check_error(ZaberAscii::reply reply)
{
  if (reply.isRejected)
  {
    Serial.println("Error: device " + String(reply.deviceNumber) + " rejected a command.");
    return true;
  }

  return false;
}


// This is the function that will be called at each grid point
// to do user-defined work there.
bool do_work(int gx, int gy)
{
  // Read the current device positions.
  long x,y;
  za.send(X_DEVICE_ADDRESS, X_AXIS_NUMBER, "get pos");
  ZaberAscii::reply reply = za.receive();
  if (check_error(reply)) { return true; }
  x = reply.responseData;
  
  za.send(Y_DEVICE_ADDRESS, Y_AXIS_NUMBER, "get pos");
  reply = za.receive();
  if (check_error(reply)) { return true; }
  y = reply.responseData;
  
  Serial.println("Paused at grid point (" + String(gx) + ", " + String(gy) + "). Device positions are (" + String(x) + ", " + String(y) + ").");
  delay(250);

  return false;
}


void setup() 
{
    shield.begin(115200);
    
    // This example uses the serial monitor for output and a Zaber shield 
    // for device interfacing at the same time. This conbination makes
    // debugging easier.
    Serial.begin(9600);
    Serial.println("Homing...");

    // Home both axes. This only needs to be done once per scan.
    za.send(X_DEVICE_ADDRESS, X_AXIS_NUMBER, "home");
    if (check_error(za.receive())) { return; } // Exit early if there is an error.
    za.send(Y_DEVICE_ADDRESS, Y_AXIS_NUMBER, "home");
    if (check_error(za.receive())) { return; }

    // Wait for the homing operations to finish.
    za.pollUntilIdle(X_DEVICE_ADDRESS);
    za.pollUntilIdle(Y_DEVICE_ADDRESS);

    // These variables will keep track of which direction the X axis is scanning.
    int x_start = 0;
    int x_end = X_NUM_POINTS;
    int x_increment = 1;

    // Nested loops to perform the scan.
    // For this example we want the X axis to move most frequently, so its loop
    // is inside the Y axis loop.
    for (int y = 0; y < Y_NUM_POINTS; y++)
    {
      // Calculate Y position and move Y axis there.
      long int yTarget = Y_uSTEPS(Y_FIRST_POINT + y * Y_POINT_SPACING);
      za.send(Y_DEVICE_ADDRESS, Y_AXIS_NUMBER, "move abs", yTarget);
      if (check_error(za.receive())) { return; }
      // The poll until idle for the Y axis happens inside the X loop so the moves will overlap.
      
      for (int x = x_start; x != x_end; x += x_increment)
      {
        // Calculate X position and move X axis there.
        long int xTarget = X_uSTEPS(X_FIRST_POINT + x * X_POINT_SPACING);
        za.send(X_DEVICE_ADDRESS, X_AXIS_NUMBER, "move abs", xTarget);
        if (check_error(za.receive())) { return; }
        za.pollUntilIdle(X_DEVICE_ADDRESS);

        // Also make sure the Y axis has stopped moving.
        za.pollUntilIdle(Y_DEVICE_ADDRESS);
        
        // Do work at the current grid point.
        if (do_work(x, y))
        {
            // Exit early if there is an error.
            return;
        }
      }

      if (boustrophedon)
      {
        // Reverse X direction for the next pass.
        x_increment = -x_increment;
        if (x_increment > 0)
        {
          x_start = 0;
          x_end = X_NUM_POINTS;
        }
        else
        {
          x_start = X_NUM_POINTS - 1;
          x_end = -1;
        }
      }
    }

    Serial.println("Scan complete!");
}

void loop() 
{
  // This function is empty because this program only makes one pass over the grid.
  // If we wanted to do it repeatedly we could put the grid scan code in here instead.
}

