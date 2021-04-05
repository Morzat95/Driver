#!/bin/bash

# Compilamos el driver, lo instalamos y luego creamos el device
source up.sh

# Tomamos los argumentos pasados por parámetro -> $1 = n, $2 = message
m=$1
message=${m:-"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ Hello World"} # Mensaje de testeo

# Testeamos que funcione correctamente

## Primero escribimos en el device
echo "Escribimos en el device..."
echo "Original message: $message"
sudo echo $message > /dev/$device

## Luego leemos del device
echo "Leemos del device..."
output=$(cat /dev/${device})
echo "Encrypted message: $output"

# Desinstalamos el driver y removemos el device
source down.sh