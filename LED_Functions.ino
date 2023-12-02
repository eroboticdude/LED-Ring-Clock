
void ledOffset() {  //copies the leds to effectively rotate the clock 180
  int offsetCount = 30;
  CRGB pData[NUM_LEDS];
  CRGB temp = leds[0];
  for (int i = 0; i < NUM_LEDS - 1; i++) {  //shifts leds one led counterclockwise
    leds[i] = leds[i + 1];
  }
  leds[NUM_LEDS - 1] = temp;

  memmove(&pData[offsetCount], &leds[0], (offsetCount) * sizeof(CRGB));  //copy leds 0-30 to pData 30-59
  memmove(&pData[0], &leds[offsetCount], (offsetCount) * sizeof(CRGB));  //copy leds 30-59 to pData 0-30
  //memmove(&leds[0], &pData[0], (NUM_LEDS) * sizeof(CRGB));

  for (int i = 0; i < NUM_LEDS; i++) {  //added because the copy wasnt fully overriding the array
    leds[i] = CRGB(0, 0, 0);
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pData[i];
  }
}

void timeDisplay1(byte h, byte m, byte s) {
  //minute dial
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));  //clear leds (each frame)
  if (h > 11) {
    h = h - 12;  //(24 hr clock to 12)
  }

  //  hour dials
  for (int i = 0; i < NUM_LEDS; i += (NUM_LEDS / 12)) {
    //for (int i = 0; i < NUM_LEDS; i += 5) {
    leds[i] = CRGB(24, 24, 24);
  }
  for (int i = 0; i < NUM_LEDS; i += (NUM_LEDS / 4)) {
    //for (int i = 0; i < NUM_LEDS; i += 15) {
    leds[i] = CRGB(240, 240, 240);
  }
  //time
  //hour
  if (m < 12) {
    leds[(h * 5)] = HourColor;  //h * 5 scales hour to 60 for clock, and the minutes dictate the hour hand inbetweens
  }
  if (m >= 12 && m < 24) {
    leds[h * 5 + 1] = HourColor;
  }
  if (m >= 24 && m < 36) {
    leds[h * 5 + 2] = HourColor;
  }
  if (m >= 36 && m < 48) {
    leds[h * 5 + 3] = HourColor;
  }
  if (m >= 48 && m < 60) {
    leds[h * 5 + 4] = HourColor;
  }
  //minute
  //leds[m / (60/NUM_LEDS)] = MinuteColor;
  leds[m] = MinuteColor;
  //second
  //leds[s / (60/NUM_LEDS)] = SecondColor;
  leds[s] = SecondColor;
}




void timeDisplay2(byte h, byte m, byte s) {  //all lights white, but dimmer
  //minute dial
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));  //clear leds (each frame)
  if (h > 11) {
    h = h - 12;  //(24 hr clock to 12)
  }

  //  hour dials
  for (int i = 0; i < NUM_LEDS; i += 1) {  //fill all leds in with dim white
    //for (int i = 0; i < NUM_LEDS; i += 5) {
    leds[i] = CRGB(10, 10, 10);
  }
  for (int i = 0; i < NUM_LEDS; i += 5) {  //make every hour position brighter
    //for (int i = 0; i < NUM_LEDS; i += 15) {
    leds[i] = CRGB(50, 50, 50);
  }
  //time
  //hour
  if (m < 12) {
    leds[(h * 5)] = HourColor;  //h * 5 scales hour to 60 for clock, and the minutes dictate the hour hand inbetweens
  }
  if (m >= 12 && m < 24) {
    leds[h * 5 + 1] = HourColor;
  }
  if (m >= 24 && m < 36) {
    leds[h * 5 + 2] = HourColor;
  }
  if (m >= 36 && m < 48) {
    leds[h * 5 + 3] = HourColor;
  }
  if (m >= 48 && m < 60) {
    leds[h * 5 + 4] = HourColor;
  }
  //minute
  //leds[m / (60/NUM_LEDS)] = MinuteColor;
  leds[m] = MinuteColor;
  //scond
  //leds[s / (60/NUM_LEDS)] = SecondColor;
  leds[s] = SecondColor;
}




void timeDisplay3(byte h, byte m, byte s) {
  //minute dial
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));  //clear leds (each frame)
  if (h > 11) {
    h = h - 12;  //(24 hr clock to 12)
  }
  //hour
  if (m < 12) {
    leds[(h * 5)] = HourColor;  //h * 5 scales hour to 60 for clock, and the minutes dictate the hour hand inbetweens
  }
  if (m >= 12 && m < 24) {
    leds[h * 5 + 1] = HourColor;
  }
  if (m >= 24 && m < 36) {
    leds[h * 5 + 2] = HourColor;
  }
  if (m >= 36 && m < 48) {
    leds[h * 5 + 3] = HourColor;
  }
  if (m >= 48 && m < 60) {
    leds[h * 5 + 4] = HourColor;
  }
  //minute
  //leds[m / (60/NUM_LEDS)] = MinuteColor;
  leds[m] = MinuteColor;
  //scond
  //leds[s / (60/NUM_LEDS)] = SecondColor;
  leds[s] = SecondColor;
}





void timeDisplay4(byte h, byte m, byte s) {  //all lights white, but dimmer
  static uint8_t startIndex = 0;
  startIndex = startIndex + .1; /* motion speed */

  FillLEDsFromPaletteColors(startIndex);



  //minute dial
  // fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0)); //clear leds (each frame)
  if (h > 11) {
    h = h - 12;  //(24 hr clock to 12)
  }


  for (int i = 0; i < NUM_LEDS; i += 5) {  //make every hour position brighter
    //for (int i = 0; i < NUM_LEDS; i += 15) {
    leds[i] = CRGB(50, 50, 50);
  }
  //time
  //hour
  if (m < 12) {
    leds[(h * 5)] = HourColor;  //h * 5 scales hour to 60 for clock, and the minutes dictate the hour hand inbetweens
  }
  if (m >= 12 && m < 24) {
    leds[h * 5 + 1] = HourColor;
  }
  if (m >= 24 && m < 36) {
    leds[h * 5 + 2] = HourColor;
  }
  if (m >= 36 && m < 48) {
    leds[h * 5 + 3] = HourColor;
  }
  if (m >= 48 && m < 60) {
    leds[h * 5 + 4] = HourColor;
  }
  //minute
  //leds[m / (60/NUM_LEDS)] = MinuteColor;
  leds[m] = MinuteColor;
  //scond
  //leds[s / (60/NUM_LEDS)] = SecondColor;
  leds[s] = SecondColor;
}


void FillLEDsFromPaletteColors(uint8_t colorIndex) {

  uint8_t TD4brightness = 20;
  for (int i = 0; i < NUM_LEDS; i += 1) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, TD4brightness, LINEARBLEND);
    colorIndex += 1;
  }
}


void timeDisplay5(byte h, byte m, byte s) {  //all lights white, but dimmer
  //minute dial
  if (h > 11) {
    h = h - 12;  //(24 hr clock to 12)
  }
  //  hour dials

  fadeToBlackBy(leds, NUM_LEDS, 100);

  //for (int i = 0; i < NUM_LEDS; i += 5 ) {  //make every hour position brighter
  //for (int i = 0; i < NUM_LEDS; i += 15) {
  //leds[i] = CRGB(50, 50, 50);
  // }
  //time
  //hour

  if (m < 12) {
    leds[(h * 5)] = HourColor;  //h * 5 scales hour to 60 for clock, and the minutes dictate the hour hand inbetweens
  }
  if (m >= 12 && m < 24) {
    leds[h * 5 + 1] = HourColor;
  }
  if (m >= 24 && m < 36) {
    leds[h * 5 + 2] = HourColor;
  }
  if (m >= 36 && m < 48) {
    leds[h * 5 + 3] = HourColor;
  }
  if (m >= 48 && m < 60) {
    leds[h * 5 + 4] = HourColor;
  }
  //minute
  //leds[m / (60/NUM_LEDS)] = MinuteColor;
  leds[m] = MinuteColor;
  //scond
  //leds[s / (60/NUM_LEDS)] = SecondColor;
  leds[s] = SecondColor;
}




void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;  //gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}

//code from https://gist.github.com/kriegsman/99082f66a726bdff7776


void softtwinkles() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (!leds[i]) continue;   // skip black pixels
    if (leds[i].r & 1) {      // is red odd?
      leds[i] -= lightcolor;  // darken if red is odd
    } else {
      leds[i] += lightcolor;  // brighten if red is even
    }
  }
  // Randomly choose a pixel, and if it's black, 'bump' it up a little.
  // Since it will now have an EVEN red component, it will start getting
  // brighter over time.
  if (random8() < 40) {
    int j = random16(NUM_LEDS);
    if (!leds[j]) leds[j] = lightcolor;
  }
}

void lightSpin(int8_t blinkCount, CRGB color, int speed) {

  uint16_t pos;  // pixel postion.
  uint16_t mappedValue;
  FastLED.clear();
  for (int8_t i = 0; i < blinkCount; i++) {
    for (uint16_t i = 0; i < NUM_LEDS; i++) {

      // Map pixel position to range of 0-255 that the ease function needs.
      mappedValue = map(i, 0, NUM_LEDS - 1, 0, 255);  // map(inputValue, fromLow, fromHigh, toLow, toHigh)

      // Run mappedValue through ease funtion.
      mappedValue = ease8InOutCubic(mappedValue);

      // Map ease value (0-255 range) back to pixel position in range of 0 to NUM_LEDS.
      pos = map(mappedValue, 0, 255, 0, NUM_LEDS - 1);  // map(inputValue, fromLow, fromHigh, toLow, toHigh)

      // Display pixels
      leds[pos] = color;
      FastLED.show();
      vTaskDelay(speed / portTICK_PERIOD_MS);
      leds[pos] = CRGB::Black;
      FastLED.show();
    }
  }
}
void showDigit(String digitString, int counter, CRGB color) {
  int8_t digitCount = digitString.length();
  int8_t letterToInt;
  String letter;
  FastLED.clear(true);
  Serial.println(F("Cleared LEDS"));
  Serial.print(F("Inputted String: "));
  Serial.println(digitString);
  Serial.print(F("Printing "));
  Serial.print(digitCount);
  Serial.println(F(" digits"));

  for (int i = 0; i < digitCount; i++) {
    letter = digitString.charAt(i);
    letterToInt = letter.toInt();
    //if int is greater than 9, error

    if (letterToInt > 2) {
      for (int j = 0; j < NUM_LEDS; j += (NUM_LEDS / letterToInt)) {  //evenly space leds
        leds[j] = color;
      }
      ledOffset();
      FastLED.show();
      Serial.print(F(" digit print #"));
      Serial.println(letterToInt);
      vTaskDelay((counter * 2) / portTICK_PERIOD_MS);
    }


    if (letterToInt < 3) {
      //custom offsets for the odd numbers
      if (letterToInt == 0) {
        fill_solid(leds, NUM_LEDS, color);
      }
      if (letterToInt == 1) {
        leds[(NUM_LEDS / 2)] = color;
      }
      if (letterToInt == 2) {
        leds[0] = color;
        leds[(NUM_LEDS - 1) / 2] = color;
      }
      ledOffset();
      FastLED.show();
      Serial.print(F(" digit print(0 to 2) #"));
      Serial.print(letterToInt);
      vTaskDelay(counter / portTICK_PERIOD_MS);
    }

    if (letterToInt > 9) {
      Serial.println(F("ERROR PRINTING, EXITING"));
      return;
    }
    FastLED.clear(true);
    vTaskDelay((counter / 3) / portTICK_PERIOD_MS);
  }
}