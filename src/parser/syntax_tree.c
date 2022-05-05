#include <stdio.h>
#include "parser/syntax_tree.h"

extern int minisql_parser_line_no_;
extern int minisql_parser_column_no_;
extern int minisql_parser_debug_node_count_;
extern pSyntaxNodeList minisql_parser_syntax_node_list_;

pSyntaxNode CreateSyntaxNode(SyntaxNodeType type, char *val) {
  pSyntaxNode node = (pSyntaxNode) malloc(sizeof(struct SyntaxNode));
  node->id_ = minisql_parser_debug_node_count_++;
  node->type_ = type;
  node->line_no_ = minisql_parser_line_no_;
  node->col_no_ = minisql_parser_column_no_;
  node->child_ = NULL;
  node->next_ = NULL;
  // deep copy
  if (val != NULL) {
    // special for string, remove ""
    if (type == kNodeString) {
      size_t len = strlen(val) - 1;   // -2 + 1
      node->val_ = (char *) malloc(len);
      strncpy(node->val_, val + 1, len - 1);
      node->val_[len] = '\0';
    } else {
      size_t len = strlen(val) + 1;
      node->val_ = (char *) malloc(len);
      strcpy(node->val_, val);
      node->val_[len] = '\0';
    }
  } else {
    node->val_ = NULL;
  }
  // linked to syntax node list
  pSyntaxNodeList list_node = (pSyntaxNodeList) malloc(sizeof(struct SyntaxNodeList));
  list_node->node_ = node;
  if (minisql_parser_syntax_node_list_ == NULL) {
    minisql_parser_syntax_node_list_ = list_node;
    list_node->next_ = NULL;
  } else {
    list_node->next_ = minisql_parser_syntax_node_list_;
    minisql_parser_syntax_node_list_ = list_node;
  }
#ifdef ENABLE_PARSER_DEBUG
  printf("Create syntax node: node_id = %d, type = %s, line = %d, col = %d\n", node->id_,
         GetSyntaxNodeTypeStr(node->type_), node->line_no_, node->col_no_);
#endif
  return node;
}

void FreeSyntaxNode(pSyntaxNode node) {
  if (node != NULL) {
#ifdef ENABLE_PARSER_DEBUG
    printf("Free syntax node: node_id = %d\n", node->id_);
#endif
    if (node->val_ != NULL) {
      free(node->val_);
    }
    free(node);
  }
}

void DestroySyntaxTree() {
  pSyntaxNodeList p = minisql_parser_syntax_node_list_;
  while (p != NULL) {
    pSyntaxNodeList next = p->next_;
    FreeSyntaxNode(p->node_);
    p = next;
  }
  minisql_parser_syntax_node_list_ = NULL;
}

void SyntaxNodeAddChildren(pSyntaxNode parent, pSyntaxNode child) {
  if (parent->child_ == NULL) {
    parent->child_ = child;
    return;
  }
  pSyntaxNode p = parent->child_;
  while (p->next_ != NULL) {
    p = p->next_;
  }
  p->next_ = child;
}

void SyntaxNodeAddSibling(pSyntaxNode node, pSyntaxNode sib) {
  if (node == NULL || sib == NULL) {
    return;
  }
  if (node->next_ == NULL) {
    node->next_ = sib;
    return;
  }
  pSyntaxNode p = node->next_;
  while (p->next_ != NULL) {
    p = p->next_;
  }
  p->next_ = sib;
}

const char *GetSyntaxNodeTypeStr(SyntaxNodeType type) {
  switch (type) {
    case kNodeUnknown:
      return "kNodeUnknown";
    case kNodeQuit:
      return "kNodeQuit";
    case kNodeExecFile:
      return "kNodeExecFile";
    case kNodeIdentifier:
      return "kNodeIdentifier";
    case kNodeNumber:
      return "kNodeNumber";
    case kNodeString:
      return "kNodeString";
    case kNodeNull:
      return "kNodeNull";
    case kNodeCreateDB:
      return "kNodeCreateDB";
    case kNodeDropDB:
      return "kNodeDropDB";
    case kNodeShowDB:
      return "kNodeShowDB";
    case kNodeUseDB:
      return "kNodeUseDB";
    case kNodeShowTables:
      return "kNodeShowTables";
    case kNodeCreateTable:
      return "kNodeCreateTable";
    case kNodeDropTable:
      return "kNodeDropTable";
    case kNodeShowIndexes:
      return "kNodeShowIndexes";
    case kNodeInsert:
      return "kNodeInsert";
    case kNodeDelete:
      return "kNodeDelete";
    case kNodeUpdate:
      return "kNodeUpdate";
    case kNodeSelect:
      return "kNodeSelect";
    case kNodeConditions:
      return "kNodeConditions";
    case kNodeConnector:
      return "kNodeConnector";
    case kNodeCompareOperator:
      return "kNodeCompareOperator";
    case kNodeColumnType:
      return "kNodeColumnType";
    case kNodeColumnDefinition:
      return "kNodeColumnDefinition";
    case kNodeColumnDefinitionList:
      return "kNodeColumnDefinitionList";
    case kNodeColumnList:
      return "kNodeColumnList";
    case kNodeColumnValues:
      return "kNodeColumnValues";
    case kNodeUpdateValues:
      return "kNodeUpdateValues";
    case kNodeUpdateValue:
      return "kNodeUpdateValue";
    case kNodeAllColumns:
      return "kNodeAllColumns";
    case kNodeCreateIndex:
      return "kNodeCreateIndex";
    case kNodeDropIndex:
      return "kNodeDropIndex";
    case kNodeTrxBegin:
      return "kNodeTrxBegin";
    case kNodeTrxCommit:
      return "kNodeTrxCommit";
    case kNodeTrxRollback:
      return "kNodeTrxRollback";
    default:
      return "error type";
  }
}
