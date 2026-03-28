#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "sucursal.h"

struct {
    pid_t pid;
    char ciudad[100];
} tabla_sucursales[100];

int main() {
    char nombre_ciudad[100];
    int capacidad;
    pid_t pid_finalizado;
    int total_salas = 0;

    printf("Gestor maestro de sucursales.\n");

    while (1) {
        //Revisamos si algun hijo ha terminado antes de pedir una nueva ciudad
        //Usamos un bucle por si han terminado varios a la vez
        while ((pid_finalizado = waitpid(-1, NULL, WNOHANG))>0) {
            //Buscamos a quién pertenecía ese PID en nuestra tabla
            for (int i = 0; i < total_salas; i++) {
                if (tabla_sucursales[i].pid == pid_finalizado) {
                    printf("\n[NOTIFICACIÓN] La sala de '%s' (PID: %d) ha finalizado su ejecución.\n",
                           tabla_sucursales[i].ciudad, pid_finalizado);
                    break;
                }
            }        }
        printf("Introduce nombre de la ciudad (o 'salir'): ");
        //Si el usuario no escribe nada o hay error, salimos
        if (scanf("%s", nombre_ciudad) != 1) break;

        if (strcmp(nombre_ciudad, "salir") == 0) {
            printf("Cerrando programa maestro y esperando a las salas restantes\n");
            //Esperar a que todos terminen antes de cerrar el maestro de verdad
            while (wait(NULL) > 0);
            break;
        }
        printf("Introduce capacidad para %s: ", nombre_ciudad);
        if (scanf("%d", &capacidad) != 1) {
            printf("Capacidad no válida.\n");
            continue;
        }
        // Creamos la sucursal y guardamos su información
        pid_t nuevo_pid = crea_sucursal(nombre_ciudad, capacidad);
        if (nuevo_pid > 0) {
            tabla_sucursales[total_salas].pid = nuevo_pid;
            strcpy(tabla_sucursales[total_salas].ciudad, nombre_ciudad);
            total_salas++;
            printf("[Maestro] Sucursal '%s' lanzada con éxito (PID: %d).\n", nombre_ciudad, nuevo_pid);
        }
    }
    return 0;
}
