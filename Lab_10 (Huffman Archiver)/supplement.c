#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

struct node *new_tree_node(int key, int amount) {
    struct node *new_1 = (struct node *) malloc(sizeof(struct node));

    new_1->key = key;
    new_1->times = amount;
    new_1->left = NULL;
    new_1->right = NULL;

    return new_1;
}

struct node *stick_tree_node(struct node *l, struct node *r, int amount) {
    struct node *new_1 = (struct node *) malloc(sizeof(struct node));

    new_1->times = amount;
    new_1->left = l;
    new_1->right = r;

    return new_1;
}

struct nodes_pointers *connecting_nodes(int h, struct node *initial_nodes) {
    struct nodes_pointers *set_tree = (struct nodes_pointers *) malloc(sizeof(struct nodes_pointers) * h);

    for (int i = 0; i < h; i++) {
        set_tree[i].p = new_tree_node(initial_nodes[i].key, initial_nodes[i].times);
    }

    while (h != 1) {
        set_tree[1].p = stick_tree_node(set_tree[0].p, set_tree[1].p, set_tree[0].p->times + set_tree[1].p->times);
        for (int i = 1; i < h; i++)
            set_tree[i - 1] = set_tree[i];
        h--;
        for (int i = 0; i < h - 1; i++) {
            for (int k = i + 1; k < h; k++) {
                if (set_tree[i].p->times > set_tree[k].p->times) {
                    struct node *temp = set_tree[i].p;
                    set_tree[i].p = set_tree[k].p;
                    set_tree[k].p = temp;
                }
            }
        }
    }

    return set_tree;
}

void print_help() {
    printf("USAGE:\n [for decoding]: -d inputfilename outputfilename \n "
                   "[for encoding]: -c inputfilename outputfrilename \n "
                   "(!) inputfilename must not be empty");
}

void encoding_symbols(struct node *p, char *seq, int *j, struct sequence *arr3, int *max) {
    if (p->left != NULL) {
        seq[*j] = '0';
        (*j)++;
        encoding_symbols(p->left, seq, j, arr3, max);
    }

    if (p->right != NULL) {
        seq[*j] = '1';
        (*j)++;
        encoding_symbols(p->right, seq, j, arr3, max);
    } else {
        if (*j > *max) {
            *max = *j;
        }
        for (int v = 0; v < *j; v++) {
            arr3[p->key].cod[v] = seq[v];
            arr3[p->key].otm = 0;
        }
    }
    seq[*j] = '\0';
    (*j)--;
}

void finding_commands(int argc, char *argv[], int *c_cmd, int *d_cmd, int *check_fail) {
    for (int i = 1; i < argc - 2; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'c':
                    *c_cmd = 1;
                    break;
                case 'd':
                    *d_cmd = 1;
                    break;
                default:
                    *check_fail = 1;
                    break;
            }
        } else {
            *check_fail = 1;
        }
    }
}

void correcting_bytes(int *h, int *d, int *code_length, int *f) {
    (*h)++;
    (*d)--;
    if (*h / 8 > 0) {
        *h = 0;
        *code_length -= 8;
        (*f)++;
        (*d) = 7;
    }
}

void restoring_tree(struct node *p, unsigned char *ar, int *h, int *code_length, unsigned char r, int *f, int *d, int *m) {
    *m = 0;
    while (*h < *code_length) {
        if ((ar[*f] & (1 << *d)) == 0) {
            if (p->left == NULL) {
                p->left = new_tree_node(-2, 0);
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->left, ar, h, code_length, r, f, d, m);
            } else {
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->left, ar, h, code_length, r, f, d, m);
            }
        } else if ((ar[*f] & (1 << *d)) == (1 << *d)) {
            if (p->right == NULL) {
                p->right = new_tree_node(-2, 0);
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->right, ar, h, code_length, r, f, d, m);
            } else {
                correcting_bytes(h, d, code_length, f);
                restoring_tree(p->right, ar, h, code_length, r, f, d, m);
            }
        }
        *m = 1;
    }
    if (*m == 0)
        p->key = r;
}

void printing_sym_code(struct sequence *sym_codes, const unsigned char *original_file, int j, FILE *outfile) {
    unsigned char buff = 0;
    int buff_size = 0;
    int k = 0;
    while (sym_codes[original_file[j]].cod[k] != '\0') {
        if (buff_size == 8) {
            fwrite(&buff, 1, 1, outfile);
            buff_size = 0;
            buff = 0;
        }

        buff = (unsigned char) (((buff << 1) & 254) | ((sym_codes[original_file[j]].cod[k] == '1') ? 1 : 0));
        buff_size++;
        k++;

        if ((sym_codes[original_file[j]].cod[k] == '\0') && (buff_size > 0)) {
            buff = buff << (8 - buff_size);
            fwrite(&buff, 1, 1, outfile);
        }
    }
}

void printing_encoded_sequence(int b, const char *code, FILE *outfile) {
    int j = 0;
    unsigned char buff = 0;
    int buff_size = 0;

    while (j < b) {
        if (buff_size == 8) {
            fwrite(&buff, 1, 1, outfile);
            buff_size = 0;
            buff = 0;
        }

        buff = (unsigned char) (((buff << 1) & 254) | ((code[j] == '1') ? 1 : 0));
        buff_size++;
        j++;

        if ((j == b) && (buff_size > 0)) {
            buff = buff << (8 - buff_size);
            fwrite(&buff, 1, 1, outfile);
        }
    }
}