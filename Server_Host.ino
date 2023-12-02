
void APcode(void* parameter) {
  //setup for server
  WiFi.softAP(ssidAP, passwordAP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  Serial.println(local_ip);
  vTaskDelete(NULL);
}

/*
  String yourInputString = readFile(SPIFFS, "/inputString.txt");
  Serial.print("*** Your inputString: ");
  Serial.println(yourInputString);

  int yourInputInt = readFile(SPIFFS, "/inputInt.txt").toInt();
  Serial.print("*** Your inputInt: ");
  Serial.println(yourInputInt);

  float yourInputFloat = readFile(SPIFFS, "/inputFloat.txt").toFloat();
  Serial.print("*** Your inputFloat: ");
  Serial.println(yourInputFloat);
  delay(5000);


  void STAcode( void * parameter){ // only use if the offline functions are too different
  }

*/

const char* PARAM_SSID = "inputSSID";  //not sure how this works but its like a flag that point to the html inputs
const char* PARAM_PSK = "inputPSK";    //this potentially is the title of the form blocks and the code looks for this string specifically
const char* PARAM_TIME = "inputTime";
const char* PARAM_BRIGHTNESS = "inputBrightness";
const char* PARAM_MODE = "lightMode";
const char* PARAM_DARKTOGGLE = "darkToggle";
const char* PARAM_DARKSTART = "darkStart";
const char* PARAM_DARKEND = "darkEnd";
const char* PARAM_DARKMODE = "darkmodeBrightness";



// HTML web page to handle 3 input fields (inputString, inputTime, inputFloat)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved value to ESP SPIFFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>
  <form action="/get" target="hidden-form">
    inputSSID (current value %inputSSID%): <input type="text" name="inputSSID"/>  
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputPSK (current value %inputPSK%): <input type="text" name="inputPSK"/>
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputTime (current time %inputTime%): <input type="time" name="inputTime" value="%inputTime%"/>
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputBrightness (current value %inputBrightness%): <input type="range" name="inputBrightness" value="%inputBrightness%" min="0" max="255" step="1"/>
  <input type="submit" value="Submit" onclick="submitMessage()">     
  </form><br>
  <form action="/get" target="hidden-form">
    lightMode (current value %lightMode%): <input type="range" name="lightMode" value="%lightMode%" min=1 max="%lightMax%" step="1"/>
  <input type="submit" value="Submit" onclick="submitMessage()">     
  </form><br>
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";


void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}





void serverSetup(void* parameter)  // (void * parameter) needed for the esp core tasks??
{
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;

    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>

    if (request->hasParam(PARAM_SSID)) {
      inputMessage = request->getParam(PARAM_SSID)->value();  //somehow this gets the param from the main code and depending on which, it chooses between these save locations
      writeFile(SPIFFS, "/wifiSSID.txt", inputMessage.c_str());
    }

    else if (request->hasParam(PARAM_PSK)) {
      inputMessage = request->getParam(PARAM_PSK)->value();
      writeFile(SPIFFS, "/wifiPSK.txt", inputMessage.c_str());
    }

    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_TIME)) {
      inputMessage = request->getParam(PARAM_TIME)->value();
      writeFileString(SPIFFS, "/inputTime.txt", inputMessage);
      timeFromString(inputMessage);
    }

    // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
    else if (request->hasParam(PARAM_BRIGHTNESS)) {
      inputMessage = request->getParam(PARAM_BRIGHTNESS)->value();
      writeFile(SPIFFS, "/inputBrightness.txt", inputMessage.c_str());
      FastLED.setBrightness(inputMessage.toInt());
    }

    else if (request->hasParam(PARAM_MODE)) {
      inputMessage = request->getParam(PARAM_MODE)->value();
      writeFile(SPIFFS, "/lightMode.txt", inputMessage.c_str());
      lightMode = inputMessage.toInt();
    }

    else {
      inputMessage = "No message sent";
    }

    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();

  xTaskCreatePinnedToCore(Maincode, "Main", 50000, NULL, 1, &Main, 1);  //execute Maincode on core 1 with priority 1
  vTaskDelete(NULL);                                                    //delete server function so there are no conflicts

  //I did this because usually calling a function waits until it is over to continue, but these multicore functions mostly likely do not
}



String readFile(fs::FS& fs, const char* path) {
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}




void writeFile(fs::FS& fs, const char* path, const char* message) {
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void writeFileString(fs::FS& fs, const char* path, String message) {
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("- failed to open file for writing String");
    return;
  }
  if (file.print(message)) {
    Serial.println("- String file written");
  } else {
    Serial.println("- String write failed");
  }
  file.close();
}


// Replaces placeholder (% surr) with stored values
String processor(const String& var)  //var is the current value of the const char
{
  //Serial.println(var);
  if (var == "inputSSID") {
    return readFile(SPIFFS, "/wifiSSID.txt");
  } else if (var == "inputPSK") {
    return readFile(SPIFFS, "/wifiPSK.txt");
  } else if (var == "inputTime") {
    return readFile(SPIFFS, "/inputTime.txt");
  } else if (var == "inputBrightness") {
    return readFile(SPIFFS, "/inputBrightness.txt");
  } else if (var == "lightMode") {
    return readFile(SPIFFS, "/lightMode.txt");
  } else if (var == "lightMax") {  //lightMax is set to the amount of modes
    return String(modeCount);
  } else {
    return String();
  }
}


void timeFromString(String inputMessage) {
  String hourStr;
  String minuteStr;
  hourStr.concat(inputMessage.charAt(0));
  hourStr.concat(inputMessage.charAt(1));
  minuteStr.concat(inputMessage.charAt(3));
  minuteStr.concat(inputMessage.charAt(4));

  hour = hourStr.toInt();
  minute = minuteStr.toInt();
  //writeFile(SPIFFS, "/timeHour.txt", hourStr.c_str());
  //writeFile(SPIFFS, "/timeMinutes.txt", minuteStr.c_str());
  Serial.print(F("Time updated from String to "));
  Serial.println(hour, minute);
}

void timeFromFile() {
  String inputMessage;
  String hourStr;
  String minuteStr;
  inputMessage = readFile(SPIFFS, "/inputTime.txt");

  hourStr.concat(inputMessage.charAt(0));
  hourStr.concat(inputMessage.charAt(1));
  minuteStr.concat(inputMessage.charAt(3));
  minuteStr.concat(inputMessage.charAt(4));

  hour = hourStr.toInt();
  minute = minuteStr.toInt();
  //writeFile(SPIFFS, "/timeHour.txt", hourStr.c_str());
  //writeFile(SPIFFS, "/timeMinutes.txt", minuteStr.c_str());
  Serial.print(F("Time updated from file to "));
  Serial.println(hour, minute);
}
