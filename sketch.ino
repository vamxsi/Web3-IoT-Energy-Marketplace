// Required Libraries for Wokwi
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

// --- IoT HARDWARE DEFINITION ---
#define POT_PIN 34       // Potentiometer to simulate energy production
#define PRODUCER_LED 26  // Green LED for producing energy
#define CONSUMER_LED 25  // Red LED for consuming energy

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- WEB3 SIMULATION ---
struct Wallet {
  String owner;
  int balance;
};

struct Transaction {
  String from;
  String to;
  int amount;
  unsigned long timestamp;
};

// Simulated wallets and ledger
Wallet producerWallet = {"Producer", 100};
Wallet consumerWallet = {"Consumer", 100};
Transaction ledger[20]; // Ledger can hold 20 transactions
int transactionCount = 0;

// --- WEB SERVER & WIFI ---
const char* ssid = "Wokwi-GUEST"; // Wokwi's default WiFi
const char* password = "";
WebServer server(80);

// --- SYSTEM STATE ---
enum EnergyState { PRODUCING, CONSUMING };
EnergyState currentState;
int energyLevel = 0;

void setup() {
  Serial.begin(115200);

  // Initialize hardware pins
  pinMode(PRODUCER_LED, OUTPUT);
  pinMode(CONSUMER_LED, OUTPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Connect to WiFi
  connectToWiFi();

  // Setup web server endpoints
  setupWebServer();
  
  Serial.println("Decentralized Energy Marketplace Initialized.");
  Serial.print("Connect to DApp at: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient(); // Handle incoming web requests
  updateEnergyState();   // Read sensors and update IoT state
  updateDisplay();       // Update the physical LCD display
  delay(500);
}

void connectToWiFi() {
  lcd.print("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected.");
}

void updateEnergyState() {
  energyLevel = analogRead(POT_PIN); // Read 0-4095

  if (energyLevel > 2048) { // More than 50% on the pot means we are producing
    currentState = PRODUCING;
    digitalWrite(PRODUCER_LED, HIGH);
    digitalWrite(CONSUMER_LED, LOW);
  } else {
    currentState = CONSUMING;
    digitalWrite(PRODUCER_LED, LOW);
    digitalWrite(CONSUMER_LED, HIGH);
  }
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (currentState == PRODUCING) {
    lcd.print("Mode: Producing");
  } else {
    lcd.print("Mode: Consuming");
  }
  lcd.setCursor(0, 1);
  lcd.print("EnergyCoin: ");
  lcd.print(producerWallet.balance);
}

// --- WEB3 & SERVER FUNCTIONS ---
void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/trade", handleTrade);
  server.on("/ledger", handleLedger);
  server.begin();
}

void handleRoot() {
  String html = R"HTML(
    <!DOCTYPE html>
    <html><head><title>P2P Energy Marketplace</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; }
      .container { padding: 20px; }
      button { background-color: #4CAF50; color: white; padding: 15px 20px; border: none; cursor: pointer; }
      #ledger { border: 1px solid #ccc; padding: 10px; margin-top: 20px; text-align: left; }
    </style></head>
    <body>
      <div class="container">
        <h1>Decentralized Energy Marketplace</h1>
        <p>Your Wallet (Producer): <span id="balance">--</span> EnergyCoin</p>
        <button onclick="tradeEnergy()">Buy 10 Energy Units (10 Coins)</button>
        <div id="ledger"><h3>Transaction Ledger</h3><p>No transactions yet.</p></div>
      </div>
      <script>
        function tradeEnergy() {
          fetch('/trade').then(updateData);
        }
        function updateData() {
          fetch('/ledger').then(res => res.text()).then(data => {
            document.getElementById('ledger').innerHTML = data;
          });
        }
        setInterval(updateData, 5000);
        window.onload = updateData;
      </script>
    </body></html>
  )HTML";
  server.send(200, "text/html", html);
}

// This is our simulated "Smart Contract"
void handleTrade() {
  if (currentState == PRODUCING) {
    if (consumerWallet.balance >= 10) {
      // Execute the trade
      consumerWallet.balance -= 10;
      producerWallet.balance += 10;

      // Log the transaction on the ledger
      if (transactionCount < 20) {
        ledger[transactionCount] = {"Consumer", "Producer", 10, millis()};
        transactionCount++;
      }
      server.send(200, "text/plain", "Trade Successful!");
      Serial.println("Trade Executed: 10 EnergyCoin from Consumer to Producer.");
    } else {
      server.send(400, "text/plain", "Trade Failed: Consumer has insufficient funds.");
      Serial.println("Trade Failed: Consumer has insufficient funds.");
    }
  } else {
    server.send(400, "text/plain", "Trade Failed: You are not producing excess energy.");
    Serial.println("Trade Failed: Not producing energy.");
  }
}

void handleLedger() {
  String html = "<h3>Transaction Ledger</h3>";
  html += "<p>Producer Balance: " + String(producerWallet.balance) + "</p>";
  html += "<p>Consumer Balance: " + String(consumerWallet.balance) + "</p><hr>";

  for (int i = 0; i < transactionCount; i++) {
    html += "<p><b>Tx " + String(i) + ":</b> " + String(ledger[i].amount) + " coins from " + ledger[i].from + " to " + ledger[i].to + "</p>";
  }
  server.send(200, "text/html", html);
}
