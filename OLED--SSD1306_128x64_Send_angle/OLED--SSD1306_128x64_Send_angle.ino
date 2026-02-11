/************************************************************************************************************
 🔹 PROYECTO MASTER-SLAVE: ANGULO PICO ↔ ZERO CON OLED 🔹
 
 DESCRIPCIÓN:
  - Master: Raspberry Pi Pico
      • Envía un ángulo de prueba (123) al RP2040 Zero mediante UART0 (TX=GP0, RX=GP1).
      • Recibe la respuesta del Zero (ángulo + 1) por UART.
      • Muestra en un OLED SSD1306 128x64 el ángulo enviado y la respuesta recibida.
      • Pines I2C del OLED: SDA=GP4, SCL=GP5.
      • Pantalla actualizada en tiempo real cada 0.5-1s.
  
  - Slave: RP2040 Zero
      • Recibe el ángulo enviado por el Pico vía Serial1 (TX=GP0, RX=GP1).
      • Calcula respuesta = ángulo + 1 y la envía de vuelta al Pico.
      • Imprime en el monitor USB del Zero el ángulo recibido y la respuesta enviada (debug).

 ESQUEMA DE CONEXIONES:

 ┌──────────────┐          ┌───────────────┐
 │   Pico       │          │   Zero        │
 ├──────────────┤          ├───────────────┤
 │ GP0 (TX) ─────────────────▶ GP1 (RX)    │
 │ GP1 (RX) ◀───────────────── GP0 (TX)    │
 │ GND ─────────────────────── GND         │
 │ 3V3 ─────────────────────── 3V3         │
 └──────────────┘          └───────────────┘

 OLED SSD1306 <-> Pico
 ┌──────────────┐
 │ SDA → GP4    │
 │ SCL → GP5    │
 │ VCC → 3.3V   │
 │ GND → GND    │
 └──────────────┘

 COMPORTAMIENTO:
  1. Pico envía ángulo.
  2. Zero recibe, suma 1 y envía respuesta.
  3. Pico recibe respuesta y la muestra en OLED y Serial.
*************************************************************************************************************/


void setup(){
    Serial.begin(115200);       // Debug USB
    Serial1.setTX(0);           // TX al Pico
    Serial1.setRX(1);           // RX del Pico
    Serial1.begin(115200);      // UART a 115200
}

void loop(){
    // revisar si hay datos en UART1
    if (Serial1.available()){
        String angulo = Serial1.readStringUntil('\n');  // leer ángulo enviado por Pico
        int valor = angulo.toInt();

        Serial.print("Zero recibio: ");
        Serial.println(valor);

        int respuesta = valor + 1;

        Serial1.println(respuesta);   // enviar de vuelta al Pico
        Serial.print("Zero envio: ");
        Serial.println(respuesta);
    }
}
