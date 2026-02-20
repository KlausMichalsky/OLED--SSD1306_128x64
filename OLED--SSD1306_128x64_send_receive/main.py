# ========================================================================
#               🔸 O L E D  -  S E N D   R E C E I V E 🔸
# ========================================================================
#  Archivo    : Pico_Send_Receive_angle_SSD1306.py
#  Autor      : Klaus Michalsky
#  Fecha      : Feb-2026
#
#  DESCRIPCION
#  -----------------------------------------------------------------------
#  - Envía ángulo de prueba al Zero por UART0 (GP0=TX, GP1=RX).
#  - Recibe la respuesta (ángulo+1) y la muestra en OLED SSD1306 128x64.
#  - Actualiza pantalla cada 0.5 s.
#  - Debug por Serial USB.
#
#  HARDWARE
#  -----------------------------------------------------------------------
#  MCU     : Raspberry Pi Pico
#  OLED    : SSD1306 128x64 I2C (GP4=SDA, GP5=SCL)
#  UART0   : GP0=TX, GP1=RX (Zero)
#
#  ESTADO
#  -----------------------------------------------------------------------
#  ✅ Funcional
# ========================================================================


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
# i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)
i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)
oled = SSD1306_I2C(128, 64, i2c)

# -----------------------------
# ANGULO DE PRUEBA
# -----------------------------
angulo = 123


def mostrar(texto1, texto2):
    oled.fill(0)
    oled.text(texto1, 0, 0)
    oled.text(texto2, 0, 16)
    oled.show()


# Inicializar pantalla
respuesta = ""  # definir respuesta como string
mostrar("Esperando...", "")

while True:
    # Enviar angulo al Zero
    uart.write(str(angulo) + "\n")
    print("Pico -> Zero:", angulo)

    # Leer respuesta si hay datos
    if uart.any():
        respuesta = uart.readline().decode().strip()
        print("Zero -> Pico", respuesta)

    # Mostrar en OLED
    mostrar(f"Enviado: {angulo}", f"Recibido: {respuesta}")

    utime.sleep(0.5)
