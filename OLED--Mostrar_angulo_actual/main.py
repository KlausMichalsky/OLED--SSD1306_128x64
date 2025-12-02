"""
/************************************************************************************************************
 🔹 LECTURA DE ÁNGULO CON AS5600 vía I2C (Wire1), imprimir en OLED 🔹
  - Lee el ángulo del sensor AS5600 conectado a GP26 (SDA) y GP27 (SCL) usando I2C.
  - Convierte el valor crudo de 12 bits (0–4095) a grados (0°–360°).
  - Manda el angulo actual al OLED SSD1306 cada 200 ms.
  - Usa Wire1 para I2C independiente de los pines por defecto.
  K. Michalsky – 11.2025
*************************************************************************************************************/
"""

from machine import UART, Pin, I2C
import utime
from ssd1306 import SSD1306_I2C

# UART al Zero
uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))

# OLED SSD1306
i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)
oled = SSD1306_I2C(128, 64, i2c)

# Valor inicial
degrees_1decimal = "0.0"


# Función para mostrar en OLED
def mostrar(texto1, texto2):
    oled.fill(0)
    oled.text(texto1, 0, 0)
    oled.text(texto2, 0, 16)
    oled.show()


# Inicialización
mostrar("Esperando...", degrees_1decimal)
utime.sleep(1)  # espera 1 segundo para que el Zero arranque

while True:
    # Leer datos del Zero
    if uart.any():
        try:
            line = uart.readline()
            if line:
                degrees = line.decode().strip()
                if len(degrees) > 1:  # evitar strings vacíos
                    # quitar segundo decimal si hay
                    degrees_1decimal = degrees[:-1]
                    print("Zero -> Pico:", degrees_1decimal)
        except Exception as e:
            print("Error UART:", e)

    # Mostrar en OLED el último valor recibido
    mostrar("Angulo:", degrees_1decimal)

    utime.sleep(0.05)  # 50ms
