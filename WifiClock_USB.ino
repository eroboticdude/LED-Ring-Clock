/*
   Elliott Bradley
   Last Edited 9/10/23

   Used code from:
    https://www.instructables.com/Desktop-Ring-Clock/
  https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/


*/
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <FastLED.h>

#define FASTLED_INTERRUPT_RETRY_COUNT 1
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LED_PIN 16
#define NUM_LEDS 60
CRGB leds[NUM_LEDS];
CRGB HourColor = CRGB(145, 0, 0);      //RED               //FastLED config
CRGB MinuteColor = CRGB(0, 118, 138);  //GREEN
CRGB SecondColor = CRGB(144, 0, 112);  //BLUE
const CRGB lightcolor(8, 5, 1);
uint8_t BRIGHTNESS = 255;

CRGBPalette16 currentPalette;

const int wifiTimeout = 8000;  //8 seconds
bool wifiCast;

String defaultSSID = "mnrbradley";    // WiFi name
String defaultPassword = "Kr1st1n@";  // WiFi password


const char* ssidAP = "Ring_Clock";  // Soft AP
const char* passwordAP = "123456789";
// IP Address details
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);  //soft AP data
IPAddress subnet(255, 255, 255, 0);

// Set web server port number to 80
AsyncWebServer server(80);

// Variable to store the HTTP request
String header;

String tempStr;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "north-america.pool.ntp.org", -18000);  // -5 hours offset, Chicago Time

const int modeCount = 4;
int lightMode = 1;
int hour = 10;
int minute = 30;
int second = 0;

/*
int touchButton;
int touchButtonLast;
int touchThreshold = 20;
*/

byte temp_second = 0;

TaskHandle_t Main;
TaskHandle_t Server;
TaskHandle_t AP;

void lightSpin(int8_t blinkCount, CRGB color, int speed = 20);  //need declaration for default values
void showDigit(String digits, int counter = 1500, CRGB color = CRGB::White);



void setup() {

  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println(F("An Error has occurred while mounting SPIFFS"));
    return;
  }

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  tempStr = readFile(SPIFFS, "/inputBrightness.txt");  //read brightness stored in file
  uint8_t readBrightness = tempStr.toInt();            //convert to int
  FastLED.setBrightness(readBrightness);
  Serial.print(F("FastLed Setup done on core "));
  Serial.println(xPortGetCoreID());
  vTaskDelay(100 / portTICK_PERIOD_MS);

  WiFi.mode(WIFI_STA);


  //WiFi.begin(ssid, password);
  WiFi.begin(readFile(SPIFFS, "/wifiSSID.txt").c_str(), readFile(SPIFFS, "/wifiPSK.txt").c_str());


  int wifiCount = millis();
  int wifiPrev = wifiCount;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("|");
    //softtwinkles();
    pride();
    FastLED.show();
    wifiCount = millis();
    if (wifiCount - wifiPrev >= wifiTimeout) {
      Serial.println(F("|"));
      Serial.println(F("WiFi not available, switching to default SSID & password"));
      WiFi.begin(defaultSSID, defaultPassword);
      wifiCount = millis();
      wifiPrev = wifiCount;
      lightSpin(1, CRGB::Yellow, 10);
      break;
      //if successful, save default ssid to spiffs to speed up connection?
    }
  }

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(F("."));
    //softtwinkles();
    pride();
    FastLED.show();
    wifiCount = millis();
    if (wifiCount - wifiPrev >= wifiTimeout) {  //if timeout
      wifiCast = true;
      timeFromFile();
      second = 0;
      Serial.println(F("."));
      Serial.println(F("WiFi not available, switching to Access Point mode"));
      Serial.print(F("wifiCast "));
      Serial.println(wifiCast);
      WiFi.mode(WIFI_AP);
      lightSpin(2, CRGB::Red, 20);  //Blink leds red to indicate not connected (kinda)
      goto tasks;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("!"));
    Serial.println(F("WiFi connected."));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("connected with core "));
    Serial.println(xPortGetCoreID());
    Serial.print(F("time taken "));
    Serial.println(wifiCount - wifiPrev);
    //show last 3 digits of IP
    showDigit(ipShort((WiFi.localIP().toString()), 3));  //I did it this way so in the settings there can be an option to show the entire IP, just in case
  }

tasks:
  //serverSetup();
  if (WiFi.status() != WL_CONNECTED)  //if not connected, host offline server
  {
    xTaskCreatePinnedToCore(APcode, "AP", 4096, NULL, 1, &AP, 0);  //runs the async server on core 1
  }

  xTaskCreatePinnedToCore(serverSetup, "Server", 4096, NULL, 0, &Server, 0);  //execute serverSetup on core 0 with priority 0  MAYBE TRY RUNNING ON CORE 1


  //xTaskCreatePinnedToCore(Maincode, "Main", 50000, NULL, 1, &Main, 1); //MOVED TO END OF "Server" FUNCTION to not crash(using wifi and fastLED on the same core can crash)
}

void loop() {
  delay(1);  //to hopefully reduce processing times if the loop is considered
}


void Maincode(void* parameter) {

  for (;;)  //loop forever
  {
    if (wifiCast == false) {
      if (temp_second != timeClient.getSeconds()) {  //update the temp_second only when the time client second changes
        temp_second = timeClient.getSeconds();
        timeClient.update();  //this might be redundant, but im not sure if the timeClient.get commands update the client or not (pretty sure it does)
        Serial.print(F("24 hr time "));
        Serial.println(timeClient.getFormattedTime());
        Serial.print(F("looped on "));
        Serial.println(xPortGetCoreID());
        hour = timeClient.getHours();
        minute = timeClient.getMinutes();
        second = timeClient.getSeconds();
        //EEPROM.write so it saves current internet time for if the esp reboots offline?
      }
    }
    if (wifiCast == true) {

      EVERY_N_SECONDS(1) {  //this should replace my timer code to make it look pretty
        second += 1;
        Serial.print(F("Offline time "));
        Serial.print(hour);
        Serial.print(F(":"));
        Serial.print(minute);
        Serial.print(F("."));
        Serial.println(second);

        if (second >= 59) {
          second = 0;
          minute += 1;
        }
        if (minute >= 60) {
          minute = 0;
          hour += 1;
        }
        if (hour >= 24) {
          hour = 0;
        }
      }
      EVERY_N_SECONDS(60) {
        String timeStore;
        String modeStore;
        timeStore = String(int(hour) + ":" + int(minute + 1));  //use String to prevent corruption
        modeStore = String(lightMode);
        writeFileString(SPIFFS, "/inputTime.txt", timeStore);  //save current time to spiffs
        writeFile(SPIFFS, "/lightMode.txt", modeStore.c_str());
        //WORK ON CONVERTING SAVING SINGLE VALUES TO NVS INSTEAD OF SPIFFS
        //https://github.com/espressif/arduino-esp32/blob/master/libraries/Preferences/examples/Prefs2Struct/Prefs2Struct.ino
      }
    }

    //buttonRead();  //not implemented or used currently

    switch (lightMode) {
      case 1:
        timeDisplay1(hour, minute, second);
        break;

      case 2:
        timeDisplay2(hour, minute, second);
        break;

      case 3:
        timeDisplay3(hour, minute, second);
        break;

      case 4:
        timeDisplay4(hour, minute, second);
        break;
        /*
      case 5:
        timeDisplay5(hour, minute, second); //mode is broken
        break;
*/
      default:
        Serial.print(F("timeDisplay Error!!!!!"));
        lightMode = modeCount;
        break;
    }
    ledOffset();
    FastLED.show();
  }
}

String ipShort(String ip, int8_t digitsFromEnd) {
  String cutIP;
  cutIP = ip.substring(ip.length() - digitsFromEnd);
  Serial.println(cutIP);
  return (cutIP);
}


/*
void buttonRead() {
  int touch;
  touch = touchRead(T0);

  if (touch < touchThreshold) {
    touchButton = 1;
  } else {
    touchButton = 0;
  }

  if (touchButton == 0 && touchButton != touchButtonLast)  //senses falloff, if last cycle button was pressed and it's no longer pressed, increment
  {
    lightMode++;
    if (lightMode > modeCount) {
      lightMode = 1;
    }
    String modeStore = String(lightMode);
    writeFile(SPIFFS, "/lightMode.txt", modeStore.c_str());
  }
  touchButtonLast = touchButton;


  if (touchButton == 1) {
    Serial.print(F("touch value "));
    Serial.println(touch);
    Serial.print(F("button value "));
    Serial.println(touchButton);
    Serial.print(F("light mode "));
    Serial.println(lightMode + 1);
    Serial.print(F("last button state "));
    Serial.println(touchButtonLast);
  }
}
*/