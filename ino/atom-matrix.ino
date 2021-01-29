#include <ArduinoJson.h>

// Allocate a 2048-byte buffer for the JSON document.
StaticJsonDocument<2048> jsonDoc;

#include <HTTPClient.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <M5Atom.h>   // http://librarymanager/All#M5Atom  https://github.com/m5stack/M5Atom
#include <FastLED.h>  // http://librarymanager/All#FastLED https://github.com/FastLED/FastLED

WiFiClientSecure client;
HTTPClient http;

CRGB grb(int r, int g, int b) {
  return CRGB(g, r, b);
}

const CRGB CLR_RED   = grb(255, 0, 0);
const CRGB CLR_GREEN = grb(0, 255, 0);
const CRGB CLR_BLUE  = grb(0, 0, 255);

void setup() {
  // Connect to WiFi
  WiFi.begin("TPLink-Louis", "4uAenVLtkmdc");
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected.");


  // 初期化
  M5.begin(true, false, true);  // (Serial, I2C, NeoPixel)
  Wire.begin(25, 21, 10000);    // 内蔵I2C 0x68(MPU6886)

  // GPIO初期化
  pinMode(22, INPUT); // PIN  (INPUT, OUTPUT,       )
  pinMode(19, INPUT); // PIN  (INPUT, OUTPUT,       )
  pinMode(23, INPUT); // PIN  (INPUT, OUTPUT,       )
  pinMode(33, INPUT); // PIN  (INPUT, OUTPUT, ANALOG)
  pinMode(26, INPUT); // GROVE(INPUT, OUTPUT, ANALOG)無線利用時にはANALOG利用不可, DAC出力可
  pinMode(32, INPUT); // GROVE(INPUT, OUTPUT, ANALOG)

  // 内部ピン初期化
  pinMode(12, OUTPUT_OPEN_DRAIN); // IR赤外線(LOWで出力)
  digitalWrite(12, HIGH);

  // LED(0-24)
  for (int i = 0; i < 25; i++) {
    switch (i % 3) {
      case 0:
        M5.dis.drawpix(i, CLR_RED);
        break;
      case 1:
        M5.dis.drawpix(i, CLR_GREEN);
        break;
      case 2:
        M5.dis.drawpix(i, CLR_BLUE);
        break;
    }
    //M5.dis.drawpix(i, grb(255, 0, 0)); // GRB
  }
  //
  //getFirebase();
}

void loop() {
  M5.update();

  if (M5.Btn.wasPressed()) {
    getFirebase();
  }

  delay(1);
}

void getFirebase() {
  String url = "https://atom-matrix-default-rtdb.europe-west1.firebasedatabase.app/leds.json";
  http.setTimeout(1000);
  http.begin(client, url); // Issue the HTTP GET request.
  int response = http.GET();
  if (response <= 0) {
    Serial.printf("HTTP error: %s\n", http.errorToString(response).c_str());
    return;
  }
  // Read the response.
  String payload = http.getString();
  Serial.println("Got HTTP response:");
  Serial.println(payload);
  // Parse the JSON response.
  DeserializationError err = deserializeJson(jsonDoc, payload);
  Serial.print("Deserialize returned: ");     // Cast the response to a JSON object.
  JsonObject jobj = jsonDoc.as<JsonObject>();  // Read each JSON object field.
  // Set LEDs
  for (int i = 0; i < 25; i++) {
    int r_value = jobj["LED_" + String(i)]["R"];
    int g_value = jobj["LED_" + String(i)]["G"];
    int b_value = jobj["LED_" + String(i)]["B"];
    M5.dis.drawpix(i, grb(r_value, g_value, b_value));
    delay(50);
  }
}
