#include <SparkFunBQ27441.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define   CONTRAST_PIN   9
#define   BACKLIGHT_PIN  7
#define   CONTRAST       110
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Settings of battery
const unsigned int BATTERY_CAPACITY = 2500; // 2000mAh battery

// Global variables
unsigned int full_capacity;

void setup()
{
  Serial.begin (115200);

  // Switch on the backlight and LCD contrast levels
  pinMode(CONTRAST_PIN, OUTPUT);
  analogWrite ( CONTRAST_PIN, CONTRAST );

  //lcd.setBacklightPin ( BACKLIGHT_PIN, POSITIVE );
  //lcd.setBacklight ( HIGH );
  lcd.backlight();
    
  lcd.begin(16,2);               // initialize the lcd 

  // Configure battery monitor

  setupBQ27441();

}

void loop()
{
  // Read battery stats
  unsigned int soc = getSOC();
   lcd.home ();                   // go home
  lcd.print("Charge Level:");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (String(soc)+"%");
    delay (50);
    lcd.print("");
}

void setupBQ27441(void)
{
  if (!lipo.begin())
  {
    Serial.println("Error: Unable to communicate with BQ27441.");
    Serial.println("  Check wiring and try again.");
    Serial.println("  (Battery must be plugged into Battery Babysitter!)");
    while (1) ;
  }
  Serial.println("Connected to BQ27441!");

  lipo.setCapacity(BATTERY_CAPACITY);
  full_capacity = lipo.capacity(FULL);
  Serial.print("Full charge capacity set to: ");
  Serial.println(String(full_capacity) + " mAh");
}

unsigned int getSOC()
{
  // Get battery stats function
  unsigned int soc = lipo.soc();
  unsigned int mv = lipo.voltage();
  int current = lipo.current(AVG);
  unsigned int capacity = lipo.capacity(REMAIN);
  full_capacity = lipo.capacity(FULL);

  // Print stats
  String toPrint = String(soc) + "% | ";
  toPrint += String(mv) + " mV | ";
  toPrint += String(current) + " mA | ";
  toPrint += String(capacity) + " / ";
  toPrint += String(full_capacity) + " mAh";
  
  Serial.println(toPrint);

  return soc;
}

