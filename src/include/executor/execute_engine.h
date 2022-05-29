#ifndef MINISQL_EXECUTE_ENGINE_H
#define MINISQL_EXECUTE_ENGINE_H

#include <string>
#include <unordered_map>
#include "common/dberr.h"
#include "common/instance.h"
#include "transaction/transaction.h"
#include <vector>

extern "C" 
{
extern int yyparse(void);
extern FILE *yyin;
#include "../parser/minisql_lex.h"
#include "../parser/parser.h"
};

/**
 * ExecuteContext stores all the context necessary to run in the execute engine
 * This struct is implemented by student self for necessary.
 *
 * eg: transaction info, execute result...
 */
struct ExecuteContext 
{
  bool flag_quit_{false};
  Transaction *txn_{nullptr};
};

/**
 * ExecuteEngine
 */
class ExecuteEngine {
public:
  ExecuteEngine();

  ~ExecuteEngine() {
    for (auto it : dbs_) {
      delete it.second;
    }
  }

  /**
   * executor interface
   */
  dberr_t Execute(pSyntaxNode ast, ExecuteContext *context);

private:
  dberr_t ExecuteCreateDatabase(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteDropDatabase(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteShowDatabases(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteUseDatabase(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteShowTables(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteCreateTable(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteDropTable(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteShowIndexes(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteCreateIndex(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteDropIndex(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteSelect(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteInsert(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteDelete(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteUpdate(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteTrxBegin(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteTrxCommit(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteTrxRollback(pSyntaxNode ast, ExecuteContext *context);

  dberr_t ExecuteExecfile(pSyntaxNode ast, ExecuteContext *context);
 
  dberr_t ExecuteQuit(pSyntaxNode ast, ExecuteContext *context);

  std::vector<int64_t> FitchAllRows(std::string tablename);

  dberr_t SelectPerform(string table_name, string column_name,  
          string comparator_name, char* cval, std::vector<int64_t> org_rows, 
          std::vector<int64_t>& reslt_rows);

  std::vector<int64_t> RowIdMerge(std::vector<int64_t>, std::vector<int64_t>); 

  void SaveDBs();

  dberr_t WherePerform(pSyntaxNode where_node, string tablename, vector<int64_t> org_rows, vector<int64_t>& new_rows);

  void PrintRow(TableInfo* tmp_table_info, int64_t rowid, vector<string> print_columns);

  void PrintRows(TableInfo* tmp_table_info, vector<int64_t> rowids, 
                vector<string> print_columns);

  dberr_t GetField(pSyntaxNode ast, TableInfo* table_info);

  void ExeSelectAll(string tablename);

  bool ifhaveIndex(string tablename, string column_name, IndexInfo*& indexinfo);
  // 有index返回true并传回指针到indexinfo，没有就false；

  // =
  int ifSameVal(TableInfo* table_info, string cloumn_name, string val, int64_t rowid);

  // <=
  int ifLessEque(TableInfo* table_info, string column_name, string val, int64_t rowid);

  // <
  int ifLess(TableInfo* table_info, string column_name, string val, int64_t rowid);

  // >=
  int ifBiggerEque(TableInfo* table_info, string column_name, string val, int64_t rowid);

  // >
  int ifBigger(TableInfo* table_info, string column_name, string val, int64_t rowid);
  
  int ifUnique(TableInfo* table_info, string column_name);


  // int ifInIndex(TableInfo)
  // bool GetIndex(vector<string> column)
private:
  [[maybe_unused]] std::unordered_map<std::string, DBStorageEngine *> dbs_;  /** all opened databases */
  [[maybe_unused]] std::string current_db_;  /** current database */
  string dbs_name_file = "DBEXEENGINE.txt";
  // std::vector<std::string> db_names;
  // BufferPoolManager* buffer_pool_manager_;
  // std::string root_db_name = "root";
  // std::string root_table_name = "dbs";  
  // int ifInDatabases(std::string da_name); // new added @hz
};

#endif //MINISQL_EXECUTE_ENGINE_H
