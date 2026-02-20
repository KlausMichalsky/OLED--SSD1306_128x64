// ========================================================================
//          🔸 O L E D  -  A S 5 6 0 0   S H O W S   A N G L E 🔸
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

// DEFINICION DE LIBRERIAS Y VARIABLES
//  -----------------------------------------------------------------------
#include <Wire.h>

#define AS5600_ADDR 0x36 // Dirección I2C del AS5600

#define SDA_PIN 26 // Pin SDA AS5600
#define SCL_PIN 27 // Pin SCL AS5600

// ========================================================================
// SETUP
// ========================================================================

void setup()
{
    Serial.begin(115200);  // Comunicación Serial USB
    Serial1.setTX(0);      // TX al Pico
    Serial1.setRX(1);      // RX al Pico
    Serial1.begin(115200); // UART al Pico
    delay(1000);

    // Configuración I2C en GP26=SDA, GP27=SCL usando Wire1
    Wire1.setSDA(26); // Asigna GP26 como SDA
    Wire1.setSCL(27); // Asigna GP27 como SCL
    Wire1.begin();    // Inicializa Wire1

    Serial.println("Zero listo: leyendo AS5600 y enviando al Pico...");
}

// ========================================================================
// LOOP
// ========================================================================
void loop()
{
    uint16_t value = readAS5600Angle();       // Lectura cruda de 12 bits (0–4095)
    float degrees = (value * 360.0) / 4096.0; // Conversión a grados

    // Imprime texto en Serial del Zero
    Serial.print("Ángulo: ");
    Serial.print(degrees);
    Serial.println("°");

    // Manda angulo al Pico con un decimal
    Serial1.print("Ángulo: ");
    Serial1.print(degrees);

    delay(500); // Espera 500 ms antes de siguiente lectura
}

// FUNCION DE LECTURA DEL AS5600
//  -----------------------------------------------------------------------
//  - Lee registros 0x0E (alto) y 0x0F (bajo)
//  - Valor de 12 bits (0–4095)
uint16_t readAS5600Angle()
{
    Wire1.beginTransmission(AS5600_ADDR);       // Inicia comunicación con el sensor
    Wire1.write(0x0E);                          // Selecciona registro de ángulo alto
    Wire1.endTransmission(false);               // Termina escritura, pero mantiene bus activo (repeated start)
    Wire1.requestFrom(AS5600_ADDR, (uint8_t)2); // Solicita 2 bytes (alto y bajo)

    if (Wire1.available() < 2) // Si no llegan 2 bytes, devuelve 0
        return 0;

    uint8_t high = Wire1.read(); // Lee byte alto
    uint8_t low = Wire1.read();  // Lee byte bajo

    return ((high & 0x0F) << 8) | low; // Combina los 12 bits y devuelve valor final
}