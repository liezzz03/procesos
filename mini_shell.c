#include "mini_shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sala.h"

int main(int argc, char *argv[]) {
    // verificamos que el maestro nos haya pasado ciudad y capacidad
    if (argc < 3) {
        printf("Error: Argumentos insuficientes.\n");
        return -1;
    }

    char *ciudad = argv[1];
    int capacidad = atoi(argv[2]);

    // inicializamos la sala en este proceso
    if (crea_sala(capacidad) == -1) {
        printf("Error al crear la sala en %s.\n", ciudad);
        return -1;
    }

    char comando[100];
    int id_o_asiento;

    printf("Sucursal %s (Capacidad: %d)\n", ciudad, capacidad);
    printf("Comandos disponibles: reserva [id], libera [asiento], estado_asiento [asiento], estado_sala, cerrar_sala\n");

    while (1) {
        printf("\n%s> ", ciudad);
        fflush(stdout);

        if (scanf("%s", comando) <= 0) break;

        if (strcmp(comando, "reserva") == 0) {
            scanf("%d", &id_o_asiento);
            int result = reserva_asiento(id_o_asiento);
            if (result == -1) printf("No hay asientos libres o ID inválido.\n");
            else printf("Asiento %d reservado para persona %d.\n", result, id_o_asiento);

        } else if (strcmp(comando, "libera") == 0) {
            scanf("%d", &id_o_asiento);
            int result = libera_asiento(id_o_asiento);
            if (result == -1) printf("El asiento %d ya está libre o no existe.\n", id_o_asiento);
            else printf("Asiento %d liberado (estaba la persona %d).\n", id_o_asiento, result);

        } else if (strcmp(comando, "estado_asiento") == 0) {
            scanf("%d", &id_o_asiento);
            int result = estado_asiento(id_o_asiento);
            if (result == -1) printf("Asiento libre o inexistente.\n");
            else printf("Asiento %d ocupado por: %d\n", id_o_asiento, result);

        } else if (strcmp(comando, "estado_sala") == 0) {
            printf("Aforo: %d | Libres: %d | Ocupados: %d\n",
                    capacidad_sala(), asientos_libres(), asientos_ocupados());

        } else if (strcmp(comando, "cerrar_sala") == 0) {
            elimina_sala();
            printf("Cerrando sucursal de %s...\n", ciudad);
            break; // Finaliza el proceso hijo
        } else {
            printf("Comando desconocido: %s\n", comando);
        }
    }

    return 0;
}