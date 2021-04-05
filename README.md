# TP Drivers

## Modo de uso
Puede ejecutar `demo.sh` para ver una demostración del driver funcionando o `up.sh` y `down.sh` por separado y jugar
con los mensajes a reversar.

## config.sh
En este archivo se puede configurar el nombre del driver, el nombre del dispositivo y los permisos de éste último.

## up.sh
Compila el driver, lo instala y luego crea el dispositivo asociado.

## down.sh
Desinstala el driver y remueve el device que fueron creados al ejecutar `./up.sh`.

## demo.sh
Es una demostración del TP en funcionamiento.
Ejecuta up.sh y down.sh de forma secuencial. Se le pueden pasar por parámetro el mensaje a encriptar (si no se da un mensaje usa uno por default). En el siguiente ejemplo el mensaje a ser reversado es "Hello World": `./demo.sh "Hello World"`