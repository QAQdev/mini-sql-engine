//
// Created by Chengjun Ying on 2022/4/5.
//
#include <fstream>
#include <iostream>
#include <string>

#include "parser/syntax_tree_printer.h"

void SyntaxTreePrinter::PrintTree(std::ofstream &out) {
  if (root_ == nullptr) {
    out << "digraph G{ }" << std::endl;
    return;
  }
  out << "digraph G{ " << std::endl;
  PrintTreeLow(root_, out);
  out << "}" << std::endl;
}

void SyntaxTreePrinter::PrintTreeLow(pSyntaxNode node, std::ofstream &out) {
  out << "SYNTAX_NODE_" << node->id_ << "[label=\"";
  out << GetSyntaxNodeTypeStr(node->type_) << "(" << node->line_no_ << "," << node->col_no_ << ")";
  out << "\\n";
  out << "id(" << node->id_ << ")";
  if (node->val_ != nullptr) {
    out << ",val(" << node->val_ << ")";
  }
  out << "\"];" << std::endl;

  if (node->child_ != nullptr) {
    PrintTreeLow(node->child_, out);
    out << "SYNTAX_NODE_" << node->id_ << "  ->  " << "SYNTAX_NODE_" << node->child_->id_ << ";" << std::endl;
  }

  if (node->next_ != nullptr) {
    PrintTreeLow(node->next_, out);
    out << "SYNTAX_NODE_" << node->id_ << "  ->  " << "SYNTAX_NODE_" << node->next_->id_ << ";" << std::endl;
    out << "{rank=same; " << "SYNTAX_NODE_" << node->id_ << "," << "SYNTAX_NODE_" << node->next_->id_ << "};"
        << std::endl;
  }

}