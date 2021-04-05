#!/bin/bash

# Seteamos la configuración y las variables
source config.sh

# Compilamos el driver
make clean & make

# Instalamos el driver
sudo insmod mi_driver.ko

# Vemos que el driver se haya instalado correctamente
dmesg | tail -9

# Obtenemos el major number que le fue asignado al driver
major=`awk "\\$2==\"$device\" {print \\$1}" /proc/devices`
echo "Major number is $major"

# Creamos el device que será manejado por nuestro driver
sudo mknod /dev/$device c $major 0

# Modificamos los permisos del device para poder operar con el mismo
sudo chmod $mode /dev/$device