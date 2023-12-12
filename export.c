#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENV_VARS 100
#define MAX_ENV_VAR_LENGTH 50

typedef struct {
    char name[MAX_ENV_VAR_LENGTH];
    char value[MAX_ENV_VAR_LENGTH];
} EnvVar;

EnvVar environment[MAX_ENV_VARS];
int num_environment_vars = 0;

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

int main() {
    // Esempio di utilizzo della funzione export
    set_env_variable("HOME", "/home/user");
    set_env_variable("PATH", "/usr/bin:/bin");

    print_environment();

    // Aggiunta o modifica di variabili di ambiente
    set_env_variable("USER", "newuser");
    set_env_variable("PATH", "/usr/local/bin:/usr/bin:/bin");

    print_environment();

    return 0;
}
