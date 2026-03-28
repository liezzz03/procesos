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

    printf("Gestor maestro de sucursales.\n");

    while (1) {
        //Revisamos si algun hijo ha terminado antes de pedir una nueva ciudad
        //Usamos un bucle por si han terminado varios a la vez
        while ((pid_finalizado = waitpid(-1, NULL, WNOHANG))>0) {
            printf("\n[Notificación] Una sala (PID: %d) ha finalizado su ejecución.\n", pid_finalizado);
        }
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
        }
        crea_sucursal(nombre_ciudad, capacidad);
    }
    return 0;
}
