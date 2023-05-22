#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "translate.h"

// функция закрытия рабочих файлов и очистки динамически выделенной памяти словаря
void freeResources(FILE *input_file, FILE *dictionary_file, FILE *output_file, DictionaryEntry *dictionary, int num_entries) {

    UNUSED(num_entries);
    
    if (input_file != NULL) {
        fclose(input_file);
    }
    if (dictionary_file != NULL) {
        fclose(dictionary_file);
    }
    if (output_file != NULL) {
        fclose(output_file);
    }
    if (dictionary != NULL) {
        free(dictionary);
    }
}