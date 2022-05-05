#include <stdio.h>
#include "parser/parser.h"
#include "parser/syntax_tree.h"

pSyntaxNode minisql_parser_root_node_ = NULL;
pSyntaxNodeList minisql_parser_syntax_node_list_ = NULL;
int minisql_parser_line_no_ = 0;
int minisql_parser_column_no_ = 0;
int minisql_parser_error_ = 0;
char *minisql_parser_error_message_ = NULL;
int minisql_parser_debug_node_count_ = 0;

void MinisqlParserMovePos(int line, char *text) {
  size_t i = 0;
  while (i < strlen(text)) {
    char ch = text[i];
    switch (ch) {
      case '\n':
        minisql_parser_column_no_ = 0;
        minisql_parser_line_no_++;
        break;
      case '\t':
        minisql_parser_column_no_ += 4 - (minisql_parser_column_no_ % 4);
        break;
      default:
        minisql_parser_column_no_++;
    }
    i++;
  }
}

void MinisqlParserSetRoot(pSyntaxNode node) {
  minisql_parser_root_node_ = node;
}

void MinisqlParserSetError(char *msg) {
  if (minisql_parser_error_) {
    return;
  }
  minisql_parser_error_ = 1;
  if (minisql_parser_error_message_ != NULL) {
    printf("minisql parse error message not null in MinisqlParserSetError.\n");
  }
  printf("Minisql parse error at line %d, col %d, message: %s\n", minisql_parser_line_no_, minisql_parser_column_no_,
         msg);
  minisql_parser_error_message_ = msg;
}

pSyntaxNode MinisqlGetParserRootNode() {
  return minisql_parser_root_node_;
}

void MinisqlParserInit() {
  minisql_parser_root_node_ = NULL;
  minisql_parser_line_no_ = 1;
  minisql_parser_column_no_ = 0;
  minisql_parser_error_ = 0;
  minisql_parser_error_message_ = NULL;
  minisql_parser_debug_node_count_ = 0;
}

void MinisqlParserFinish() {
  DestroySyntaxTree();
}

int MinisqlParserGetError() {
  return minisql_parser_error_;
}

char *MinisqlParserGetErrorMessage() {
  return minisql_parser_error_message_;
}