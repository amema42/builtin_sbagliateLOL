#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

// Funzione per cercare una variabile d'ambiente
char *find_env(const char *name) {
    for (char **env = environ; *env != NULL; ++env) {
        char *env_name = strtok(*env, "=");
        if (env_name && strcmp(env_name, name) == 0) {
            return *env;
        }
    }
    return NULL;
}

// Funzione per impostare o aggiornare una variabile d'ambiente
int my_setenv(const char *name, const char *value) {
    char *existing_var = find_env(name);
    if (existing_var) {
        // Se la variabile esiste già, aggiorna il suo valore
        size_t len = strlen(name) + strlen(value) + 2; // +2 per '=' e '\0'
        char *env_var = (char *)malloc(len);
        if (env_var == NULL) {
            perror("Errore di allocazione di memoria");
            return -1;
        }
        snprintf(env_var, len, "%s=%s", name, value);

        // Sostituisci la vecchia variabile d'ambiente con la nuova
        for (char **env = environ; *env != NULL; ++env) {
            if (*env == existing_var) {
                *env = env_var;
                free(existing_var); // Libera la memoria della variabile precedente
                return 0;
            }
        }

        // Se non viene trovata una corrispondenza esatta, gestisci l'errore
        free(env_var); // Libera la memoria allocata per env_var
        fprintf(stderr, "Errore nell'aggiornamento della variabile d'ambiente\n");
        return -1;
    } else {
        // Se la variabile non esiste, crea una nuova variabile d'ambiente
        size_t len = strlen(name) + strlen(value) + 2; // +2 per '=' e '\0'
        char *env_var = (char *)malloc(len);
        if (env_var == NULL) {
            perror("Errore di allocazione di memoria");
            return -1;
        }
        snprintf(env_var, len, "%s=%s", name, value);

        // Trova l'ultima posizione nell'array delle variabili d'ambiente e aggiungi la nuova variabile
        int index = 0;
        while (environ[index] != NULL) {
            index++;
        }
        environ[index] = env_var;
        environ[index + 1] = NULL;

        return 0;
    }
}

// Funzione per gestire il comando export
void my_export(const char *arg) {
    char *arg_copy = strdup(arg);
    if (arg_copy == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria\n");
        return;
    }

    char *name = strtok(arg_copy, "=");
    char *value = strtok(NULL, "");

    if (name == NULL || value == NULL) {
        fprintf(stderr, "Errore: formato non valido per export\n");
        free(arg_copy);
        return;
    }

    if (my_setenv(name, value) != 0) {
        fprintf(stderr, "Errore nell'impostare o aggiornare la variabile d'ambiente\n");
    }

    free(arg_copy);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        my_export(argv[1]);
    } else {
        for (char **env = environ; *env != NULL; ++env) {
            printf("%s\n", *env);
        }
    }

    return 0;
}
