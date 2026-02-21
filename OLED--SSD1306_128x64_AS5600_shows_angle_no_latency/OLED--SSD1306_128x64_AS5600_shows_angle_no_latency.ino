// ========================================================================
// 🔸 O L E D  -  A S 5 6 0 0  S H O W S  A N G L E  N O  L A T E N C Y 🔸
// ========================================================================
//  Archivo    : OLED--SSD1306_128x64_AS5600_shows_angle.ino
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
//
//  DESCRIPCION
//  -----------------------------------------------------------------------
//  - Lee ángulo del sensor AS5600 (GP26=SDA, GP27=SCL) vía I2C (Wire1).
//  - Convierte valor crudo 12 bits (0–4095) a grados (0°–360°).
//  - Envía ángulo al Pico por UART0 (GP0=TX, GP1=RX) cada 500 ms.
//  - Debug por Serial USB.
//
//  HARDWARE
//  -----------------------------------------------------------------------
//  MCU     : RP2040-Zero
//  OLED    : SSD1306 128x64 I2C (GP4=SDA, GP5=SCL)
//  SENSOR  : AS5600 I2C
//  UART0   : GP0=TX, GP1=RX (Pico)
//
//  ESTADO
//  -----------------------------------------------------------------------
//  ✅ Funcional
// ========================================================================

// DEFINICION DE LIBRERIAS Y VARIABLES ------------------------------------
#include <Wire.h>

#define AS5600_ADDR 0x36
#define SDA_PIN 26
#define SCL_PIN 27

#define DELTA_DEG 0.5
#define SEND_INTERVAL 33 // ms -> ~30Hz
unsigned long lastSendTime = 0;
float lastSentAngle = -1000;

// SETUP ----------------------------------------------------------------
void setup()
{
    Serial.begin(115200);
    Serial1.setTX(0);
    Serial1.setRX(1);
    Serial1.begin(115200);
    delay(500);

    Wire1.setSDA(SDA_PIN);
    Wire1.setSCL(SCL_PIN);
    Wire1.begin();

    Serial.println("Zero listo: leyendo AS5600 y enviando al Pico...");
}

// LOOP ----------------------------------------------------------------
void loop()
{
    uint16_t value = readAS5600Angle();
    float degrees = (value * 360.0) / 4096.0;

    // Redondeo a 1 decimal
    degrees = ((int)(degrees * 10 + 0.5)) / 10.0;

    Serial.print("Ángulo AS5600: ");
    Serial.println(degrees, 1);

    unsigned long now = millis();
    if (abs(degrees - lastSentAngle) >= DELTA_DEG && now - lastSendTime >= SEND_INTERVAL)
    {
        Serial1.println(degrees, 1); // enviar 1 decimal
        lastSentAngle = degrees;
        lastSendTime = now;
    }

    delay(1);
}

// FUNCION AS5600 ----------------------------------------------------------------
uint16_t readAS5600Angle()
{
    Wire1.beginTransmission(AS5600_ADDR);
    Wire1.write(0x0E);
    Wire1.endTransmission(false);
    Wire1.requestFrom(AS5600_ADDR, (uint8_t)2);

    if (Wire1.available() < 2)
        return 0;

    uint8_t high = Wire1.read();
    uint8_t low = Wire1.read();

    return ((high & 0x0F) << 8) | low;
}