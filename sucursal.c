//
// Created by paula on 23/03/2026.
//

#include "sucursal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void crea_sucursal(const char* ciudad, int capacidad) {
  pid_t pid = fork();
  if (pid < 0) {
    perror("Error al crear el proceso hijo");
	return;
  }

  if (pid == 0) {
    // convertimos la capacidad a string para pasarla como argumento
    char cap_str[10];
    sprintf(cap_str, "%d", capacidad);
    // execlp busca el ejecutable en el path automáticamente
    execlp("gnome-terminal", "gnome-terminal", "--wait", "--", "./mini_shell", ciudad, cap_str, NULL);
	// si execlp falla, error
    perror("Error al ejecutar gnome-terminal");
    exit(1);
  } else {
    printf("[Maestro] Sucursal creada en %s con pid: %d\n", ciudad, pid);
    // por ahora no ponemos wait () aqui porque queremos que el maestro
    // siga pidiendo ciudades mientras las terminales están abiertas
  }
}