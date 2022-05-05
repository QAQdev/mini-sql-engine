//
// Created by Chengjun Ying on 2022/4/5.
//
#ifndef MINISQL_TREE_FILE_MGR_H
#define MINISQL_TREE_FILE_MGR_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

class TreeFileManagers {
public:
  explicit TreeFileManagers(std::string file_prefix) : file_prefix_(file_prefix) {};

  ~TreeFileManagers() {
    for (auto e : outs_) {
      close_file(e.first);
      delete e.second;
    }
  }

  std::ofstream &operator[](int id) {
    if (outs_.find(id) == outs_.end()) {
      new_out_stream(id);
    }
    return *(outs_[id]);
  }

private:
  void open_file(std::string file_name, std::ofstream &out) {
    remove(file_name.c_str());
    out.open(file_name, std::ios::in | std::ios::out);
    if (!out.is_open()) {
      out.clear();
      // create a new file
      out.open(file_name, std::ios::in | std::ios::trunc | std::ios::out);
      out.close();
      // reopen with original mode
      out.open(file_name, std::ios::in | std::ios::out);
      if (!out.is_open()) {
        throw std::exception();
      }
    }
  }

  void close_file(int id) {
    if (outs_.find(id) != outs_.end()) {
      outs_[id]->flush();
      outs_[id]->close();
    }
  }

  void new_out_stream(int id) {
    if (outs_.find(id) == outs_.end()) {
      std::stringstream ss;
      std::string filename;
      ss << file_prefix_ << std::to_string(id) << ".txt";
      ss >> filename;
      std::ofstream *out = new std::ofstream;
      open_file(filename, *out);
      outs_[id] = out;
    }
  }

private:
  std::map<int, std::ofstream *> outs_;
  std::string file_prefix_;
};

#endif //MINISQL_TREE_FILE_MGR_H
