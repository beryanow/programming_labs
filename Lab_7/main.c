#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char *output = (char *) malloc(strlen(argv[1]) * sizeof(char));
    char *stack = (char *) malloc(strlen(argv[1]) * sizeof(char));
    int m = 0;
    int k = 0;
    for (int i = 0; i < strlen(argv[1]); i++) {
        if ((argv[1][i] >= '0') && (argv[1][i] <= '9')) {
            output[m] = argv[1][i];
            m++;
        } else if ((argv[1][i] == '*') | (argv[1][i] == '/')) {
            while (1) {
                if ((stack[k - 1] == '*') | (stack[k - 1] == '/')) {
                    output[m] = stack[k - 1];
                    stack[k - 1] = argv[1][i];
                    m++;
                } else {
                    stack[k] = argv[1][i];
                    k++;
                    break;
                }
                k--;
            }
        } else if ((argv[1][i] == '+') | (argv[1][i] == '-')) {
            while (1) {
                if ((stack[k - 1] == '*') | (stack[k - 1] == '/') | (stack[k - 1] == '+') | (stack[k - 1] == '-')) {
                    output[m] = stack[k - 1];
                    stack[k - 1] = argv[1][i];
                    m++;
                } else {
                    stack[k] = argv[1][i];
                    k++;
                    break;
                }
                k--;
            }
        }
        else if (argv[1][i] == '(') {
            stack[k] = argv[1][i];
            k++;
        }
        else if (argv[1][i] == ')') {
            int c = k - 1, z;
            while (stack[k] != '(') k--;
            for (z = c; z > k; z--) {
                output[m] = stack[z];
                m++;
                stack[z] = '\0';
            }
            stack[z] = '\0';
            k = z;
        }
    }
    for (int i = k - 1; i >= 0; i--) {
        output[m] = stack[i];
        stack[i] = '\0';
        k--;
        m++;
    }
    for (int i = 0; i < m; i++) {
        printf("%c ", output[i]);
    }
    printf("\n");
    for (int i = 0; i < k; i++) {
        printf("%c ", stack[i]);
    }
    return 0;
}