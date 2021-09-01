/* 
This is a sketch for sending WSPR signals in Ham radio bands, you are not allowed to use this if you are not an radio amateur.
There is a config.h file that could be used to enter your call sign and adjust the outputs of the SI5351.
The sketch is first reciving GPS coordinates, time  and calculate the current maidenhead. 
Then the SI5351 will be initilaized and after that the WSPR will send by active band and time slot.
 */

#include <Arduino.h>
#include <config.h>

// Simple date conversions and calculations
// Date and time functions using just software, based on millis() & timer

//-------------------------------------------------------------------------------------
// RTC clock
#include "RTClib.h"
RTC_DS3231 rtc;
char buf1[] = "hh:mm";
int oldMinute = 0;
char t_sz[32];
char d_sz[32];
//-------------------------------------------------------------------------------------
#include <rs_common.h>
#include <int.h>
#include <string.h>

//-------------------------------------------------------------------------------------
// I2C
#include <Wire.h>

//-------------------------------------------------------------------------------------
// Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define OLED_RESET 4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//-------------------------------------------------------------------------------------
// Geodata / Maidenhead
#include <TinyGPS++.h>
// The TinyGPS++ object
TinyGPSPlus gps;
TinyGPSTime t;
TinyGPSDate d;
TinyGPSInteger sats;
#include "../lib/maidenhead/maidenhead.h"
//-------------------------------------------------------------------------------------
// Time Plan
int resync = 0;
String currentBand;

//-------------------------------------------------------------------------------------
// SI5351 + JTEncode

#include <si5351.h>
#include <JTEncode.h>

//Si5351

Si5351 si5351;
//-------------------------------------------------------------------------------------
//JTEncode
JTEncode jtencode;
unsigned long freq;
uint8_t tx_buffer[255];
uint8_t symbol_count;
uint16_t tone_delay, tone_spacing;

#define LED_PIN 13

// Bands 
//-------------------------------------------------------------------------------------

struct Band{
  String name;
  long unsigned int freq;
  bool send;
  int relay;
  si5351_clock clk;
};

Band band_160m{"160m",WSPR_160m_FREQ,send_160m,RELAY_160m,OUTPUT_160m};
Band band_80m{"80m",WSPR_80m_FREQ,send_80m,RELAY_80m,OUTPUT_80m};
//Band band_80m2{"80m",WSPR_80m_FREQ2,send_80m,RELAY_80m,OUTPUT_80m};
Band band_60m{"60m",WSPR_60m_FREQ,send_60m,RELAY_60m,OUTPUT_60m};
Band band_40m{"40m",WSPR_40m_FREQ,send_40m,RELAY_40m,OUTPUT_40m};
Band band_30m{"30m",WSPR_30m_FREQ,send_30m,RELAY_30m,OUTPUT_30m};
Band band_20m{"20m",WSPR_20m_FREQ,send_20m,RELAY_20m,OUTPUT_20m};
Band band_17m{"17m",WSPR_17m_FREQ,send_17m,RELAY_17m,OUTPUT_17m};
Band band_15m{"15m",WSPR_15m_FREQ,send_15m,RELAY_15m,OUTPUT_15m};
Band band_12m{"12m",WSPR_12m_FREQ,send_12m,RELAY_12m,OUTPUT_12m};
Band band_10m{"10m",WSPR_10m_FREQ,send_10m,RELAY_10m,OUTPUT_10m};

//-------------------------------------------------------------------------------------
// DEBUG

#ifdef DEBUG
#define DEBUG_BEGIN(x) Serial.begin(x);
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_BEGIN(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#ifdef DEBUG_GPS
#define DEBUG_GPS_PRINT(x) Serial.print(x)
#define DEBUG_GPS_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_GPS_PRINT(x)
#define DEBUG_GPS_PRINTLN(x)
#endif

#ifdef _AVR
#define GPS_BEGIN(x)  Serial.begin(9600);
#define GPS_AVAILABLE(x) Serial.available()
#define GPS_READ(x) Serial.read()
#else
#define GPS_BEGIN(x)  Serial1.begin(9600);
#define GPS_AVAILABLE(x) Serial1.available()
#define GPS_READ(x) Serial1.read()
#endif
// Loop through the string, transmitting one character at a time.
void encode(si5351_clock clk)
{
  uint8_t i;

  // Reset the tone to the base frequency and turn on the output
  si5351.output_enable(clk, 1);
  digitalWrite(LED_PIN, HIGH);
  si5351.set_clock_pwr(clk,1);
  for (i = 0; i < symbol_count; i++)
  {
    //si5351.set_pll(SI5351_PLL_FIXED,SI5351_PLLA);
    si5351.set_freq((freq * 100) + (tx_buffer[i] * tone_spacing), clk);
    delay(tone_delay);
  }
  // Turn off the output
  si5351.set_clock_pwr(clk,0);
  si5351.output_enable(clk, 0);
  digitalWrite(LED_PIN, LOW);
}
void set_tx_buffer()
{
  // Clear out the transmit buffer
  memset(tx_buffer, 0, 255);
  jtencode.wspr_encode(call, locator, dbm, tx_buffer);
}

void initDisplay(){
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    DEBUG_PRINTLN(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  
  // Clear the buffer
  display.clearDisplay();
  delay(2000);
}

void printGPS(bool GPS)
{
  display.fillRect(0, 0, 24, 8, BLACK);
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  if (GPS)
  {
    display.println("GPS");
  }
  else
  {
    display.println("!GPS");
  }
  display.display();
}

void printBand(String band, bool draw)
{
  display.fillRect(44, 0, 24, 8, BLACK);
  if (draw)
  {
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(44, 0);            // Start at top-left corner
    display.println(band);
    display.display();
  }
}

void printTX(bool TX)
{
  display.fillRect(24, 0, 12, 8, BLACK);
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_BLACK); // Draw white text
  display.setCursor(24, 0);
  if (TX)
  {
    display.fillRect(24, 0, 12, 8, WHITE);
    // Start at top-left corner
    display.println("TX");
  }
  display.display();
}

void printTime(DateTime time)

{
  String minute = "";
  if (time.minute()>=10){
  minute = String(time.minute());
} else{
  minute = "0" + String(time.minute());
}
  String myTime = String(time.hour()) + ":"+minute;

  display.fillRect(98, 0, 30, 8, BLACK);
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(98, 0);            // Start at top-left corner
  display.println(myTime);
  display.display();
}

void printTemp()
{ 
  String tempString = String(rtc.getTemperature());
  display.fillRect(98, 8, 30, 8, BLACK);
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(98, 8);            // Start at top-left corner
  display.println(tempString);
  display.display();
  DEBUG_PRINT(tempString);
  DEBUG_PRINTLN("°C");
}

void printLocator()
{
  display.fillRect(92, 24, 30, 8, BLACK);
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(92, 24);           // Start at top-left corner
  display.println(locator);
  display.display();
}

void printText(String text, bool draw)
{
  display.fillRect(0, 24, 90, 8, BLACK);
  if (draw)
  {
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 24);            // Start at top-left corner
    display.println(text);
    display.display();
  }
}

void setup()
{
  DEBUG_BEGIN(115200);
  rtc.begin();
  GPS_BEGIN(9600);
  initDisplay();
  DateTime starttime = rtc.now();
  // TX
  //String time = starttime.toString(buf1);
  printGPS(false); 
  printTime(starttime);
  if(!POWERTEST){
  //printGPS(" GPS");
  DEBUG_PRINTLN("Wait for GPS");
  while (!gps.location.isValid())
  {
    // DEBUG_GPS_PRINTLN("No GPS");

    if (GPS_AVAILABLE() > 0)
    {
      if (gps.encode(GPS_READ()))
      {
        DEBUG_GPS_PRINT("Encodeded>> \t");
        String test = String(gps.time.hour());
        d = gps.date;
        t = gps.time;

        if (!d.isValid())
        {
          DEBUG_GPS_PRINT(F("********** "));
        }
        else
        {
          char sz[32];
          sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
          DEBUG_GPS_PRINT(sz);
        }
        DEBUG_GPS_PRINT("\t");
        if (!t.isValid())
        {

          DEBUG_GPS_PRINTLN(F("********s "));
        }
        else
        {
          char sz[32];
          sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
          DEBUG_GPS_PRINT(sz);
          DEBUG_GPS_PRINT("\t");
        }
        //delay(1000);
        //printText(test);
        DEBUG_GPS_PRINT("Lat: ");
        DEBUG_GPS_PRINT(gps.location.lat());
        DEBUG_GPS_PRINT("\tLong: ");
        DEBUG_GPS_PRINTLN(gps.location.lng());
        //DEBUG_GPS_PRINT("\tSats: ");
        //DEBUG_GPS_PRINTLN(gps.satellites.value());
        //DEBUG_GPS_PRINTLN(String(gps.satellites));
      }
    }
  }
  
  //jtencode.latlon_to_grid(float(gps.location.lat()), float(gps.location.lng()),locator);
  printGPS(true);
  locator = get_mh(gps.location.lat(), gps.location.lng(), 4);
  locator_full = get_mh(gps.location.lat(), gps.location.lng(), 6);
  DEBUG_PRINTLN(locator);
  printText("Time syncing", true);
  DEBUG_PRINTLN("Time syncing");

  //encode current GPS signal to get latest timestamp for RTC
  gps.encode(GPS_READ());

  d = gps.date;
  t = gps.time;

  //set RTC to date from GPS

  rtc.adjust(DateTime(d.year(), d.month(), d.day(), t.hour(), t.minute(), t.second()));
  DateTime now = rtc.now();

  // TX
  //String time = now.toString(buf1);
  printTime(now);
  DEBUG_PRINTLN("Time syncing");
  // Initialize the Si5351
  }
  printText("Start SI5351", true);
  DEBUG_PRINTLN("Start SI5351");

  // Set SI5351  and outputs 
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  si5351.set_correction(calibration,SI5351_PLL_INPUT_XO);
  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA); // Set for max power if desired
  si5351.output_enable(SI5351_CLK0, 0);            // Disable the clock initially
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA); // Set for max power if desired
  si5351.output_enable(SI5351_CLK1, 0);
  si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_8MA); // Set for max power if desired
  si5351.output_enable(SI5351_CLK2, 0);      
  si5351.set_clock_pwr(SI5351_CLK0,0);
  si5351.set_clock_pwr(SI5351_CLK1,0);
  si5351.set_clock_pwr(SI5351_CLK2,0);
  // Use the Arduino's on-board LED as a keying indicator.
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(RELAY_PIN0,OUTPUT);
  pinMode(RELAY_PIN1,OUTPUT);
  pinMode(RELAY_PIN2,OUTPUT);
  digitalWrite(RELAY_PIN0, LOW);
  digitalWrite(RELAY_PIN1, LOW);
  digitalWrite(RELAY_PIN2, LOW);
  symbol_count = WSPR_SYMBOL_COUNT; // From the library defines
  tone_spacing = WSPR_TONE_SPACING;
  tone_delay = WSPR_DELAY;

  // Encode the message in the transmit buffer
  set_tx_buffer();
  DEBUG_PRINTLN("Starting Transmisson mode...");

  //printText("TX ready");
  printText(call, true);
  printLocator();
  if(!POWERTEST){
  int delaying = 60 - (rtc.now().second());
  delay(delaying * 1000);}
  printTemp();
  
}

void send_wspr_struct(Band band)
{ if(band.send){
  freq = band.freq;
  printTX(true);
  printBand(band.name, true);
  digitalWrite(band.relay, HIGH);
  DEBUG_PRINTLN("Transmisson " + band.name + " started");
  encode(band.clk);
  digitalWrite(band.relay, LOW);
  printTX(false);
  printBand("", false);
  resync = 0;
  printTemp();
  DEBUG_PRINTLN("Transmisson  " + band.name + " ended");
  if(POWERTEST){
    delay(60000);
  }
}
}

void loop()
{
if(!POWERTEST){

  DateTime rtc_time = rtc.now();

if(rtc_time.minute() != oldMinute)
{
oldMinute = rtc_time.minute();

  //String myTime = String(rtc_time.hour()) + ":"+String(rtc_time.minute());
  printTime(rtc_time);
}

  if (rtc_time.second() == 00)
  {
// 160m   
if ((rtc_time.minute() == 00 || rtc_time.minute() == 20 ||  rtc_time.minute() == 40))
    {
      send_wspr_struct(band_160m);
    } 
// 80m 
    if ((rtc_time.minute() == 2 || rtc_time.minute() == 22 ||  rtc_time.minute() == 42))
    {
      //if(rtc_time.minute() == 42){
      //  send_wspr_struct(band_80m2);
     // }
      //else {
      send_wspr_struct(band_80m);
      //}
    }
// 60m    
if ((rtc_time.minute() == 4 || rtc_time.minute() == 24 ||  rtc_time.minute() == 44))
    {
      send_wspr_struct(band_60m);
    } 
// 40m    
if ((rtc_time.minute() == 6 || rtc_time.minute() == 26 ||  rtc_time.minute() == 46))
    {
      send_wspr_struct(band_40m);
    } 
// 30m  
if ((rtc_time.minute() == 8 || rtc_time.minute() == 28 ||  rtc_time.minute() == 48))
    {
      send_wspr_struct(band_30m);
    } 
// 20m 
  
if ((rtc_time.minute() == 10 || rtc_time.minute() == 30 ||  rtc_time.minute() == 50))
    {
      send_wspr_struct(band_20m);
    } 
// 17m   
if ((rtc_time.minute() == 12 || rtc_time.minute() == 32 ||  rtc_time.minute() == 52))
    {
      send_wspr_struct(band_17m);
    } 
// 15m 
if ((rtc_time.minute() == 14 || rtc_time.minute() == 34 ||  rtc_time.minute() == 54))
    {
      send_wspr_struct(band_15m);
    } 
// 12m  
if ((rtc_time.minute() == 16 || rtc_time.minute() == 36 ||  rtc_time.minute() == 56))
    {
      send_wspr_struct(band_12m);
    } 
// 10m 
    if ((rtc_time.minute() == 18 || rtc_time.minute() == 38 || rtc_time.minute() == 58))
    {
      send_wspr_struct(band_10m);
    }
  }
  // Sleep until next TX
  delay(500);
  }else{
    delay(1000);
      send_wspr_struct(band_160m);
      send_wspr_struct(band_80m);
      send_wspr_struct(band_60m);
      send_wspr_struct(band_40m);
      send_wspr_struct(band_30m);
      send_wspr_struct(band_20m);
      send_wspr_struct(band_17m);
      send_wspr_struct(band_15m);
      send_wspr_struct(band_12m);
      send_wspr_struct(band_10m);
  }
}