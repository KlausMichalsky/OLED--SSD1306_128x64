# ========================================================================
# 🔸 O L E D - A S 5 6 0 0  S H O W S  A N G L E  N O  L A T E N C Y 🔸
# ========================================================================
#  Archivo    : OLED--SSD1306_128x64_AS5600_shows_angle_no_latency.ino.py
#  Autor      : Klaus Michalsky
#  Fecha      : Feb-2026
#
#  DESCRIPCION
#  -----------------------------------------------------------------------
#  - Lee ángulo del sensor AS5600 vía I2C (Wire1) en GP26=SDA, GP27=SCL.
#  - Convierte valor crudo 12 bits (0–4095) a grados (0°–360°).
#  - Muestra ángulo en OLED SSD1306 128x64 cada 200 ms.
#  - Recibe valor desde Zero por UART0 (GP0=TX, GP1=RX).
#  - Suavizado exponencial para evitar parpadeos en OLED.
#
#  HARDWARE
#  -----------------------------------------------------------------------
#  MCU     : Raspberry Pi Pico
#  Sensor  : AS5600 I2C (GP26=SDA, GP27=SCL)
#  OLED    : SSD1306 128x64 0.96” I2C (GP4=SDA, GP5=SCL)
#  UART0   : GP0=TX, GP1=RX (Zero)
#
#  ESTADO
#  -----------------------------------------------------------------------
#  ✅ Funcional
# ========================================================================

import utime
from machine import UART, Pin, I2C
from ssd1306 import SSD1306_I2C  # librería SSD1306

# UART al Zero
uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))

# OLED SSD1306 I2C
i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)
oled = SSD1306_I2C(128, 64, i2c)

# Posiciones
LABEL_X = 0
LABEL_Y = 0
VALUE_X = 0
VALUE_Y = 20

# Dibujar etiqueta fija
oled.fill(0)
oled.text("Angulo:", LABEL_X, LABEL_Y)
oled.show()

# Variables
degrees_1decimal = "0.0"
last_displayed = ""
smooth_val = 0.0
alpha = 0.3  # factor suavizado


# Dibujar número en OLED SSD1306 (sin fill_rect)
def mostrar_numero(valor):
    oled.fill(0)  # limpiar toda la pantalla
    oled.text("Angulo:", LABEL_X, LABEL_Y)  # dibujar etiqueta fija
    oled.text(valor, VALUE_X, VALUE_Y)      # dibujar número
    oled.show()


while True:
    # Leer UART
    while uart.any():
        try:
            line = uart.readline()
            if line:
                s = line.decode().strip()
                try:
                    val_float = float(s)
                    # Suavizado exponencial
                    smooth_val = alpha * val_float + (1 - alpha) * smooth_val
                    degrees_1decimal = "{:.1f}".format(smooth_val)
                except:
                    pass
        except Exception as e:
            print("Error UART:", e)

    # Actualizar OLED solo si cambió
    if degrees_1decimal != last_displayed:
        mostrar_numero(degrees_1decimal)
        last_displayed = degrees_1decimal

    utime.sleep(0.02)  # ~50 FPS máximo
