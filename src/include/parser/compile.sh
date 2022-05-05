lex --header-file=./minisql_lex.h --outfile=../../parser/minisql_lex.c minisql.l \
&& yacc -d -o ./minisql_yacc.c minisql.y \
&& mv minisql_yacc.c ../../parser/minisql_yacc.c