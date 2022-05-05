%{
    #include <stdio.h>
    #include "parser/parser.h"
    #include "parser/minisql_yacc.h"
    int yywrap();
    extern YYSTYPE yylval;
%}

%option yylineno

L			[a-zA-Z_]
D			[0-9]
LD    {L}|{D}

%%

\"(\\.|[^"\\])*\" {
  MinisqlParserMovePos(yylineno, yytext);
  yylval.syntax_node = CreateSyntaxNode(kNodeString, yytext);
  return STRING;
}

"create"  {
  MinisqlParserMovePos(yylineno, yytext);
  return CREATE;
}

"drop" {
  MinisqlParserMovePos(yylineno, yytext);
  return DROP;
}

"select" {
  MinisqlParserMovePos(yylineno, yytext);
  return SELECT;
}

"insert" {
  MinisqlParserMovePos(yylineno, yytext);
  return INSERT;
}

"delete" {
  MinisqlParserMovePos(yylineno, yytext);
  return DELETE;
}

"update" {
  MinisqlParserMovePos(yylineno, yytext);
  return UPDATE;
}

"begin" {
  MinisqlParserMovePos(yylineno, yytext);
  return TRXBEGIN;
}

"commit" {
  MinisqlParserMovePos(yylineno, yytext);
  return TRXCOMMIT;
}

"rollback" {
  MinisqlParserMovePos(yylineno, yytext);
  return TRXROLLBACK;
}

"quit" {
  MinisqlParserMovePos(yylineno, yytext);
  return QUIT;
}

"execfile" {
  MinisqlParserMovePos(yylineno, yytext);
  return EXECFILE;
}

"show" {
  MinisqlParserMovePos(yylineno, yytext);
  return SHOW;
}

"use" {
  MinisqlParserMovePos(yylineno, yytext);
  return USE;
}

"using" {
  MinisqlParserMovePos(yylineno, yytext);
  return USING;
}

"database"  {
  MinisqlParserMovePos(yylineno, yytext);
  return DATABASE;
}

"databases" {
  MinisqlParserMovePos(yylineno, yytext);
  return DATABASES;
}

"table" {
  MinisqlParserMovePos(yylineno, yytext);
  return TABLE;
}

"tables" {
  MinisqlParserMovePos(yylineno, yytext);
  return TABLES;
}

"index" {
  MinisqlParserMovePos(yylineno, yytext);
  return INDEX;
}

"indexes" {
  MinisqlParserMovePos(yylineno, yytext);
  return INDEXES;
}

"on" {
  MinisqlParserMovePos(yylineno, yytext);
  return ON;
}

"from"  {
  MinisqlParserMovePos(yylineno, yytext);
  return FROM;
}

"where" {
  MinisqlParserMovePos(yylineno, yytext);
  return WHERE;
}

"into"  {
  MinisqlParserMovePos(yylineno, yytext);
  return INTO;
}

"set" {
  MinisqlParserMovePos(yylineno, yytext);
  return SET;
}

"values"  {
  MinisqlParserMovePos(yylineno, yytext);
  return VALUES;
}

"primary" {
  MinisqlParserMovePos(yylineno, yytext);
  return PRIMARY;
}

"key" {
  MinisqlParserMovePos(yylineno, yytext);
  return KEY;
}

"unique"  {
  MinisqlParserMovePos(yylineno, yytext);
  return UNIQUE;
}

"char"  {
  MinisqlParserMovePos(yylineno, yytext);
  return CHAR;
}

"int" {
  MinisqlParserMovePos(yylineno, yytext);
  return INT;
}

"float" {
  MinisqlParserMovePos(yylineno, yytext);
  return FLOAT;
}

"and" {
  MinisqlParserMovePos(yylineno, yytext);
  return AND;
}

"or" {
  MinisqlParserMovePos(yylineno, yytext);
  return OR;
}

"not" {
  MinisqlParserMovePos(yylineno, yytext);
  return NOT;
}

"is"  {
  MinisqlParserMovePos(yylineno, yytext);
  return IS;
}

"null"  {
  MinisqlParserMovePos(yylineno, yytext);
  return FLAGNULL;
}

{L}{LD}*  {
  MinisqlParserMovePos(yylineno, yytext);
  yylval.syntax_node = CreateSyntaxNode(kNodeIdentifier, yytext);
  return IDENTIFIER;
}

[-]?{D}*\.{D}+ {
  MinisqlParserMovePos(yylineno, yytext);
  yylval.syntax_node = CreateSyntaxNode(kNodeNumber, yytext);
  return NUMBER;
}

[-]?{D}* {
  MinisqlParserMovePos(yylineno, yytext);
  yylval.syntax_node = CreateSyntaxNode(kNodeNumber, yytext);
  return NUMBER;
}

"=" {
  MinisqlParserMovePos(yylineno, yytext);
  return EQ;
}

"<>" {
  MinisqlParserMovePos(yylineno, yytext);
  return NE;
}

"<=" {
  MinisqlParserMovePos(yylineno, yytext);
  return LE;
}

">=" {
  MinisqlParserMovePos(yylineno, yytext);
  return GE;
}

"," {
  MinisqlParserMovePos(yylineno, yytext);
  return (',');
}

"*" {
  MinisqlParserMovePos(yylineno, yytext);
  return ('*');
}

";" {
  MinisqlParserMovePos(yylineno, yytext);
  return (';');
}

"'" {
  MinisqlParserMovePos(yylineno, yytext);
  return ('\'');
}

"<" {
  MinisqlParserMovePos(yylineno, yytext);
  return ('<');
}

">" {
  MinisqlParserMovePos(yylineno, yytext);
  return ('>');
}

"(" {
  MinisqlParserMovePos(yylineno, yytext);
  return ('(');
}

")" {
  MinisqlParserMovePos(yylineno, yytext);
  return (')');
}

[ \t\v\n\f] {
  MinisqlParserMovePos(yylineno, yytext);
}

. {
  char str[128] = {0};
  sprintf(str, "Unrecognized token [%s] in input sql.", yytext);
  MinisqlParserSetError(str);
}

%%
int yywrap() {
	return 1;
}