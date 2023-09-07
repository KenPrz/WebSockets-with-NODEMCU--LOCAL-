## ESP8266 Web Server with WebSocket

This README provides a brief overview and instructions for using the ESP8266 Web Server with WebSocket code. This code allows you to create a simple web interface to control an LED connected to an ESP8266 microcontroller through a WebSocket connection.

### Hardware Requirements

To use this code, you will need the following:

1. ESP8266 microcontroller (e.g., NodeMCU).
2. An LED connected to GPIO pin 2 (you can change the `ledPin` variable if needed).
3. A Wi-Fi network with the SSID and password configured in the code.

### Software Requirements

- Arduino IDE with the ESP8266 board support installed.
- Required libraries: ESP8266WiFi, ESPAsyncTCP, ESPAsyncWebServer.

### Getting Started

1. Open the Arduino IDE.
2. Make sure you have the ESP8266 board support installed. If not, follow the [official ESP8266 Arduino core installation instructions](https://github.com/esp8266/Arduino#installing-with-boards-manager).
3. Install the required libraries: ESP8266WiFi, ESPAsyncTCP, ESPAsyncWebServer. You can install them using the Arduino Library Manager.
4. Create a new Arduino sketch and paste the provided code into the sketch.
5. Replace the `SSID` and `SSIDKEY` variables with your Wi-Fi network credentials.
6. Connect your ESP8266 to your computer via USB.
7. Select the correct board and port in the Arduino IDE.
8. Upload the sketch to your ESP8266.

### Usage

1. After uploading the sketch, open the Arduino Serial Monitor to view the ESP8266's IP address. This IP address will allow you to access the web interface.

2. Open a web browser and enter the ESP8266's IP address in the address bar.

3. You should see a web page with a "Click" button. Clicking this button will toggle the LED on and off. The button's text will indicate the LED's state.

4. Additionally, the LED state can be controlled via WebSocket communication. You can integrate WebSocket control into your own applications using the provided WebSocket interface. The WebSocket server listens on port 80.

### Customization

You can customize the web page appearance by modifying the `index_html` variable in the code. You can also change the GPIO pin used for the LED by modifying the `ledPin` variable.

### Troubleshooting

- If you encounter any issues, ensure that you have correctly configured your Wi-Fi credentials in the code.
- Double-check your connections, especially if the LED is not responding as expected.

That's it! You now have a basic web server running on your ESP8266 with WebSocket control for an LED. Enjoy experimenting with this project and expanding its functionality as needed.