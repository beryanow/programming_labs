#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void encoding(char *argv[]) {
    int arr[256];
    for (int i = 0; i < 256; i++)
        arr[i] = 0;

    int i = 2;
    int check_fail = 0;

    FILE *infile = fopen(argv[i], "rb");
    FILE *outfile = fopen(argv[i + 1], "wb");
    if (infile == NULL) {
        print_help();
        check_fail = 1;
    }

    if (check_fail == 0) {
        fseek(infile, 0, SEEK_END);
        long file_size = ftell(infile);
        rewind(infile);

        unsigned char *original_file;
        original_file = (unsigned char *) malloc(sizeof(unsigned char) * file_size);
        fread(original_file, sizeof(unsigned char), (size_t) file_size, infile);

        int j = 0;
        int h = 0;
        while (j < file_size) {
            arr[original_file[j]]++;
            j++;
        }

        for (int w = 0; w < 256; w++) {
            if (arr[w] != 0)
                h++;
        }

        int y = h; // amount of coded symbols of each kind

        s_node *initial_nodes = (s_node *) malloc(sizeof(s_node) * h);

        j = 0;
        for (i = 0; i < 256; i++) {
            if (arr[i] > 0) {
                initial_nodes[j].key = i;
                initial_nodes[j].times = arr[i];
                j++;
            }
        }

        int *times_arr = (int *) malloc(sizeof(int) * 256);

        qsort_implementation(times_arr, h, initial_nodes);

        s_node **set_tree = (s_node **) connecting_nodes(h, initial_nodes);

        char *temp_seq = (char *) malloc(sizeof(char) * 8);

        struct sequence *sym_codes = (struct sequence *) calloc(sizeof(struct sequence), 256);

        int max;
        j = 0;

        encoding_symbols(set_tree[0], temp_seq, &j, sym_codes, &max);

        if (y == 1) {
            sym_codes[set_tree[0]->key].cod[0] = '1';
            sym_codes[set_tree[0]->key].otm = 0;
        }
        char *code = (char *) calloc(sizeof(char), (size_t) max * file_size);

        fwrite(&file_size, sizeof(int), 1, outfile);
        fwrite(&y, sizeof(int), 1, outfile);

        int b = 0; // used for following the encoded sequence's length
        j = 0;

        while (j < file_size) {
            int k = 0; // used for following the symbol code's length

            if ((sym_codes[original_file[j]].cod[k] != '\0') && (sym_codes[original_file[j]].otm != 1)) {

                int t = 0; // symbol code's length
                while (sym_codes[original_file[j]].cod[k] != '\0') {
                    t++;
                    k++;
                }

                fwrite(&original_file[j], sizeof(char), 1, outfile);
                fwrite(&t, sizeof(char), 1, outfile);

                sym_codes[original_file[j]].otm = 1;

                printing_sym_code(sym_codes, original_file, j, outfile);
            }

            k = 0;
            while (sym_codes[original_file[j]].cod[k] != '\0') {
                code[b] = sym_codes[original_file[j]].cod[k];
                k++;
                b++;
            }

            j++;
        }

        printing_encoded_sequence(b, code, outfile);
    }
}