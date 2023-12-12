#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENV_VARS 100
#define MAX_ENV_VAR_LENGTH 50
#define ENV_FILE "environment.txt" // File per memorizzare le variabili di ambiente

typedef struct {
    char name[MAX_ENV_VAR_LENGTH];
    char value[MAX_ENV_VAR_LENGTH];
} EnvVar;

EnvVar environment[MAX_ENV_VARS];
int num_environment_vars = 0;

void load_environment() {
    FILE *file = fopen(ENV_FILE, "r");
    if (file != NULL) {
        char line[MAX_ENV_VAR_LENGTH * 2]; // Considerando name=value
        while (fgets(line, sizeof(line), file)) {
            char *name = strtok(line, "=");
            char *value = strtok(NULL, "=");
            if (name != NULL && value != NULL) {
                strcpy(environment[num_environment_vars].name, name);
                strcpy(environment[num_environment_vars].value, value);
                num_environment_vars++;
            }
        }
        fclose(file);
    }
}

void save_environment() {
    FILE *file = fopen(ENV_FILE, "w");
    if (file != NULL) {
        for (int i = 0; i < num_environment_vars; i++) {
            fprintf(file, "%s=%s\n", environment[i].name, environment[i].value);
        }
        fclose(file);
    }
}

void print_environment() {
    for (int i = 0; i < num_environment_vars; i++) {
        printf("%s=%s\n", environment[i].name, environment[i].value);
    }
}

int find_env_variable(const char *var_name) {
    for (int i = 0; i < num_environment_vars; i++) {
        if (strcmp(environment[i].name, var_name) == 0) {
            return i;
        }
    }
    return -1;
}

int set_env_variable(const char *var_name, const char *var_value) {
    int index = find_env_variable(var_name);
    if (index != -1) {
        strcpy(environment[index].value, var_value);
    } else {
        if (num_environment_vars < MAX_ENV_VARS) {
            strcpy(environment[num_environment_vars].name, var_name);
            strcpy(environment[num_environment_vars].value, var_value);
            num_environment_vars++;
        } else {
            fprintf(stderr, "Cannot add more environment variables\n");
            return -1;
        }
    }
    return 0;
}

int execute_export(char *command) {
    // Analizza il comando per ottenere i parametri per la variabile di ambiente
    char *token = strtok(command, " ");
    token = strtok(NULL, " ");

    if (token == NULL) {
        // Mostra tutte le variabili di ambiente se non ci sono parametri
        print_environment();
        return 0;
    }

    // Estrae il nome della variabile e il suo valore dalla stringa di comando
    char *name = strtok(token, "=");
    char *value = strtok(NULL, "=");

    if (name != NULL && value != NULL) {
        set_env_variable(name, value);
        save_environment(); // Salva le modifiche nel file
    } else {
        fprintf(stderr, "Invalid syntax. Usage: export VARIABLE=VALUE\n");
        return -1;
    }

    return 0;
}

int main() {
    load_environment(); // Carica le variabili di ambiente dal file all'avvio della shell

    char user_input[100];
    while (1) {
        printf("$ "); // Prompt per l'utente
        fgets(user_input, sizeof(user_input), stdin);

        // Rimuovi il newline inserito dall'utente con fgets
        user_input[strcspn(user_input, "\n")] = '\0';

        if (strcmp(user_input, "exit") == 0) {
            break; // Esci dal ciclo se l'utente digita "exit"
        } else if (strncmp(user_input, "export", 6) == 0) {
            execute_export(user_input); // Esegui il comando di esportazione
        } else {
            // Altrimenti, esegui il comando inserito dall'utente
            // Implementa la logica per eseguire altri comandi della tua shell qui
            printf("Eseguito il comando: %s\n", user_input);
        }
    }

    return 0;
}
