#include "Display.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS_COUNT, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

uint32_t currentColor;
uint8_t currentBrightness;

Display::Display() {
}

void Display::setup() {
  currentColor = ClockConfig.clockColor;
  currentBrightness = ClockConfig.clockBrightnessNight;
  _isLoading = false;
  
  pixels.begin();
  pixels.setBrightness(ClockConfig.clockBrightnessNight); // Just to be sure as default, this will be updated when sunrise/sunset is available.
  pixels.clear();
}

void Display::loop() {
  this->loopLoading();
}

void Display::loopLoading() {
  if (!_isLoading) {
    return;
  }
  unsigned long currentMillis = millis();
  if (currentMillis - _lastLoadingFrame < 42) {
    // only draw 1 frame every 42 milliseconds
    return;
  }
  _lastLoadingFrame = currentMillis;
  uint16_t animationTime = 1000;
  unsigned long loadingTime = (currentMillis - _loadingStarted);
  float modulo = loadingTime % animationTime;
  float progress = modulo/animationTime;
  int degrees = 360.0*progress;

  double brightness1 = this->brightness(degrees);
  double brightness2 = this->brightness(degrees+30);
  double brightness3 = this->brightness(degrees+60);
  double brightness4 = this->brightness(degrees+90);
  pixels.clear();
  pixels.setPixelColor(81, pixels.Color(brightness1,brightness1,brightness1));
  pixels.setPixelColor(82, pixels.Color(brightness2,brightness2,brightness2));
  pixels.setPixelColor(83, pixels.Color(brightness3,brightness3,brightness3));
  pixels.setPixelColor(84, pixels.Color(brightness4,brightness4,brightness4));
  pixels.show();
}

void Display::displayTemperature(uint8_t temperature) {
  if (_isLoading) {
    return;
  }
  // Only possible to display temperature of 2 digits.
  if (temperature < 100) {
    uint8_t lastDigit = temperature % 10;
    uint8_t firstDigit = (temperature-lastDigit)/10;
    pixels.clear();
    this->displayNumberAtPosition(firstDigit, 0);
    this->displayNumberAtPosition(lastDigit, 1);
    pixels.show();
  }
}

//  Number: number to display
//  Position: 0 = left, 1 = right
void Display::displayNumberAtPosition(uint8_t number, uint8_t position) {
  if (_isLoading) {
    return;
  }
  uint8_t cols = Mappings::numberMappingCols();
  uint8_t rows = Mappings::numberMappingRows();
  uint8_t shiftX = position*cols+position*1;
  for (uint8_t y=0;y<rows;y++) {
    for (uint8_t x=0;x<cols;x++) {
      uint8_t value = pgm_read_byte(&Mappings::numberMapping[number][y][x]);
      if (value == 1) {
        uint8_t pixel = pgm_read_byte(&Mappings::displayMapping[y][x+shiftX]);
        pixels.setPixelColor(pixel, currentColor);
      }
    }
  }
}

uint32_t Display::getCurrentColor() {
  return currentColor;
}

uint8_t Display::getCurrentBrightness() {
  return currentBrightness;
}

/**
 * Reading a word from the PROGMEM
 */
void Display::displayWordAt(uint8_t index) {
  if (_isLoading) {
    return;
  }

  //size is always 6, hardcodede for now
  for (uint8_t i=0; i < 6; i++) {
    uint8_t pixel = pgm_read_byte(&Mappings::wordMapping[index][i]);
    if (pixel != 99) {
      pixels.setPixelColor(pixel, currentColor);
    }
  }
}

/**
 * Display the time on the display
 */
void Display::displayTime(uint8_t hour, uint8_t minute) {
  if (_isLoading) {
    return;
  }
  pixels.clear();

  // 10:20 => tien voor half ELF
  if (minute >= 20) {
    hour = hour+1;
  }

  // 12 hour clock
  hour = hour%12;

  // hour
  if (hour == 0) {
    this->displayWordAt(11);
  } else if (hour >= 1 && hour <= 11) {
    this->displayWordAt(hour-1);
  }

  // 
  if (minute >= 0 && minute < 5) {
    this->displayWordAt(18); // Uur
  } else if (minute >= 5 && minute < 10) {
    this->displayWordAt(12); // Vijf
    this->displayWordAt(16); // Over
  } else if (minute >= 10 && minute < 15) {
    this->displayWordAt(14); // Tien
    this->displayWordAt(16); // Over
  } else if (minute >= 15 && minute < 20) {
    this->displayWordAt(13); // Kwart
    this->displayWordAt(16); // Over
  } else if (minute >= 20 && minute < 25) {
    this->displayWordAt(14); // Tien
    this->displayWordAt(15); // Voor
    this->displayWordAt(17); // Half
  } else if (minute >= 25 && minute < 30) {
    this->displayWordAt(12); // Vijf
    this->displayWordAt(15); // Voor
    this->displayWordAt(17); // Half
  } else if (minute >= 30 && minute < 35) {
    this->displayWordAt(17); // Half
  } else if (minute >= 35 && minute < 40) {
    this->displayWordAt(12); // Vijf
    this->displayWordAt(16); // Over
    this->displayWordAt(17); // Half
  } else if (minute >= 40 && minute < 45) {
    this->displayWordAt(14); // Tien
    this->displayWordAt(16); // Over
    this->displayWordAt(17); // Half
  } else if (minute >= 45 && minute < 50) {
    this->displayWordAt(13); // Kwart
    this->displayWordAt(15); // Voor
  } else if (minute >= 50 && minute < 55) {
    this->displayWordAt(14); // Tien
    this->displayWordAt(15); // Voor
  } else if (minute >= 55 && minute < 60) {
    this->displayWordAt(12); // Vijf
    this->displayWordAt(15); // Voor
  }

  switch (minute%5) {
    case 1: 
      this->displayWordAt(19); // 1
      break;
    case 2:
      this->displayWordAt(20); // 2
      break;
    case 3:
      this->displayWordAt(21); // 3
      break;
    case 4:
      this->displayWordAt(22); // 4
      break;
    default:
      break;
  }
  
  pixels.show();
}

void Display::setBrightness(uint8_t brightness) {
  currentBrightness = brightness;
  pixels.setBrightness(brightness);
  pixels.show();
}

void Display::setColor(uint8_t r, uint8_t g, uint8_t b) {
  this->setColor(pixels.Color(r, g, b));
}

void Display::setColor(uint32_t color) {
  currentColor = color;

  if (_isLoading) {
    return;
  }
  
  for(int j=0; j<=pixels.numPixels();j++){
    uint32_t color = pixels.getPixelColor(j);
    if(color != 0) {
      pixels.setPixelColor(j, currentColor);
    }
  }

  pixels.show();
  
  for(int j=0; j<=pixels.numPixels();j++){
    uint32_t color = pixels.getPixelColor(j);
    if(color != 0) {
      pixels.setPixelColor(j, currentColor);
    }
  }

  pixels.show();
}

double Display::brightness(int deg) {
  return 255.0*0.5*(1.0+sin(deg2rad(deg)));
}

void Display::startLoading() {
  _isLoading = true;
  _loadingStarted = millis();
}

void Display::stopLoading() {
  _isLoading = false;
}

float Display::deg2rad(int deg) {
  return (float)deg*PI/180.0;
}