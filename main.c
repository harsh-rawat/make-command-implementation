#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "error_handle.h"
#include "makefile_parser.h"
#include "execution_engine.h"

void findMakefileTargetAndName(int argc, char *argv[], char **makefile_args);

char *findMakefile(char *name);

int main(int argc, char *argv[]) {

    //makefile_args contain target_name and makefile_name
    char **makefile_args = malloc(sizeof(char *) * 2);
    ValidateMemoryAllocationError(makefile_args);

    //Find the appropriate target and file name based on the input args
    findMakefileTargetAndName(argc, argv, makefile_args);
    //If filename passed using -f is not present then raise an error
    if (makefile_args[1] == NULL || access(makefile_args[1], F_OK) == -1)
        FileNotFoundError(makefile_args[1]);

    FILE *file = fopen(makefile_args[1], "r");
    if (file == NULL) {
        FileNotFoundError("");
    }

    //Generate execution graph by calling ParseMakefile. Then check for any cyclic dependencies in the graph.
    list_node *execution_graph = ParseMakefile(file);
    if (DetectCycleInGraph(execution_graph))
        CycleInGraphError();

    //Execute the execution graph from the node which has the target name as provided (In case of NULL, we use the first rule).
    ExecuteExecutionGraph(execution_graph, makefile_args[0]);

    //Close the file and perform other cleanup
    fclose(file);
    free(makefile_args);
    return 1;
}

void findMakefileTargetAndName(int argc, char *argv[], char **makefile_args) {
    if (argc < 1 || argc > 4)
        InvalidArgumentsError("Error: Number of arguments should be between 0 and 3. Exiting!\n");

    if (argc == 1) {
        //Usage as "make537"
        makefile_args[0] = NULL;
        makefile_args[1] = findMakefile(NULL);
    } else if (argc == 2) {
        //Usage as "make537 clean"
        makefile_args[0] = argv[1];
        makefile_args[1] = findMakefile(NULL);
    } else if (argc == 3) {
        //Usage as "make537 -f custom_makefile"
        if (strcmp(argv[1], "-f") == 0) {
            makefile_args[0] = NULL;
            makefile_args[1] = findMakefile(argv[2]);
        } else
            InvalidArgumentsError(
                    "Error: If 2 arguments are being passed then first argument should be -f. Exiting!\n");
    } else if (argc == 4) {
        //Usage as "make537 -f custom_makefile clean"
        if (strcmp(argv[1], "-f") != 0)
            InvalidArgumentsError(
                    "Error: If 3 arguments are being passed then first argument should be -f. Exiting!\n");
        makefile_args[0] = argv[3];
        makefile_args[1] = findMakefile(argv[2]);
    }
}

char *findMakefile(char *name) {
    if (name == NULL) {
        char *makefile_1 = "makefile";
        char *makefile_2 = "Makefile";
        if (access(makefile_1, F_OK) != -1)
            return makefile_1;
        else if (access(makefile_2, F_OK) != -1)
            return makefile_2;
        else return NULL;
    } else {
        if (access(name, F_OK) == -1)
            return NULL;
        else
            return name;
    }
}