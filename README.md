# 🌦️ Estación Meteorológica ESP32 con Display OLED y ThingSpeak

Sistema de visualización de información meteorológica en tiempo real usando ESP32, pantalla OLED y sincronización con ThingSpeak IoT.

## 📋 Descripción del Proyecto

Este proyecto permite mostrar datos meteorológicos (ciudad, clima y temperatura) en una pantalla OLED conectada a un ESP32. Los datos se reciben mediante un servidor web integrado y se envían automáticamente a ThingSpeak para su monitoreo y análisis en la nube.

## 🔧 Componentes Necesarios

### Hardware
- **ESP32** (cualquier modelo con WiFi)
- **Pantalla OLED SSD1306** (128x64 píxeles, I2C)
- Cables Dupont para conexiones
- Cable USB para programación

### Software
- [PlatformIO](https://platformio.org/) (IDE recomendado)
- Arduino IDE (alternativa)

## 📦 Librerías Requeridas

Las siguientes librerías deben instalarse en tu entorno de desarrollo:

```
- Adafruit GFX Library
- Adafruit SSD1306
- WiFi (incluida en ESP32)
- WebServer (incluida en ESP32)
- Wire (incluida en ESP32)
```

> **Nota importante:** Aunque el código incluye `#include <ThingSpeak.h>`, esta librería **NO se usa** en la implementación actual. El proyecto envía datos a ThingSpeak usando peticiones HTTP manuales, lo cual es más educativo y permite mayor control.

### Instalación de Librerías en PlatformIO

Agrega estas líneas en tu archivo `platformio.ini`:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
    adafruit/Adafruit GFX Library@^1.11.3
    adafruit/Adafruit SSD1306@^2.5.7
    mathworks/ThingSpeak@^2.0.0
monitor_speed = 115200
```

### Instalación de Librerías en Arduino IDE

1. Ve a **Sketch → Include Library → Manage Libraries**
2. Busca e instala cada librería:
   - Adafruit GFX Library
   - Adafruit SSD1306
   - ThingSpeak

## 🔌 Conexiones Hardware

Conecta la pantalla OLED al ESP32 usando el protocolo I2C:

| OLED SSD1306 | ESP32 |
|--------------|-------|
| VCC          | 3.3V  |
| GND          | GND   |
| SCL          | GPIO 22 (SCL por defecto) |
| SDA          | GPIO 21 (SDA por defecto) |

> **Nota**: Algunos módulos ESP32 pueden tener diferentes pines I2C. Verifica la documentación de tu placa.

## ⚙️ Configuración del Proyecto

### 1. Configurar Credenciales WiFi

Abre el archivo `src/main.cpp` y modifica estas líneas con tus datos:

```cpp
const char* ssid = "TU_RED_WIFI";
const char* password = "TU_CONTRASEÑA_WIFI";
```

### 2. Configurar ThingSpeak

#### Crear Canal en ThingSpeak:

1. Ve a [ThingSpeak.com](https://thingspeak.com) y crea una cuenta
2. Crea un nuevo canal (New Channel)
3. Configura 3 campos:
   - **Field 1**: Ciudad
   - **Field 2**: Clima
   - **Field 3**: Temperatura
4. Guarda el canal y copia:
   - **Channel ID**
   - **Write API Key**

#### Actualizar Código:

Modifica estas líneas en `src/main.cpp`:

```cpp
unsigned long myChannelNumber = TU_CHANNEL_ID;
const char * myWriteAPIKey = "TU_WRITE_API_KEY";
```

Y también actualiza esta línea dentro de `handleUpdate()`:

```cpp
String apiKey = "TU_WRITE_API_KEY";
```

## 🚀 Compilación y Carga del Código

### Con PlatformIO:

1. Abre el proyecto en VS Code con PlatformIO
2. Conecta el ESP32 por USB
3. Presiona el botón **Upload** (→) o ejecuta:
   ```bash
   pio run --target upload
   ```
4. Abre el Monitor Serial:
   ```bash
   pio device monitor
   ```

### Con Arduino IDE:

1. Abre `src/main.cpp` en Arduino IDE
2. Selecciona tu placa: **Tools → Board → ESP32 Dev Module**
3. Selecciona el puerto: **Tools → Port → COM#** (Windows) o **/dev/ttyUSB#** (Linux/Mac)
4. Presiona el botón **Upload**
5. Abre el Monitor Serial: **Tools → Serial Monitor** (115200 baud)

## 📡 Uso del Sistema

### 1. Verificar Conexión

Al iniciar, el ESP32 mostrará en el Monitor Serial:

```
Conectando a WiFi...
✅ Conectado a WiFi!
🌐 IP del ESP32: 192.168.x.x
🌍 Servidor web iniciado
```

Anota la dirección IP mostrada.

### 2. Enviar Datos al ESP32

Puedes enviar datos meteorológicos mediante una petición HTTP GET:

```
http://IP_DEL_ESP32/update?city=Bogota&weather=Soleado&temp=22
```

**Ejemplo con diferentes ciudades:**
```
http://192.168.1.100/update?city=Madrid&weather=Nublado&temp=18
http://192.168.1.100/update?city=Lima&weather=Lluvia&temp=15
http://192.168.1.100/update?city=Buenos%20Aires&weather=Despejado&temp=25
```

> **Nota**: Los espacios deben codificarse como `%20` en la URL.

### 3. Métodos para Enviar Datos

#### Desde el navegador:
Copia la URL en tu navegador y presiona Enter.

#### Desde Python:
```python
import requests

ip_esp32 = "192.168.1.100"
url = f"http://{ip_esp32}/update"

datos = {
    "city": "Bogotá",
    "weather": "Soleado",
    "temp": "23"
}

response = requests.get(url, params=datos)
print(response.text)
```

#### Desde JavaScript (página web):
```javascript
const ipESP32 = "192.168.1.100";
const url = `http://${ipESP32}/update?city=Bogota&weather=Soleado&temp=24`;

fetch(url)
  .then(response => response.text())
  .then(data => console.log("Respuesta:", data))
  .catch(error => console.error("Error:", error));
```

#### Desde cURL (terminal):
```bash
curl "http://192.168.1.100/update?city=Bogota&weather=Soleado&temp=22"
```

## 📊 Visualización en ThingSpeak

1. Ve a tu canal en ThingSpeak
2. Los datos se actualizarán automáticamente cada vez que envíes información
3. Puedes crear gráficas y widgets personalizados
4. Los datos se almacenan con timestamp para análisis histórico

> **Importante**: ThingSpeak tiene un límite de 15 segundos entre actualizaciones en cuentas gratuitas.

## 🛠️ Solución de Problemas

### La pantalla OLED no enciende:
- Verifica las conexiones VCC, GND, SCL y SDA
- Confirma que la dirección I2C sea 0x3C (estándar)
- Prueba con otro cable USB con mayor corriente

### No se conecta a WiFi:
- Verifica SSID y contraseña
- Asegúrate de que la red sea 2.4GHz (ESP32 no soporta 5GHz)
- Revisa que el router esté encendido y en rango

### Error al subir código:
- Mantén presionado el botón **BOOT** durante la carga
- Intenta con otro cable USB
- Verifica el puerto COM seleccionado

### Los datos no llegan a ThingSpeak:
- Confirma que el API Key sea correcto
- Revisa que el Channel ID corresponda al canal correcto
- Espera al menos 15 segundos entre actualizaciones

## 📝 Estructura del Proyecto

```
proyecto/
│
├── include/              # Archivos de cabecera (.h)
├── lib/                  # Librerías privadas del proyecto
├── src/
│   ├── main.cpp         # Código principal del proyecto
│   └── CMakeLists.txt   # Configuración para ESP-IDF
├── platformio.ini       # Configuración de PlatformIO
└── README.md            # Este archivo
```

## 🔄 Posibles Mejoras

- [ ] Integrar API de OpenWeatherMap para obtener datos reales
- [ ] Agregar sensor DHT22 para medir temperatura local
- [ ] Implementar modo AP para configuración WiFi sin código
- [ ] Añadir iconos gráficos del clima en la pantalla OLED
- [ ] Crear aplicación móvil para control remoto
- [ ] Agregar almacenamiento local en SD card

## 📄 Licencia

Este proyecto es de código abierto y está disponible bajo licencia MIT.

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor:

1. Haz un Fork del proyecto
2. Crea una rama para tu característica (`git checkout -b feature/NuevaCaracteristica`)
3. Commit tus cambios (`git commit -m 'Agregar nueva característica'`)
4. Push a la rama (`git push origin feature/NuevaCaracteristica`)
5. Abre un Pull Request

## 📧 Contacto

Si tienes preguntas o sugerencias, no dudes en abrir un issue en el repositorio.

---

