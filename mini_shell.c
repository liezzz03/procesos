//
// Created by paula on 25/03/2026.
//

#include "mini_shell.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("¡Bienvenido a la sucursal de %s!\n", argv[1]);
    printf("Capacidad asiganda: %s\n", argv[2]);
    printf("Pulsa enter para cerrar esta sucursal.");
    getchar();
    getchar();
    return 0;
}