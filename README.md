# BLEHandler - Encapsulado para protocolo BLE

Libreria C++ para usar el protocolo BLE de forma mas visual. Diseñada para pryectos pequeños y sencillos  

#### Instalación 
Descargar o clonar en la carpeta `lib/` de tu proyecto PlatformIO.  

## Funciones disponibles

`begin()`: inicializa el servidor BLE  
`addService()`: añade un servicio BLE con una UUID especifica  
`addCharacteristic()`: añada una caracteristica BLE a un servicio en especifico  
`startAdvertising()`: inicia la publicacion del servicio BLE  
`nottify()`: notifica a los clientes conectados.  
`isClientConnected`: chequea si el cliente esta conectado al servidor.  

## Ejemplos incluidos

`basic example`  
    Muestra la implementacion basica y correcta de las funciones minimas necesarias para el funcionamiento del BLE server.  

## Contribuciones
Se aceptan pull requests. Usar clang-format antes de enviar código. Abrir issues si detectás errores o querés sugerencias.  

## Licencia
MIT. Libre para uso personal o comercial.  