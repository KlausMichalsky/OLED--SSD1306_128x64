// ========================================================================
//              🔸 O L E D  -  S E N D   R E C E I V E 🔸
// ========================================================================
//  Archivo    : OLED--SSD1306_128x64_send_receive.ino
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
//
//  DESCRIPCION
//  -----------------------------------------------------------------------
//  - Zero recibe ángulo del Pico por UART0.
//  - Suma 1 y envía la respuesta de vuelta.
//  - Debug por Serial USB.
//
//  HARDWARE
//  -----------------------------------------------------------------------
//  MCU     : RP2040-Zero
//  OLED    : SSD1306 128x64 I2C (GP4=SDA, GP5=SCL)
//  UART0   : GP0=TX, GP1=RX (Pico)
//
//  ESTADO
//  -----------------------------------------------------------------------
//  ✅ Funcional
// ========================================================================

void setup()
{
    Serial.begin(115200);  // Debug USB
    Serial1.setTX(0);      // TX al Pico
    Serial1.setRX(1);      // RX del Pico
    Serial1.begin(115200); // UART a 115200
}

void loop()
{
    // revisar si hay datos en UART1
    if (Serial1.available())
    {
        String angulo = Serial1.readStringUntil('\n'); // leer ángulo enviado por Pico
        int valor = angulo.toInt();

        Serial.print("Zero recibio: ");
        Serial.println(valor);

        int respuesta = valor + 1;

        Serial1.println(respuesta); // enviar de vuelta al Pico
        Serial.print("Zero envio: ");
        Serial.println(respuesta);
    }
}
