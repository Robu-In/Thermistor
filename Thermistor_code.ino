#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED_PIN 13                            // defining pin 13 as the led input pin
#define DIGITAL_INPUT 3                   // defining digital input at pin 3
#define ANALOG_INPUT   A0            // defining analog input at A0
#define DIGITAL_SET_POINT 500

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// initializing the variables
int digital_output;                          // This will read the digital value
int analog_output;                        // This will read the analog value
int revised_output;                        // variable to store the corrected value
float temp_C;                                  // Variable for storing the temperature
float temp_f;                                  // Variable for storing the Fahrenheit

void setup()                                                                     // Anything written in it will run once.
{
  pinMode(LED_PIN, OUTPUT);                               // declaring pin 13 as output
  pinMode(DIGITAL_INPUT, INPUT);                   // declaring pin 3 as input
  pinMode(ANALOG_INPUT, INPUT);                 // declaring A0 as input pin

  Serial.begin(9600);                                            // selecting the baud rate at 9600
  Serial.println("Robu.in: The data is");

  // SSD1306 display initialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  // Clear the buffer
  display.clearDisplay();
}

void loop()                               // Anything written in it will run continuously
{
  analog_output = analogRead(ANALOG_INPUT);         // Reading the analog value and storing in analog_output

  // The module has thermistor connection reversed
  revised_output = map(analog_output, 0, 1023, 1023, 0);

  temp_C = Thermistor(revised_output);
  temp_f = (temp_C * 9.0) / 5.0 + 32.0;

  // Reading the digital data
  digital_output = digitalRead ( DIGITAL_INPUT ) ;

  Serial.print ( "   Digital value of the module is =  " ) ;

  Serial.println ( digital_output ) , DEC ;     // This will display the digital value on the display 

  Serial.print ( " LED is =" ) ;

  if ( analog_output < 450 )   // The LED will turn on When the sensor value will exceed the set point

  {

    digitalWrite ( LED_PIN, HIGH ) ;

    Serial.print ( "ON " ) ;
    delay(1000);

  }

  else

  {

    digitalWrite ( LED_PIN, LOW ) ;

    Serial.print ( "OFF " ) ;   

  }

  // Displaying temperature readings on the OLED display
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Temperature:");
  display.print(temp_f, 1); // Fahrenheit
  display.println("F");
  display.print(temp_C, 1); // Celsius
  display.println("C");
  display.display();

  Serial.print("Analog value of the module is = ");
  Serial.println(analog_output);

  Serial.print("Digital value of the module is = ");
  Serial.println(digital_output);

  digitalWrite(LED_PIN, digital_output); // Turn LED on or off based on digital output

  Serial.print("Measured Temperature = ");
  Serial.print(temp_f, 1);    // This will display the temperature in Fahrenheit
  Serial.print(" F  ");
  Serial.print(temp_C, 1);  // This will display the temperature in Celsius
  Serial.println(" C ");

  Serial.println();     // Leaving a blank line

  delay(1000);       // Wait for 1 second
}

double Thermistor(int RawADC)
{
  double Temp;
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;     // This will Convert Kelvin to Celsius
  return Temp;
}
