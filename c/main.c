#include "raft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
  int clave;
  int valor;
} record;

void showRecord() {
  printf("clave: %d\n", record.clave);
  printf("valor: %d\n", record.valor);
}

void noRecord() { printf("No existe la clave\n"); }

void getRecord(FILE *file, int clave) {
  int check = 0;
  fseek(file, 0, SEEK_SET);
  while ((fscanf(file, "%d %d", &(record.clave), &(record.valor)) == 2)) {
    if (record.clave == clave) {
      showRecord();
      check = 1;
    }
  }
  if (check == 0) {
    noRecord();
  }
}

void setRecord(FILE *file, int clave, int valor) {
  int old_size = 0, new_size;
  int check = 0;
  fseek(file, 0, SEEK_SET);
  while ((fscanf(file, "%d %d", &(record.clave), &(record.valor)) == 2)) {
    if (record.clave == clave) {
      new_size = ftell(file);
      fseek(file, -(new_size - old_size + 1), SEEK_CUR);
      fprintf(file, "\n%d %d", record.clave, valor);
      check = 1;
    }
    old_size = ftell(file);
  }
  if (check == 0) {
    fprintf(file, "\n%d %d", clave, valor);
  }
}

int main() {
  char database[] = "database.txt";

  FILE *file = fopen(database, "r+");

  if (file == NULL) {
    printf("No existe el fichero\n");
    return (0);
  }

  int run = 1, clave, valor, accion;

  while (run) {
    printf("Introduce acción (leer -> 0, escribir -> 1, salir -> 2): ");
    scanf("%d", &accion);
    switch (accion) {
    case 0:
      printf("Introduce la clave: ");
      scanf("%d", &clave);
      getRecord(file, clave);
      break;
    case 1:
      printf("Introduce la clave: ");
      scanf("%d", &clave);
      printf("Introduce el valor: ");
      scanf("%d", &valor);
      setRecord(file, clave, valor);
      break;

    default:
      run = 0;
      break;
    }
  }

  fclose(file);
  return 0;
}