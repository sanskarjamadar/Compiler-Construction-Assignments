%{
#include <stdio.h>
int line_count = 0;
int word_count = 0;
int char_count = 0;
%}

%%
\n      { line_count++; }
[a-zA-Z]+   { word_count++; char_count += strlen(yytext); }
.       { char_count++; }
%%

int main(int argc, char* argv[]) {
    yylex();
    printf("Lines: %d\nWords: %d\nCharacters: %d\n", line_count, word_count, char_count);
    return 0;
}
