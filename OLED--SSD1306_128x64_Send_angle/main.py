"""
************************************************************************************************************
 🔹 MASTER RP2040-PICO: ENVÍO Y RECEPCIÓN DE ÁNGULO CON OLED 🔹
 
 DESCRIPCIÓN:
  - Envía un ángulo de prueba (123) al RP2040 Zero mediante UART0 (TX=GP0, RX=GP1).
  - Recibe la respuesta del Zero (ángulo + 1) a través del mismo UART.
  - Muestra en un OLED SSD1306 128x64 el ángulo enviado y la respuesta recibida.
  - La pantalla OLED se actualiza en tiempo real cada 0.5 segundos.
  - UART0: GP0=TX, GP1=RX.
  - I2C0 (OLED): SDA=GP4, SCL=GP5.
  - Inicializa la variable 'respuesta' como string vacío para evitar errores antes de recibir datos.
  - Incluye función 'mostrar(texto1, texto2)' para actualizar el OLED con dos líneas de texto.
 
 ESQUEMA DE CONEXIONES:
 ┌──────────────┐          ┌───────────────┐
 │   Pico       │          │   Zero        │
 ├──────────────┤          ├───────────────┤
 │ GP0 (TX) ───────────────────▶ GP1 (RX)  │
 │ GP1 (RX) ◀─────────────────── GP0 (TX)  │
 │ GND ───────────────────────── GND       │
 │ 3V3 ───────────────────────── 3V3       │
 └──────────────┘          └───────────────┘

 OLED SSD1306 <-> Pico
 ┌──────────────┐
 │ SDA → GP4    │
 │ SCL → GP5    │
 │ VCC → 3.3V   │
 │ GND → GND    │
 └──────────────┘

 COMPORTAMIENTO:
  1. Pico envía ángulo al Zero.
  2. Zero recibe, suma 1 y envía respuesta.
  3. Pico recibe respuesta y la muestra en OLED y Serial USB.
*************************************************************************************************************
"""


from machine import UART, Pin, I2C
import utime
from ssd1306 import SSD1306_I2C

# -----------------------------
# CONFIG UART
# -----------------------------
uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))


# -----------------------------
# CONFIG OLED I2C (SSD1306)
# -----------------------------
i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)
oled = SSD1306_I2C(128, 64, i2c)

# -----------------------------
# ANGULO DE PRUEBA
# -----------------------------
angulo = 123

def mostrar(texto1, texto2):
    oled.fill(0)
    oled.text(texto1, 0, 0)
    oled.text(texto2, 0,16)
    oled.show()

# Inicializar pantalla
respuesta = "" # definir respuesta como string
mostrar("Esperando...", "")

while True:
    # Enviar angulo al Zero
    uart.write(str(angulo) + "\n")
    print("Pico -> Zero:", angulo)

    #Leer respuesta si hay datos
    if uart.any():
        respuesta = uart.readline().decode().strip()
        print("Zero -> Pico", respuesta)

    # Mostrar en OLED
    mostrar(f"Enviado: {angulo}", f"Recibido: {respuesta}")

    utime.sleep(0.5)