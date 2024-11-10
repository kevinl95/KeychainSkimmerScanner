/*
*******************************************************************************
* Description: Credit-card skimmer Bluetooth scanner for M5Core
* Date: 2024/10/13
* Author: Kevin Loeffler
* Attribution: HelloWorld.ino (c) 2021 by M5Stack
* bt_classic_device_discovery.ino by Espressif: https://github.com/espressif/arduino-esp32/
*******************************************************************************
*/
#include "M5CoreInk.h"
#include "BluetoothSerial.h"
#include "esp_adc_cal.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#define BT_DISCOVER_TIME 10000

void btAdvertisedDeviceFound(BTAdvertisedDevice *pDevice) {
  Serial.printf("Found a device asynchronously: %s\n", pDevice->toString().c_str());
}

Ink_Sprite InkPageSprite(&M5.M5Ink);
char batteryStrBuff[64];

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(const char *str, const char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}

float getBatVoltage() {
    analogSetPinAttenuation(35, ADC_11db);
    esp_adc_cal_characteristics_t *adc_chars =
        (esp_adc_cal_characteristics_t *)calloc(
            1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12,
                             3600, adc_chars);
    uint16_t ADCValue = analogRead(35);

    uint32_t BatVolmV = esp_adc_cal_raw_to_voltage(ADCValue, adc_chars);
    float BatVol      = float(BatVolmV) * 25.1 / 5.1 / 1000;
    free(adc_chars);
    return BatVol;
}

void Scan() {
    InkPageSprite.clear();                  // clear the sprite.
    InkPageSprite.drawString(10, 50, "Scanning, please wait...");  // draw the string.
    InkPageSprite.pushSprite();             // push the sprite.
    Serial.println("Starting synchronous discovery... ");
    int count = 0;
    int skimmerCount = 0;
    int hc05 = 0;
    int hc06 = 0;
    int other = 0;

    BTScanResults *pResults = SerialBT.discover(BT_DISCOVER_TIME);
    if (pResults) {
      pResults->dump(&Serial);
      count = pResults->getCount();
      for (int i = 0; i < count; i++) {
        BTAdvertisedDevice *dev = pResults->getDevice(i);
        char * success = strdup(dev->toString().c_str());
        Serial.println(success);
        if (dev) {
          if (StrContains(success, "HC-05") == 1) {
            skimmerCount += 1;
            hc05 += 1;
          }
          else if (StrContains(success, "HC-06") == 1) {
            skimmerCount += 1;
            hc06 += 1;
          }
          else if (StrContains(success, "Name: , ") == 1) {
            skimmerCount += 1;
            other += 1;
          }
        }
      }
    } else {
      Serial.println("Error on BT Scan, no result!");
    }
    InkPageSprite.clear();

    sprintf(batteryStrBuff, "Battery:%.2fV", getBatVoltage());
    InkPageSprite.drawString(10, 20, batteryStrBuff,&AsciiFont8x16);

    String displayCount = "Devices found: " + String(count);
    String skimmerDisplayCount = "Skimmers found: " + String(skimmerCount);
    String skimmerTypes = "  " + String(hc05) + "  " + String(hc06) + "  " + String(other);
    InkPageSprite.drawString(35, 50, displayCount.c_str());  // draw the string.
    InkPageSprite.drawString(35, 75, skimmerDisplayCount.c_str());  // draw the string.
    InkPageSprite.drawString(35, 100, skimmerTypes.c_str());  // draw the string.
    if (skimmerCount > 0) {
      InkPageSprite.drawString(0, 150, "Possible skimmers present!");  // draw the danger string.
    }
    InkPageSprite.pushSprite();             // push the sprite.
    if (skimmerCount > 0) {
      M5.Speaker.tone(1000, 200);  // Emit a tone that danger is present
      delay(100);
      M5.Speaker.mute();
    }
    delay(10000);  // Wait 10 seconds
    Reset();
}

void Reset() {
  M5.M5Ink.clear();  // clear the screen.
  InkPageSprite.clear();  // clear the sprite.
  sprintf(batteryStrBuff, "Battery:%.2fV", getBatVoltage());
  InkPageSprite.drawString(10, 20, batteryStrBuff,&AsciiFont8x16);
  InkPageSprite.drawString(35, 50, "Ready to scan");  // draw string.
  InkPageSprite.pushSprite();  // push the sprite.
}

void setup() {
    M5.begin();                // Initialize CoreInk.
    Serial.begin(115200);      // Initialize serial
    if (!M5.M5Ink.isInit()) {
        while (1) delay(100);
    }
    M5.M5Ink.clear();  // clear the screen.
    delay(1000);
    if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0) {
    }
    sprintf(batteryStrBuff, "Battery:%.2fV", getBatVoltage());
    InkPageSprite.drawString(10, 20, batteryStrBuff,&AsciiFont8x16);
    InkPageSprite.drawString(35, 50, "Ready to scan");  // draw string.
    InkPageSprite.pushSprite();  // push the sprite.
    M5.Speaker.tone(2700, 200);
    delay(200);
    M5.Speaker.mute();
    SerialBT.begin("ESP32");  // Bluetooth device name
}

void loop() {
  if (M5.BtnUP.wasPressed() || M5.BtnDOWN.wasPressed() || M5.BtnMID.wasPressed())
      Scan();
  if (M5.BtnPWR.wasPressed()) {  // Right button press
      M5.M5Ink.clear();  // clear the screen.
      M5.shutdown();  // Turn off the power.
  }
  M5.update();  // Refresh device buttons
}
