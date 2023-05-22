#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef TRANSLATE_H
#define TRANSLATE_H
#define UNUSED(x) (void)(x)

#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 1000

typedef struct {
    char src_word[MAX_WORD_LENGTH];
    char dst_word[MAX_WORD_LENGTH];
} DictionaryEntry;

void freeResources(FILE *input_file, FILE *dictionary_file, FILE *output_file, DictionaryEntry *dictionary, int num_entries);

#endif