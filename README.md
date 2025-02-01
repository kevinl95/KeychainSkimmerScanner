# M5Stack ESP32 CoreInk Credit Card Skimmer Detecting Keychain

![A keychain with an e-ink display that says "Zero skimmers found" being dangled in front of a gas pump](images/device.JPG)

Skimming devices, which steal card information from ATMs, gas pumps, and other payment terminals, have become more sophisticated and harder to detect. They work by sitting between your card and the payment equipment, stealing your payment information and storing it for a criminal to retrieve later. Even in 2024, when I am writing this guide, they remain a problem despite having been reported on for years. With a portable, easy-to-use skimmer detector at your fingertips, you can quickly scan for suspicious devices before making a transaction, preventing potential fraud. In this guide we will build an ESP32 Bluetooth scanner that allows for regular use, offering peace of mind and saving you from the hassle of dealing with identity theft or unauthorized charges. Protecting your financial security has never been easier or more accessible!

Why the ESP32, and why a keychain? You may recall years ago that [SparkFun launched an app for your smart phone that scanned for credit card skimmers](https://learn.sparkfun.com/tutorials/gas-pump-skimmers/all). This clever app used the Bluetooth chipset in your phone to look for signatures of credit card skimmers that law enforcement had given them access to. However:

-Bluetooth skimmers communicate over Bluetooth Classic, not Bluetooth Low Energy. This means this app and ones like it never worked for iPhones!

-The original SparkFun app is no longer available

-The ESP32 chipset is low-power, comes in a variety of development kits with cool peripherals, and can communicate with Bluetooth Classic devices

This means we can easily develop a companion for your smart phone, regardless of if you run Android or iOS, that can sit on your keychain ready for you to use at the gas pump, ATM, or other places you swipe your cards!

One of these ESP32 development kits is the [M5Stack Core Ink development kit](https://shop.m5stack.com/products/m5stack-esp32-core-ink-development-kit1-54-elnk-display). It has buttons, a rechargeable battery, and a slick e-ink display. This is a tiny version of the kinds of displays you've seen used for eReaders, like Amazon's Kindle. These displays are low-power, making them great for keychains and other electronics that you don't want to recharge frequently.

# Building

You will need the [Arduino IDE](https://www.arduino.cc/en/software).

Before opening `KeychainSkimmerScanner.ino` from this repository, you will need to add some board management URLs so that we can download dependencies for this project. To do this:

Step 1: Open the Arduino IDE

Launch the Arduino IDE on your computer.

Step 2: Open the Preferences Window

In the top menu, click on "File" (on Windows/Linux) or "Arduino" (on macOS). Select "Preferences" from the dropdown.

Step 3: Add the URL in the Preferences Window

In the Preferences window, look for the field labeled "Additional Boards Manager URLs". This is where you will add the URL.

Step 4: Enter the URLs

Enter these URLs and separate them with commas.

```
https://dl.espressif.com/dl/package_esp32_index.json, https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
```

Step 5: Click OK

After entering the URLs, click OK to save the changes and close the Preferences window.

Step 6: Open the Boards Manager

From the top menu, click "Tools".
    Navigate to "Board" and select "Boards Manager..." from the dropdown.

Step 7: Search for M5Stack

In the Boards Manager window, use the search bar to find the board package for M5Stack.

Step 8: Install the Package

Once you've found the package click Install.
The Arduino IDE will download and install the necessary libraries and tools automatically.

Step 9: Install the M5Core-Ink library

From the top menu, click "Tools".
    Navigate to "Manage Libraries" and click it.
    A pane will open on the left. Search for "m5core-ink". Install version 1.0.0 by clicking the "Install" button.

Step 10: Select the Installed Board

After installation, you can select the newly installed board by navigating to Tools > Board and choosing the M5CoreInk


Now that we have our IDE set up, open KeychainSkimmerScanner.ino. Connect your M5CoreInk development kit using the included USB cable. Select the correct serial port from the Ports menu in the Arduino IDE under Tools > Port. If you are unsure what port to use, check the device manager for your operating system to see where the board connected. Click `Upload' in the upper left. The code will build and the code will automatically be uploaded to the development kit. When finished, your new keychain will emit a tone!

There is a wheel on the right side of the develoment kit. Move it up or down. Scanning will begin automatically. The device will report how many suspicious devices it detected, as well as how many Bluetooth devices it saw in total.
