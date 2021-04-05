#!/bin/bash

# Seteamos la configuración y las variables
source config.sh

# Removemos el nodo obsoleto
sudo rm -f /dev/$device

# Removemos el driver
sudo rmmod $module

# Vemos que el driver se haya desinstalado correctamente
dmesg | tail -3