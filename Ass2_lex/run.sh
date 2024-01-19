lex word_count.lex
gcc lex.yy.c -o word_count -ll
./word_count < input.txt
