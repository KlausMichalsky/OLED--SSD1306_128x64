from machine import UART, Pin, I2C
import utime
from ssd1306 import SSD1306_I2C

# UART al Zero
uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))

# OLED SSD1306
i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)
oled = SSD1306_I2C(128, 64, i2c)

# Función para mostrar en OLED
def mostrar(texto1, texto2):
    oled.fill(0)
    oled.text(texto1, 0, 0)
    oled.text(texto2, 0,16)
    oled.show()

# Inicializar
respuesta = ""
mostrar("Esperando...", "")

while True:
    # Leer datos del Zero
    if uart.any():
        respuesta = uart.readline().decode().strip()
        print("Zero -> Pico:", respuesta)

    # Mostrar en OLED
    mostrar("Angulo Zero:", respuesta)

    utime.sleep(0.05)  # 50ms
