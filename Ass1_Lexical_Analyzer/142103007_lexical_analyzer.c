#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void recognizeToken(FILE *fout, char *token, int line, int *tokenNo) {
    char *keywords[] = {"int", "float", "void", "main", "return", "if", "else", "while", "for", "do", NULL};
    char *operators[] = {"+", "-", "*", "/", "=", "==", "<", ">", "<=", ">=", "&&", "||", "!", NULL};

    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, *tokenNo, "Keyword", token);
            (*tokenNo)++;
            return;
        }
    }

    for (int i = 0; operators[i] != NULL; i++) {
        if (strcmp(token, operators[i]) == 0) {
            fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, *tokenNo, "Operator", token);
            (*tokenNo)++;
            return;
        }
    }

    if (isalpha(token[0]) || token[0] == '_') {
        fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, *tokenNo, "Identifier", token);
        (*tokenNo)++;
        return;
    }
}

int main() {
    FILE *fin = fopen("input.txt", "r");
    FILE *fout = fopen("output.txt", "w");

    if (fin == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }

    if (fout == NULL) {
        printf("Failed to open output file.\n");
        return 1;
    }

    char ch, buffer[256];
    int line = 1, tokenNo = 0, j = 0;
    int inComment = 0;

    fprintf(fout, "%-10s%-15s%-25s%-25s\n", "Line no.", "Token no.", "Token", "Lexeme");

    while ((ch = fgetc(fin)) != EOF) {
        if (ch == '\n') {
            line++;
            inComment = 0;
            continue;
        }

        if (inComment) {
            continue;
        }

        if (isalnum(ch) || ch == '_') {
            buffer[j++] = ch;
        } else {
            if (j > 0) {
                buffer[j] = '\0';
                recognizeToken(fout, buffer, line, &tokenNo);
                j = 0;
            }

            if (ch == '(') {
                fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, tokenNo, "Opening bracket", "(");
                tokenNo++;
            } else if (ch == ')') {
                fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, tokenNo, "Closing bracket", ")");
                tokenNo++;
            } else if (ch == '{') {
                fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, tokenNo, "Opening brace", "{");
                tokenNo++;
            } else if (ch == '}') {
                fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, tokenNo, "Closing brace", "}");
                tokenNo++;
            } else if (ch == ';') {
                fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, tokenNo, "Semicolon", ";");
                tokenNo++;
            } else if (ch == '"') {
                buffer[j++] = ch;
                while ((ch = fgetc(fin)) != '"') {
                    buffer[j++] = ch;
                }
                buffer[j++] = ch;
                buffer[j] = '\0';
                fprintf(fout, "%-10d%-15d%-25s%-25s\n", line, tokenNo, "Literal", buffer);
                tokenNo++;
                j = 0;
            } else if (ch == '/') {
                ch = fgetc(fin);
                if (ch == '/') {
                    inComment = 1;
                } else {
                    recognizeToken(fout, "/", line, &tokenNo);
                    ungetc(ch, fin);  // Put the character back into the stream
                }
            } else {
                buffer[j++] = ch;
                buffer[j] = '\0';
                recognizeToken(fout, buffer, line, &tokenNo);
                j = 0;
            }
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
