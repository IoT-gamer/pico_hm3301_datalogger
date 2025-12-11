# Pico W HM3301 Air Quality Datalogger

This project implements a standalone Air Quality Monitor using a **Raspberry Pi Pico W** and the **HM3301 Particulate Matter sensor**. It logs PM1.0, PM2.5, and PM10 data to an **SD Card** and acts as a **BLE Peripheral** to communicate with a mobile client for live readings, file transfer, and time synchronization.

## Features

* **Air Quality Monitoring:** Reads Standard Particulate Matter (CF=1) values (PM1.0, PM2.5, PM10) from the HM3301 sensor.
* **SD Card Logging:** Logs data every 15 minutes to daily CSV files (e.g., 2025-10-30.txt).
* **BLE Connectivity:**
    - **Live Updates:** Notifies connected clients of the current PM2.5 value every 5 seconds.
    - **File Transfer:** Allows clients to request and stream logged files via BLE.
    - **RTC Synchronization:** Syncs the Pico's internal clock via BLE to ensure accurate timestamps for logs.
* **Low Power Design:** Uses the Pico's AON (Always-on) Timer for scheduling and timekeeping.
* **Status Indication:** LED flash patterns indicate connection status (slow flash = advertising, quick flash = connected).

## Hardware Requirements

* Raspberry Pi Pico W (or Pico 2 W).
* HM3301 Laser PM2.5 Dust Sensor.
* SD Card Module (SPI Interface).
* MicroSD Card (Formatted FAT32).

## Pin Configuration / Wiring

HM3301 Sensor (I2C0)

| SD Card Pin |	Pico Pin (GPIO) |
|-------------|------------------|
| SDA | `GPIO 8` |
| SCL  | `GPIO 9` |
| VCC | `3V3_OUT` (3.3V)|
| GND | GND |


### SD Card (SPI1)

Use `hw_config.c` to configure the pins for your SD card module.
The default setting is configured to use `spi1` for the SD card .

Connect your SD card reader to the following `spi1` pins:

| SD Card Pin |	Pico Pin (GPIO) |
|-------------|------------------|
| SCK | `GPIO 10` |
| MOSI  | `GPIO 11` |
| MISO | `GPIO 12` |
| CS | `GPIO 13` |
| VCC | `3V3_OUT` (3.3V) or `VBUS` (5V)* |
| GND | GND |
*Note: Some SD card modules require 5V power. Check your module's specifications.

### BLE Interface (GATT Profile)
The device advertises with the name "PM2.5 Logger". Service UUID: `0xAAA0`.

| Characteristic Name |	UUID | Type | Description |
|---------------------|-------|------|-------------|
| **RTC Sync** | `0xAAA1` | Write | Write 7 bytes to set time: `[Year_H, Year_L, Month, Day, Hour, Min, Sec]`.
| **Command** | `0xAAA2` | Write | Send commands like `GET:<filename>` to request a file download.
| **File Data** | `0xAAA3` | Notify | Streams file content in 64-byte chunks. Ends with `$$EOT$$`.
| **Live PM2.5** | `0xAAA4` | Notify | Sends the current PM2.5 value (uint16_t, Little Endian) every 5 seconds.

## Data Logging Format
Files are stored on the SD card with the name format `YYYY-MM-DD.txt`. The content format is CSV-like:

```
YYYY-MM-DDTHH:MM:SS,PM1_0:<value>,PM2_5:<value>,PM10:<value>
```
Example: `2025-12-10T14:30:00,PM1_0:10,PM2_5:15,PM10:18`.

- **Note:** Logging only begins after the RTC has been synced via BLE to ensure valid timestamps (Year >= 1980).

### Software Dependencies
This project relies on the following libraries, fetched automatically via CMake FetchContent:
* **Pico SDK:** Standard Raspberry Pi Pico SDK.
* **BTstack:** For BLE functionality (included in Pico SDK).
* **[no-OS-FatFS-SD-SDIO-SPI-RPi-Pico](https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico):** For SD Card filesystem access.
* **[hm3301_pico](https://github.com/IoT-gamer/pico-hm3301):** Driver for the HM3301 sensor.

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/IoT-gamer/pico_hm3301_datalogger.git
    cd pico_hm3301_datalogger
   ```

2. Use official [vscode-pico extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico)

3. Select Pico W or Pico 2 W as the target device.
`Ctlr+Shift+P` -> `Raspberry Pi Pico Switch Board` -> `pico_w` or `pico_2w`

4. `Ctlr+Shift+P` -> `CMake: Configure`

5. Click `Compile` in bottom bar.

6. Hold the `BOOTSEL` button on the Pico and connect it to your PC via USB.

7. Click `Run` in bottom bar to flash the firmware.

### Usage Guide
1. **Startup:** Power on the Pico. The LED will flash slowly (1Hz), indicating it is advertising and waiting for a connection/sync.


2. **Sync:** Connect via a BLE app. The app must write the current date/time to characteristic `0xAAA1`. The LED will flash quickly upon connection.


3. **Operation:**
* Once synced, the device enters "Server Mode."
It will log data to the SD card every **15 minutes**.
* If a client stays connected, it receives live PM2.5 updates every **5 seconds**.

4. **Retrieving Data:**
* Write `GET:YYYY-MM-DD.txt` to characteristic `0xAAA2`.
* Listen to notifications on `0xAAA3` to receive the file contents.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details