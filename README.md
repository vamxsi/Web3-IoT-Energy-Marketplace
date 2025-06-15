# Decentralized P2P Energy Marketplace (IoT + Web3)

This repository contains a simulation of a peer-to-peer energy marketplace built with an ESP32. It demonstrates how IoT devices can interact within a decentralized economy, using Web3 principles to facilitate trustless transactions without a central authority.

![image](https://github.com/user-attachments/assets/b3231739-3295-42e0-b046-bd7b3bade8a3)

## Project Overview

This project simulates a micro-grid where an IoT device can either be a **Producer** of excess energy (like a home with solar panels) or a **Consumer**. Using a Web3-inspired simulated blockchain, Producers can sell energy directly to Consumers in exchange for a token ("EnergyCoin").

This system showcases the future of smart grids, where decentralized energy resources can be managed efficiently and transparently.

## Features

- **IoT Device Simulation:** Uses an ESP32 to monitor and display its energy status (Producing vs. Consuming).
- **Web3 Ledger Simulation:** The ESP32 maintains an internal, tamper-resistant (in this context) ledger of all transactions.
- **Decentralized App (DApp):** A simple web interface hosted by the ESP32 allows users to initiate trades and view the public ledger.
- **Real-Time Status Display:** An LCD screen and LEDs provide instant physical feedback on the device's status and wallet balance.
- **Smart Contract Logic:** A C++ function on the ESP32 acts as a "smart contract" to validate and execute trades securely.

## How It Works

1.  **Energy State:** A potentiometer simulates the device's energy generation. When turned up, the device enters "Producing" mode; when turned down, it's in "Consuming" mode.
2.  **Web Interface:** Users connect to the ESP32's WiFi and access a web DApp to interact with the marketplace.
3.  **The Trade:** A user can initiate a trade to "buy" energy. The request is sent to the ESP32.
4.  **Smart Contract Execution:** The ESP32's internal "smart contract" function checks if the seller is actually producing energy and if the buyer has enough funds. If both are true, it updates the wallet balances and records the transaction on its internal ledger.

## Hardware Components (for Wokwi Simulation)

| Component | Quantity | Purpose |
| :--- | :--- | :--- |
| ESP32 Board | 1 | Main controller, hosts web server. |
| LCD 16x2 (I2C) | 1 | Displays system mode and wallet balance. |
| Potentiometer | 1 | Simulates energy production level. |
| Green LED | 1 | Indicates the "Producing" state. |
| Red LED | 1 | Indicates the "Consuming" state. |
| Resistors (220Ω) | 2 | Current limiting for the LEDs. |

## Software & Setup

- **Framework:** Arduino
- **Libraries:** `WebServer`, `WiFi`, `LiquidCrystal_I2C`, `ArduinoJson`
- **Simulation Platform:** [Wokwi](https://wokwi.com)

### Installation Guide

1.  **Clone the Repository:**
    ```
    git clone https://github.com/YourUsername/Web3-IoT-Energy-Marketplace.git
    ```
2.  **Open in Wokwi:** Open the `code/P2P_Energy_Marketplace.ino` file and paste the code into a new Wokwi ESP32 project.
3.  **Add Hardware:** Add all the required components in Wokwi and connect them as per the circuit diagram.
4.  **Start Simulation:** Run the simulation. The ESP32 will connect to the "Wokwi-GUEST" WiFi network.
5.  **Access the DApp:** Open the Serial Monitor to get the ESP32's IP address. Open this IP address in a new browser tab to use the marketplace.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
