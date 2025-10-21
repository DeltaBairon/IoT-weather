cmake_minimum_required(VERSION 3.5)
set(PROJECT_VER "1.0.0")  # Fijamos la versión manualmente
set(GIT_FOUND OFF)        # Deshabilitamos completamente git

# Nombre del proyecto
project(iot_thingspeak)

# Incluimos el directorio src
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(iot_thingspeak)
