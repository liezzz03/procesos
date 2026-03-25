#include <stdio.h>
#include <string.h>
#include "sucursal.h"

int main() {
    char nombre_ciudad[100];
    int capacidad;
    while (1) {
        printf("\n Creador de sucursales\n");
        printf("Introduce nombre de la ciudad (o 'salir'): ");
        scanf(" %[^\n]", nombre_ciudad);
        if (strcmp(nombre_ciudad, "salir") == 0) {
            printf("Cerrando programa maestro\n");
            break;
        }
        printf("Introduce capacidad para %s: ", nombre_ciudad);
        scanf("%d", &capacidad);
        crea_sucursal(nombre_ciudad, capacidad);
    }
    return 0;
}
