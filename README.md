# Implementacion del OLED SSD1306 128x64 
####  ALGORITMO:
  - Mandar y recibir datos para mosrtarlos en la panalla OLED
  - Uso de UART para comunicacion entre microcontroladores
  - framebuf.py - Driver para la pantalla OLED (subir al Pi Pico)
  - ssd1306.py  - Firmware para comunicacion I2C de la pantalla OLED (subir al Pi Pico)
  - Usa Wire1 para I2C independiente de los pines por defecto
#### HARDWARE:
  - MCU        : RP2040-Zero
  - Display : OLED SSD1306 128x64
