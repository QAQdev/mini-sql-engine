/**
 * Created by Chengjun Ying on 2022/3/10.
 */
#ifndef MINISQL_SYNTAX_TREE_H
#define MINISQL_SYNTAX_TREE_H

#include <stdlib.h>
#include <string.h>

typedef enum {
  kNodeUnknown,
  kNodeQuit,  /** quit command */
  kNodeExecFile,  /** execfile command */
  kNodeIdentifier,  /** identifier for database_name, table_name, index_name, column_name... */
  kNodeNumber,  /** numeric value type, eg: int, float */
  kNodeString, /** string value type, eg: char */
  kNodeNull,  /** null value type */
  kNodeCreateDB, /** create database command */
  kNodeDropDB, /** drop database command */
  kNodeShowDB, /** show databases command */
  kNodeUseDB, /** use database command */
  kNodeShowTables, /** show tables command */
  kNodeCreateTable, /** create table command */
  kNodeDropTable, /** drop table command */
  kNodeShowIndexes,  /** show indexes command */
  kNodeInsert,  /** insert command */
  kNodeDelete,  /** delete command */
  kNodeUpdate,  /** update command */
  kNodeSelect,  /** select command */
  kNodeConditions,  /** where conditions */
  kNodeConnector, /** operators 'AND' and 'OR' */
  kNodeCompareOperator, /** operators '=', '<>', '<=', '>=', '<', '>', is, not */
  kNodeColumnType,  /** column types: int, char, float */
  kNodeColumnDefinition,   /** column definition, contains column identifier and column type and UNIQUE constraint */
  kNodeColumnDefinitionList,  /** contains several column definitions */
  kNodeColumnList,  /** column list, contains several columns, used in create table, create index and select */
  kNodeColumnValues, /** column values for insert operation */
  kNodeUpdateValues, /** column update values for update operation */
  kNodeUpdateValue, /** column update value for update operation */
  kNodeAllColumns,  /** '*', means select all columns, used in select */
  kNodeCreateIndex, /** create index command */
  kNodeDropIndex, /** drop index command */
  kNodeIndexType, /** type of index */
  kNodeTrxBegin, /** begin transaction command */
  kNodeTrxCommit, /** commit transaction command */
  kNodeTrxRollback /** rollback transaction command */
} SyntaxNodeType;

/**
 * Syntax node definition used in abstract syntax tree.
 */
struct SyntaxNode {
  int id_;    /** node id for allocated syntax node, used for debug */
  SyntaxNodeType type_; /** syntax node type */
  int line_no_; /** line number of this syntax node appears in sql */
  int col_no_;  /** column number of this syntax node appears in sql */
  struct SyntaxNode *child_;  /** children of this syntax node */
  struct SyntaxNode *next_;   /** siblings of this syntax node, linked by a single linked list */
  char *val_; /** attribute value of this syntax node, use deep copy */
};
typedef struct SyntaxNode *pSyntaxNode;

pSyntaxNode CreateSyntaxNode(SyntaxNodeType type, char *val);

/**
 * Free a syntax node and its attribute value, only called by DestroySyntaxTree
 */
void FreeSyntaxNode(pSyntaxNode node);

/**
 * Free all syntax tree, only called after parse
 */
void DestroySyntaxTree();

void SyntaxNodeAddChildren(pSyntaxNode parent, pSyntaxNode child);

void SyntaxNodeAddSibling(pSyntaxNode node, pSyntaxNode sib);

const char *GetSyntaxNodeTypeStr(SyntaxNodeType type);

/**
 * Linked list used to store all allocated syntax nodes
 */
struct SyntaxNodeList {
  struct SyntaxNodeList *next_;
  pSyntaxNode node_;
};
typedef struct SyntaxNodeList *pSyntaxNodeList;


#endif //MINISQL_SYNTAX_TREE_H
