#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "translate.h"

int main(int argc, char *argv[]) {
    // проверка на корректное количество переданных аргументов при запуске программы в командной строке
    if (argc != 4) {
        printf("Usage: %s input_file dictionary_file output_file\n", argv[0]);
        return 1;
    }

    // открытие файлов из командной строки
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Error opening input_file\n");
        return 1;
    }

    FILE *dictionary_file = fopen(argv[2], "r");
    if (dictionary_file == NULL) {
        printf("Error opening dictionary_file\n");
        fclose(input_file);
        return 1;
    }

    FILE *output_file = fopen(argv[3], "w");
    if (output_file == NULL) {
        printf("Error opening output_file\n");
        fclose(input_file);
        fclose(dictionary_file);
        return 1;
    }

    // чтение содержимого из файла-словаря с динамическим выделением памяти
    DictionaryEntry *dictionary = NULL;
    int num_entries = 0;
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, dictionary_file) != NULL) {
        char *src_word = strtok(buffer, " - \n");
        char *dst_word = strtok(NULL, " - \n");
        if (src_word != NULL && dst_word != NULL) {
            DictionaryEntry *temp = realloc(dictionary, (num_entries + 1) * sizeof(DictionaryEntry));
            if (temp == NULL) {
                printf("Error reallocating memory for dictionary\n");
                freeResources(input_file, dictionary_file, output_file, dictionary, num_entries);
                return 1;
            }
            dictionary = temp;
            strncpy(dictionary[num_entries].src_word, src_word, MAX_WORD_LENGTH - 1);
            dictionary[num_entries].src_word[MAX_WORD_LENGTH - 1] = '\0';
            strncpy(dictionary[num_entries].dst_word, dst_word, MAX_WORD_LENGTH - 1);
            dictionary[num_entries].dst_word[MAX_WORD_LENGTH - 1] = '\0';
            num_entries++;
        }
    }
    fclose(dictionary_file);

    // замена слова из файла-словаря в выходной файл
    while (fgets(buffer, MAX_LINE_LENGTH, input_file) != NULL) {
        int i = 0;
        while (isspace((unsigned char)buffer[i])) {
            fputc(buffer[i], output_file);
            i++;
        }

        char *word = strtok(buffer + i, " \n");
        while (word != NULL) {
            char lower_word[MAX_WORD_LENGTH];
            size_t word_len = strlen(word);
            if (word_len < MAX_WORD_LENGTH) {
                strncpy(lower_word, word, word_len);
                lower_word[word_len] = '\0';
            }
            else {
                strncpy(lower_word, word, MAX_WORD_LENGTH - 1);
                lower_word[MAX_WORD_LENGTH - 1] = '\0';
            }

            int found = 0;
            size_t len = strlen(lower_word);
            if (len > 0 && (ispunct(lower_word[len - 1]) || ispunct(lower_word[0]))) {
                int start = 0;
                int end = len - 1;
                if (ispunct(lower_word[0])) {
                    start = 1;
                }
                if (ispunct(lower_word[len - 1])) {
                    end = len - 2;
                }
                if (start <= end) {
                    char temp[MAX_WORD_LENGTH];
                    strncpy(temp, lower_word + start, end - start + 1);
                    temp[end - start + 1] = '\0';
                    strcpy(lower_word, temp);
                }
            }

            for (int j = 0; j < num_entries; j++) {
                if (strcmp(lower_word, dictionary[j].src_word) == 0) {
                    fputs(dictionary[j].dst_word, output_file);
                    found = 1;
                    break;
                }
            }

            if (!found) {
                fputs(word, output_file);
            }

            word = strtok(NULL, " \n");
            if (word != NULL) {
                fputc(' ', output_file);
            }
        }

        fputc('\n', output_file);
    }

    freeResources(input_file, NULL, output_file, dictionary, num_entries);

    return 0;
}