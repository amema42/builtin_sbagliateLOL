#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 100
#define MAX_VAR_LENGTH 50

char* environment[MAX_VARS]; // Array per memorizzare le variabili d'ambiente
int envCount = 0; // Contatore delle variabili d'ambiente

// Funzione per impostare una variabile d'ambiente
void exportVar(const char* variable) {
    // Controlla se abbiamo raggiunto il limite massimo di variabili d'ambiente
    if (envCount >= MAX_VARS) {
        printf("Errore: limite massimo di variabili d'ambiente raggiunto\n");
        return;
    }

    // Alloca memoria per la nuova variabile d'ambiente
    environment[envCount] = (char*)malloc(MAX_VAR_LENGTH * sizeof(char));
    if (environment[envCount] == NULL) {
        printf("Errore di allocazione della memoria\n");
        return;
    }

    // Copia la variabile d'ambiente nella posizione appropriata nell'array
    strncpy(environment[envCount], variable, MAX_VAR_LENGTH - 1);
    environment[envCount][MAX_VAR_LENGTH - 1] = '\0';

    // Incrementa il contatore delle variabili d'ambiente
    envCount++;

    printf("Variabile d'ambiente esportata con successo\n");
}

// Funzione per visualizzare le variabili d'ambiente
void displayEnvironment() {
    printf("Variabili d'ambiente:\n");
    for (int i = 0; i < envCount; i++) {
        printf("%s\n", environment[i]);
    }
}

int main() {
    char command[MAX_VAR_LENGTH];
    printf("Inserisci un comando (es. VAR=VAL): ");
    fgets(command, sizeof(command), stdin);

    // Rimuovi il carattere newline da fgets se presente
    size_t len = strlen(command);
    if (len > 0 && command[len - 1] == '\n') {
        command[len - 1] = '\0';
    }

    exportVar(command); // Imposta la variabile d'ambiente con il comando inserito

    displayEnvironment(); // Visualizza le variabili d'ambiente

    // Liberare la memoria allocata per le variabili d'ambiente
    for (int i = 0; i < envCount; i++) {
        free(environment[i]);
    }

    return 0;
}
