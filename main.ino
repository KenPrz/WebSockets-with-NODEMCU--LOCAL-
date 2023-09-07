// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "SSIDKEY";

bool ledState = 0;
const int ledPin = 2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <style>
        body {
            font-family: Arial, Helvetica, sans-serif;
            text-align: center;
            margin: 0;
        }

        .container {
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
        }

        .card {
            background-color: #f8f7f9;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            padding: 20px;
            border-radius: 8px;
            max-width: 400px;
            width: 100%;
        }

        h2 {
            font-size: 1.5rem;
            font-weight: bold;
            color: #143642;
        }

        .button {
            padding: 15px 50px;
            font-size: 24px;
            color: white;
            background-color: #52a7ec;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s, transform 0.1s, box-shadow 0.3s;
            outline: none;
        }

        .button:hover {
            background-color: #0f8b8d;
        }

        .button:active {
            background-color: #77b9ee;
            transform: translateY(2px);
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
        }

        #state {
            font-size: 1.5rem;
            color: #8c8c8c;
            font-weight: bold;
        }
    </style>
    <title>WebSocket</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link rel="icon" href="data:," />
</head>

<body>
    <div class="container">
        <div class="card">
            <h2>Click</h2>
            <button id="button" class="button"><span id="state">%STATE%</span></button>
        </div>
    </div>
    <script>
        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;
        window.addEventListener("load", onLoad);
        function initWebSocket() {
            console.log("Trying to open a WebSocket connection...");
            websocket = new WebSocket(gateway);
            websocket.onopen = onOpen;
            websocket.onclose = onClose;
            websocket.onmessage = onMessage; // <-- add this line
        }
        function onOpen(event) {
            console.log("Connection opened");
        }
        function onClose(event) {
            console.log("Connection closed");
            setTimeout(initWebSocket, 2000);
        }
        function onMessage(event) {
            var state;
            if (event.data == "1") {
                state = "Off";
            } else {
                state = "ON";
            }
            document.getElementById("state").innerHTML = state;
        }
        function onLoad(event) {
            initWebSocket();
            initButton();
        }
        function initButton() {
            document.getElementById("button").addEventListener("click", toggle);
        }
        function toggle() {
            websocket.send("toggle");
        }
    </script>
</body>

</html>
)rawliteral";

void notifyClients() {
  ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}