#include <Wire.h>

#define AS5600_ADDR 0x36 // Dirección I2C del AS5600

// Pines I2C
#define SDA_PIN 26
#define SCL_PIN 27

void setup()
{
    Serial.begin(115200);  // Debug por USB
    Serial1.begin(115200); // UART al Pico

    Wire.begin(SDA_PIN, SCL_PIN);
    Serial.println("Zero listo: leyendo AS5600 y enviando al Pico...");
}

int leerAS5600()
{
    Wire.beginTransmission(AS5600_ADDR);
    Wire.write(0x0C); // Registro de ángulo alto (ANGLE HIGH)
    Wire.endTransmission(false);
    Wire.requestFrom(AS5600_ADDR, 2);

    if (Wire.available() >= 2)
    {
        uint16_t high = Wire.read();
        uint16_t low = Wire.read();
        uint16_t angulo = ((high << 8) | low) & 0x0FFF; // 12 bits
        return angulo;                                  // 0-4095
    }
    return -1; // error
}

void loop()
{
    int angulo = leerAS5600();

    if (angulo >= 0)
    {
        // Enviar al Pico
        Serial1.println(angulo);
        Serial.print("Zero envia: ");
        Serial.println(angulo);
    }

    delay(50); // cada 50ms (~20Hz)
}
