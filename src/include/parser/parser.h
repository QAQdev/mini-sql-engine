/**
 * Created by Chengjun Ying on 2022/3/10.
 */
#ifndef MINISQL_PARSER_H
#define MINISQL_PARSER_H

#include "parser/syntax_tree.h"

void MinisqlParserMovePos(int line, char *text);

void MinisqlParserSetRoot(pSyntaxNode node);

void MinisqlParserSetError(char *msg);

pSyntaxNode MinisqlGetParserRootNode();

void MinisqlParserInit();

void MinisqlParserFinish();

int MinisqlParserGetError();

char *MinisqlParserGetErrorMessage();

#endif //MINISQL_PARSER_H
