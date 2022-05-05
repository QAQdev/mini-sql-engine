%{
  #include <stdio.h>
  #include "parser/parser.h"

  extern char *yytext;
  extern int yylex(void);
  int yyerror(char* error);
%}

%union {
	pSyntaxNode syntax_node;
}

%token <syntax_node> CREATE DROP SELECT INSERT DELETE UPDATE
%token <syntax_node> TRXBEGIN TRXCOMMIT TRXROLLBACK QUIT EXECFILE SHOW USE USING
%token <syntax_node> DATABASE DATABASES TABLE TABLES INDEX INDEXES
%token <syntax_node> ON FROM WHERE INTO SET VALUES PRIMARY KEY UNIQUE
%token <syntax_node> CHAR INT FLOAT AND OR NOT IS FLAGNULL
%token <syntax_node> IDENTIFIER STRING NUMBER EQ NE LE GE

%type <syntax_node> start sql
%type <syntax_node> sql_create_database sql_drop_database sql_show_databases sql_use_database
%type <syntax_node> sql_show_tables sql_create_table sql_drop_table
%type <syntax_node> column_definition_list column_definition column_type column_list
%type <syntax_node> sql_create_index sql_drop_index sql_show_indexes
%type <syntax_node> sql_trx_begin sql_trx_commit sql_trx_rollback
%type <syntax_node> sql_select select_columns column_values column_value operator
%type <syntax_node> connector where_conditions where_condition
%type <syntax_node> sql_insert sql_delete sql_update update_values update_value
%type <syntax_node> sql_quit sql_exec_file

%%

start:
  sql ';' {
    $$ = $1;
    MinisqlParserSetRoot($$);
  }
  ;

sql:
  sql_create_database { $$ = $1; }
  | sql_drop_database { $$ = $1; }
  | sql_show_databases { $$ = $1; }
  | sql_use_database { $$ = $1; }
  | sql_show_tables { $$ = $1; }
  | sql_create_table { $$ = $1; }
  | sql_drop_table { $$ = $1; }
  | sql_create_index { $$ = $1; }
  | sql_drop_index { $$ = $1; }
  | sql_show_indexes { $$ = $1; }
  | sql_select { $$ = $1; }
  | sql_insert { $$ = $1; }
  | sql_delete { $$ = $1; }
  | sql_update { $$ = $1; }
  | sql_trx_begin { $$ = $1; }
  | sql_trx_commit { $$ = $1; }
  | sql_trx_rollback { $$ = $1; }
  | sql_quit { $$ = $1; }
  | sql_exec_file { $$ = $1; }
  ;

sql_create_database:
  CREATE DATABASE IDENTIFIER {
    $$ = CreateSyntaxNode(kNodeCreateDB, NULL);
    SyntaxNodeAddChildren($$, $3);
  }
  ;

sql_drop_database:
  DROP DATABASE IDENTIFIER {
    $$ = CreateSyntaxNode(kNodeDropDB, NULL);
    SyntaxNodeAddChildren($$, $3);
  }
  ;

sql_show_databases:
  SHOW DATABASES {
    $$ = CreateSyntaxNode(kNodeShowDB, NULL);
  }
  ;

sql_use_database:
  USE IDENTIFIER {
    $$ = CreateSyntaxNode(kNodeUseDB, NULL);
    SyntaxNodeAddChildren($$, $2);
  }
  ;

sql_show_tables:
  SHOW TABLES {
    $$ = CreateSyntaxNode(kNodeShowTables, NULL);
  }
  ;

sql_create_table:
  CREATE TABLE IDENTIFIER '(' column_definition_list ')' {
    $$ = CreateSyntaxNode(kNodeCreateTable, NULL);
    pSyntaxNode list_node = CreateSyntaxNode(kNodeColumnDefinitionList, NULL);
    SyntaxNodeAddChildren(list_node, $5);
    SyntaxNodeAddChildren($$, $3);
    SyntaxNodeAddChildren($$, list_node);
  }
  ;

column_list:
  IDENTIFIER ',' column_list {
    $$ = $1;
    SyntaxNodeAddSibling($$, $3);
  }
  | IDENTIFIER {
    $$ = $1;
  }
  ;

column_definition_list:
  column_definition ',' column_definition_list {
    $$ = $1;
    SyntaxNodeAddSibling($$, $3);
  }
  | column_definition {
    $$ = $1;
  }
  | PRIMARY KEY '(' column_list ')' {
    $$ = CreateSyntaxNode(kNodeColumnList, "primary keys");
    SyntaxNodeAddChildren($$, $4);
  }
  ;

column_definition:
  IDENTIFIER column_type UNIQUE {
    $$ = CreateSyntaxNode(kNodeColumnDefinition, "unique");
    SyntaxNodeAddChildren($$, $1);
    SyntaxNodeAddChildren($$, $2);
  }
  | IDENTIFIER column_type {
    $$ = CreateSyntaxNode(kNodeColumnDefinition, NULL);
    SyntaxNodeAddChildren($$, $1);
    SyntaxNodeAddChildren($$, $2);
  }
  ;

column_type:
  INT {
    $$ = CreateSyntaxNode(kNodeColumnType, "int");
  }
  | FLOAT {
    $$ = CreateSyntaxNode(kNodeColumnType, "float");
  }
  | CHAR '(' NUMBER ')' {
    $$ = CreateSyntaxNode(kNodeColumnType, "char");
    SyntaxNodeAddChildren($$, $3);
  }
  ;

sql_drop_table:
  DROP TABLE IDENTIFIER {
    $$ = CreateSyntaxNode(kNodeDropTable, NULL);
    SyntaxNodeAddChildren($$, $3);
  }
  ;

sql_create_index:
  CREATE INDEX IDENTIFIER ON IDENTIFIER '(' column_list ')' {
    $$ = CreateSyntaxNode(kNodeCreateIndex, NULL);
    SyntaxNodeAddChildren($$, $3);
    SyntaxNodeAddChildren($$, $5);
    pSyntaxNode index_keys_node = CreateSyntaxNode(kNodeColumnList, "index keys");
    SyntaxNodeAddChildren(index_keys_node, $7);
    SyntaxNodeAddChildren($$, index_keys_node);
  }
  | CREATE INDEX IDENTIFIER ON IDENTIFIER '(' column_list ')' USING IDENTIFIER {
      $$ = CreateSyntaxNode(kNodeCreateIndex, NULL);
      SyntaxNodeAddChildren($$, $3);
      SyntaxNodeAddChildren($$, $5);
      pSyntaxNode index_keys_node = CreateSyntaxNode(kNodeColumnList, "index keys");
      SyntaxNodeAddChildren(index_keys_node, $7);
      SyntaxNodeAddChildren($$, index_keys_node);
      pSyntaxNode index_type_node = CreateSyntaxNode(kNodeIndexType, "index type");
      SyntaxNodeAddChildren(index_type_node, $10);
      SyntaxNodeAddChildren($$, index_type_node);
  }
  ;

sql_drop_index:
  DROP INDEX IDENTIFIER {
    $$ = CreateSyntaxNode(kNodeDropIndex, NULL);
    SyntaxNodeAddChildren($$, $3);
  }
  ;

sql_show_indexes:
  SHOW INDEXES {
    $$ = CreateSyntaxNode(kNodeShowIndexes, NULL);
  }
  ;

sql_select:
  SELECT select_columns FROM IDENTIFIER {
    $$ = CreateSyntaxNode(kNodeSelect, NULL);
    SyntaxNodeAddChildren($$, $2);
    SyntaxNodeAddChildren($$, $4);
  }
  | SELECT select_columns FROM IDENTIFIER WHERE where_conditions {
    $$ = CreateSyntaxNode(kNodeSelect, NULL);
    SyntaxNodeAddChildren($$, $2);
    SyntaxNodeAddChildren($$, $4);
    pSyntaxNode condition_node = CreateSyntaxNode(kNodeConditions, NULL);
    SyntaxNodeAddChildren(condition_node, $6);
    SyntaxNodeAddChildren($$, condition_node);
  }
  ;

select_columns:
  '*' {
    $$ = CreateSyntaxNode(kNodeAllColumns, NULL);
  }
  | column_list {
    $$ = CreateSyntaxNode(kNodeColumnList, "select columns");
    SyntaxNodeAddChildren($$, $1);
  }
  ;

where_conditions:
  where_conditions connector where_condition  {
    $$ = $2;
    SyntaxNodeAddChildren($$, $1);
    SyntaxNodeAddChildren($$, $3);
  }
  | where_condition {
    $$ = $1;
  }
  ;

connector:
  AND {
    $$ = CreateSyntaxNode(kNodeConnector, "and");
  }
  | OR {
    $$ = CreateSyntaxNode(kNodeConnector, "or");
  }
  ;

where_condition:
  IDENTIFIER operator column_value {
    $$ = $2;
    SyntaxNodeAddChildren($$, $1);
    SyntaxNodeAddChildren($$, $3);
  }
  ;

column_value:
  STRING {
    $$ = $1;
  }
  | NUMBER {
    $$ = $1;
  }
  | FLAGNULL {
    $$ = CreateSyntaxNode(kNodeNull, NULL);
  }
  ;

operator:
  EQ {
    $$ = CreateSyntaxNode(kNodeCompareOperator, "=");
  }
  | NE {
    $$ = CreateSyntaxNode(kNodeCompareOperator, "<>");
  }
  | LE {
    $$ = CreateSyntaxNode(kNodeCompareOperator, "<=");
  }
  | GE {
    $$ = CreateSyntaxNode(kNodeCompareOperator, ">=");
  }
  | '<' {
    $$ = CreateSyntaxNode(kNodeCompareOperator, "<");
  }
  | '>' {
    $$ = CreateSyntaxNode(kNodeCompareOperator, ">");
  }
  | IS {
    $$ = CreateSyntaxNode(kNodeCompareOperator, "is");
  }
  | NOT {
    $$ = CreateSyntaxNode(kNodeCompareOperator, "not");
  }
  ;

sql_insert:
  INSERT INTO IDENTIFIER VALUES '(' column_values ')' {
    $$ = CreateSyntaxNode(kNodeInsert, NULL);
    SyntaxNodeAddChildren($$, $3);
    pSyntaxNode col_val_node = CreateSyntaxNode(kNodeColumnValues, NULL);
    SyntaxNodeAddChildren(col_val_node, $6);
    SyntaxNodeAddChildren($$, col_val_node);
  }
  ;

column_values:
  column_value ',' column_values {
    $$ = $1;
    SyntaxNodeAddSibling($$, $3);
  }
  | column_value {
    $$ = $1;
  }
  ;

sql_delete:
  DELETE FROM IDENTIFIER {
    $$ = CreateSyntaxNode(kNodeDelete, NULL);
    SyntaxNodeAddChildren($$, $3);
  }
  | DELETE FROM IDENTIFIER WHERE where_conditions {
    $$ = CreateSyntaxNode(kNodeDelete, NULL);
    SyntaxNodeAddChildren($$, $3);
    pSyntaxNode condition_node = CreateSyntaxNode(kNodeConditions, NULL);
    SyntaxNodeAddChildren(condition_node, $5);
    SyntaxNodeAddChildren($$, condition_node);
  }
  ;

sql_update:
  UPDATE IDENTIFIER SET update_values {
    $$ = CreateSyntaxNode(kNodeUpdate, NULL);
    SyntaxNodeAddChildren($$, $2);
    pSyntaxNode upd_values_node = CreateSyntaxNode(kNodeUpdateValues, NULL);
    SyntaxNodeAddChildren(upd_values_node, $4);
    SyntaxNodeAddChildren($$, upd_values_node);
  }
  | UPDATE IDENTIFIER SET update_values WHERE where_conditions {
    $$ = CreateSyntaxNode(kNodeUpdate, NULL);
    SyntaxNodeAddChildren($$, $2);
    // update values
    pSyntaxNode upd_values_node = CreateSyntaxNode(kNodeUpdateValues, NULL);
    SyntaxNodeAddChildren(upd_values_node, $4);
    SyntaxNodeAddChildren($$, upd_values_node);
    // where conditions
    pSyntaxNode condition_node = CreateSyntaxNode(kNodeConditions, NULL);
    SyntaxNodeAddChildren(condition_node, $6);
    SyntaxNodeAddChildren($$, condition_node);
  }
  ;

update_values:
  update_value ',' update_values {
    $$ = $1;
    SyntaxNodeAddSibling($$, $3);
  }
  | update_value {
    $$ = $1;
  }
  ;

update_value:
  IDENTIFIER EQ column_value {
    $$ = CreateSyntaxNode(kNodeUpdateValue, NULL);
    SyntaxNodeAddChildren($$, $1);
    SyntaxNodeAddChildren($$, $3);
  }
  ;

sql_trx_begin:
  TRXBEGIN {
    $$ = CreateSyntaxNode(kNodeTrxBegin, NULL);
  }
  ;

sql_trx_commit:
  TRXCOMMIT {
    $$ = CreateSyntaxNode(kNodeTrxCommit, NULL);
  }
  ;

sql_trx_rollback:
  TRXROLLBACK {
    $$ = CreateSyntaxNode(kNodeTrxRollback, NULL);
  }
  ;

sql_quit:
  QUIT {
    $$ = CreateSyntaxNode(kNodeQuit, NULL);
  }
  ;

sql_exec_file:
  EXECFILE STRING {
    $$ = CreateSyntaxNode(kNodeExecFile, NULL);
    SyntaxNodeAddChildren($$, $2);
  }
  ;

%%
int yyerror(char* error) {
	MinisqlParserSetError(error);
	return 0;
}