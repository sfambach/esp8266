/**The MIT License (MIT)
Copyright (c) 2015 by Daniel Eichhorn
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
See more at http://blog.squix.ch
*/

#include <simpleDSTadjust.h>
//#include <TZ.h>

// Config mode SSID
const String CONFIG_SSID = "@AZSMZ_TFT";

// Setup
String WIFI_SSID = "NODES";
String WIFI_PASS = "**************";

int UPDATE_INTERVAL_SECS= 10 * 60; // Update every 10 minutes
int SAVER_INTERVAL_SECS = 50;   // Going to screen saver after idle times, set 0 for dont screen saver.
int SLEEP_INTERVAL_SECS = 120;   // Going to Sleep after idle times, set 0 for dont sleep.

#define SQUIX         10
#define AZSMZ_1_1     11
#define AZSMZ_1_6     16    // AZSMZ TFT ver 1.6 (have touchpad or no touchpad)

//#define BOARD SQUIX
//#define BOARD AZSMZ_1_1
#define BOARD AZSMZ_1_6

#if BOARD == SQUIX
  // Pins for the ILI9341
  #define TFT_DC D2
  #define TFT_CS D1
  #define TFT_LED D8
  
  #define HAVE_TOUCHPAD
  #define TOUCH_CS D3
  #define TOUCH_IRQ  D4
    
#elif BOARD == AZSMZ_1_1
  #define TFT_DC 5
  #define TFT_CS 4
  #define TFT_LED 16
  #define TFT_LED_LOW       // set LOW to Turn on;

  #define BTN_1 0

  #define LM75
  #define SDA_PIN 0
  #define SCL_PIN 2  
  // LM75A Address
  #define Addr 0x48  
  #define BATT

#elif BOARD == AZSMZ_1_6
  #define TFT_DC 0
  #define TFT_CS 2
  #define TFT_LED 16
  #define TFT_LED_LOW       // set LOW to Turn on;

  #define HAVE_TOUCHPAD
  #define TOUCH_CS 5
  #define TOUCH_IRQ 4

  #define BTN_1 0
  #define BATT
  #define NTC
  #define nominalResistance 10   // NTC 10K
  #define bCoefficient 3950      // B 3950
  #define TEMPERATURENOMINAL 25  
  #define serialResistance 10
  
#endif

// Wunderground Settings
// To check your settings first try them out in your browser:
// http://api.wunderground.com/api/WUNDERGROUND_API_KEY/conditions/q/WUNDERGROUND_COUNTTRY/WUNDERGROUND_CITY.json
// e.g. http://api.wunderground.com/api/808b********4511/conditions/q/CH/Zurich.json
// e.g. http://api.wunderground.com/api/808b********4511/conditions/q/CA/SAN_FRANCISCO.json <- note that in the US you use the state instead of country code

String DISPLAYED_CITY_NAME = "Sulzbach";
String WUNDERGRROUND_API_KEY = "5f5d67b8f03745ea9d67b8f03705ea37";
String WUNDERGRROUND_LANGUAGE = "DE";
String WUNDERGROUND_COUNTRY = "DE";
String WUNDERGROUND_CITY = "Sulzbach";

#define UTC_OFFSET + 1
struct dstRule StartRule = {"CEST", Last, Sun, Mar, 2, 3600}; // Central European Summer Time = UTC/GMT +2 hours
struct dstRule EndRule = {"CET", Last, Sun, Oct, 2, 0};       // Central European Time = UTC/GMT +1 hour

// Settings for Boston
// #define UTC_OFFSET -5
// struct dstRule StartRule = {"EDT", Second, Sun, Mar, 2, 3600}; // Eastern Daylight time = UTC/GMT -4 hours
// struct dstRule EndRule = {"EST", First, Sun, Nov, 1, 0};       // Eastern Standard time = UTC/GMT -5 hour

// values in metric or imperial system?
bool IS_METRIC = true;

// Change for 12 Hour/ 24 hour style clock
bool IS_STYLE_12HR = false;

// change for different ntp (time servers)
//#define NTP_SERVERS "0.ch.pool.ntp.org", "1.ch.pool.ntp.org", "2.ch.pool.ntp.org"
#define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"

/***************************
 * End Settings
 **************************/
