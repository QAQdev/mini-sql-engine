#include "executor/execute_engine.h"
#include "glog/logging.h"
#include <string>
#include <cstdlib>
#include <iostream>

#define ENABLE_EXECUTE_DEBUG
// @hz 22.5.23  22:58 看了半天有点想明白了
// 按好括号的回车，把不用写的函数都标出来了
// 感觉了一下如果能比较熟悉相应的函数操作应该能很快写好

// @hz 22.5.24  13:14
// 忍不了了
using namespace std;

ExecuteEngine::ExecuteEngine() 
{
// buffer_pool_manager_ = new BufferPoolManager();
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteEngine" << std::endl;
// fstream tmp_name_file_stream(dbs_name_file, ios::out);
// tmp_name_file_stream.close();
#endif
  // string name_db = "db";
  // // DBStorageEngine* new_engine = new DBStorageEngine(name_db, true);
  // // new_engine->~DBStorageEngine();
  // DBStorageEngine* new_e2 = new DBStorageEngine(name_db, false);
  // new_e2->~DBStorageEngine();

  fstream name_file_stream(dbs_name_file, ios::in);
  if(name_file_stream.fail())
  {
    fstream tmp_name_file_stream(dbs_name_file, ios::out);
    tmp_name_file_stream.close();
    name_file_stream.open(dbs_name_file, ios::in);
    if(name_file_stream.fail())
    {
      std::cout << "Fail to init." << endl;
    }
  }

  string db_name = "";
  while(getline(name_file_stream, db_name))
  {
    // char tmp_c;
    // while(tmp_c = name_file_stream.get(), tmp_c == ' '||tmp_c == '\n');
    // do 
    // {
    //   db_name += tmp_c;
    // }while(tmp_c = name_file_stream.get(), tmp_c != ' '&&tmp_c != '\n');
    // if(db_name.length() == 0)
    //   return ;
    if(db_name.length() == 0)
      continue;
    // std::cout << db_name.length() << endl;
    DBStorageEngine* new_engine = new DBStorageEngine(db_name, false);
    dbs_[db_name] = new_engine;
  }
  name_file_stream.close();
}

dberr_t ExecuteEngine::Execute(pSyntaxNode ast, ExecuteContext *context) 
{
  if (ast == nullptr) 
  {
    return DB_FAILED;
  }
  switch (ast->type_) 
  {
    case kNodeCreateDB:
      return ExecuteCreateDatabase(ast, context);
    case kNodeDropDB:
      return ExecuteDropDatabase(ast, context);
    case kNodeShowDB:
      return ExecuteShowDatabases(ast, context);
    case kNodeUseDB:
      return ExecuteUseDatabase(ast, context);
    case kNodeShowTables:
      return ExecuteShowTables(ast, context);
    case kNodeCreateTable:
      return ExecuteCreateTable(ast, context);
    case kNodeDropTable:
      return ExecuteDropTable(ast, context);
    case kNodeShowIndexes:
      return ExecuteShowIndexes(ast, context);
    case kNodeCreateIndex:
      return ExecuteCreateIndex(ast, context);
    case kNodeDropIndex:
      return ExecuteDropIndex(ast, context);
    case kNodeSelect:
      return ExecuteSelect(ast, context);
    case kNodeInsert:
      return ExecuteInsert(ast, context);
    case kNodeDelete:
      return ExecuteDelete(ast, context);
    case kNodeUpdate:
      return ExecuteUpdate(ast, context);
    case kNodeTrxBegin:
      return ExecuteTrxBegin(ast, context);
    case kNodeTrxCommit:
      return ExecuteTrxCommit(ast, context);
    case kNodeTrxRollback:
      return ExecuteTrxRollback(ast, context);
    case kNodeExecFile:
      return ExecuteExecfile(ast, context);
    case kNodeQuit:
      return ExecuteQuit(ast, context);
    default:
      break;
  }
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteCreateDatabase(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateDatabase" << std::endl;
#endif
  if(ast == nullptr || ast->child_ == nullptr)
    return DB_FAILED;
  std::string db_name(ast->child_->val_);
  if(dbs_[db_name] != nullptr)
  {
    std::cout << "database " << db_name << " exists." << endl;
    return DB_FAILED;
  }
  DBStorageEngine* new_database = new DBStorageEngine(db_name.data(), true);
  if(new_database == nullptr)
  {
    std::cout << "create database " << db_name << " fails." << endl;
    return DB_FAILED;
  }
  dbs_[db_name] = new_database;
  this->SaveDBs();
  return DB_SUCCESS;
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteDropDatabase(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropDatabase" << std::endl;
#endif
  if(ast == nullptr || ast->child_ == nullptr)
    return DB_FAILED;
  std::string del_db_name(ast->child_->val_);
  if(dbs_[del_db_name] == nullptr)
    return DB_FAILED;
  if(current_db_ == del_db_name)
    current_db_ = "";
  DBStorageEngine* target_db = dbs_[del_db_name];
  target_db->~DBStorageEngine();
  dbs_.erase(del_db_name);
  this->SaveDBs();
  return DB_SUCCESS;
  // return DB_FAILED; execfile "a.txt";
}

dberr_t ExecuteEngine::ExecuteShowDatabases(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowDatabases" << std::endl;
#endif
  
  for(std::pair<std::string, DBStorageEngine*> kv : dbs_)
  {
    std::cout << kv.first << std::endl;
  }
  std::cout << "Database(s) of number: " << dbs_.size() << endl;
  return DB_SUCCESS;
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteUseDatabase(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteUseDatabase" << std::endl;
#endif
  if(ast == nullptr || ast->child_ == nullptr)
    return DB_FAILED;
  std::string db_name(ast->child_->val_);
  if(dbs_[db_name] == nullptr)
  {
    std::cout << "no database " << db_name << endl;
    return DB_FAILED; 
  }
  current_db_ = db_name;
  return DB_SUCCESS;
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteShowTables(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowTables" << std::endl;
#endif
  DBStorageEngine* current_db_engine = dbs_[current_db_];
  if(current_db_engine == nullptr)
  {
    std::cout << "no database selected." << endl;
    return DB_FAILED;
  }
    
  std::vector<TableInfo*> vec_table_info;
  current_db_engine->catalog_mgr_->GetTables(vec_table_info);
  for(int i=0; i<int(vec_table_info.size()); i++)
  {
    std::cout << vec_table_info[i]->GetTableName() << std::endl;
  }
  std::cout << "table(s) of number: " << vec_table_info.size() << endl;
  return DB_SUCCESS;
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteCreateTable(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateTable" << std::endl;
#endif
  DBStorageEngine* current_db_engine = dbs_[current_db_];
  if(current_db_engine == nullptr)
    return DB_FAILED;
  string new_table_name(ast->child_->val_);
  TableInfo* new_table_info;
  dberr_t find_table =
      current_db_engine->catalog_mgr_->GetTable(new_table_name, new_table_info);
  if(find_table == DB_SUCCESS)
  {
    std::cout << "table exists" << endl;
    return DB_FAILED;
  }

  TableInfo* tmp_table_info;
  pSyntaxNode kNCD_List = ast->child_->next_;
  pSyntaxNode kNCD_node = kNCD_List->child_;
  vector<Column*> tmp_column_vec;
  vector<string> column_names;
  unordered_map<string, bool> if_unique;
  unordered_map<string, bool> if_primary_key;
  unordered_map<string, string> type_of_column;
  unordered_map<string, int> char_size;
  vector<string> uni_keys;
  vector<string> pri_keys;
  while(kNCD_node != nullptr && kNCD_node->type_ == kNodeColumnDefinition)
  {
    string kNCD_ifunique;
    if(kNCD_node->val_ == nullptr)
      kNCD_ifunique = "";
    else 
      kNCD_ifunique = kNCD_node->val_;
    string kNCD_typename(kNCD_node->child_->next_->val_);
    string kNCD_columname(kNCD_node->child_->val_);
    column_names.push_back(kNCD_columname);
    type_of_column[kNCD_columname] = kNCD_typename;
    if_unique[kNCD_columname] = false;
    if_primary_key[kNCD_columname] = false;
    if(kNCD_ifunique == "unique")
    {
      if_unique[kNCD_columname] = true;
      uni_keys.push_back(kNCD_columname);
    }
    else 
      if_unique[kNCD_columname] = false;
    if(kNCD_typename == "char")
    {
      pSyntaxNode kNCD_char_sizenode = kNCD_node->child_->next_->child_;
      string str_char_size (kNCD_char_sizenode->val_);
      char_size[kNCD_columname] = atoi(str_char_size.data());
      if(char_size[kNCD_columname] <= 0)
      {
        std::cout << "char size < 0 !" << endl;
        return DB_FAILED;
      }
    }
    kNCD_node = kNCD_node->next_;
  }
  if(kNCD_node != nullptr)
  {
    pSyntaxNode primary_keys_node = kNCD_node->child_;
    while(primary_keys_node)
    {
      string primary_key_name(primary_keys_node->val_);
      if_primary_key[primary_key_name] = true;
      pri_keys.push_back(primary_key_name);
      uni_keys.push_back(primary_key_name);
      primary_keys_node = primary_keys_node->next_;
    }
  }

  int column_index_counter = 0;
  for(string column_name_stp: column_names)
  {
    Column* new_column;
    if(type_of_column[column_name_stp] == "int")
    {
      if(if_unique[column_name_stp] || if_primary_key[column_name_stp])
      {
        new_column = new Column(column_name_stp, TypeId::kTypeInt, column_index_counter, 
                                false, true);
      }
      else 
      {
        new_column = new Column(column_name_stp, TypeId::kTypeInt, column_index_counter, 
                                false, false);
      }
    }
    else if(type_of_column[column_name_stp] == "float")
    {
      if(if_unique[column_name_stp] || if_primary_key[column_name_stp])
      {
        new_column = new Column(column_name_stp, TypeId::kTypeFloat, column_index_counter, 
                                false, true);
      }
      else 
      {
        new_column = new Column(column_name_stp, TypeId::kTypeFloat, column_index_counter, 
                                false, false);
      } 
    }
    else if(type_of_column[column_name_stp] == "char") {
      if (if_unique[column_name_stp] || if_primary_key[column_name_stp]) {
        new_column = new Column(column_name_stp, TypeId::kTypeChar, char_size[column_name_stp], column_index_counter,
                                false, true);
      } else {
        new_column = new Column(column_name_stp, TypeId::kTypeChar, char_size[column_name_stp], column_index_counter,
                                false, false);
      }
    }
    else 
    {
      std::cout << "unknow typename" << column_name_stp << endl;
      return DB_FAILED;
    }
    column_index_counter++;
    tmp_column_vec.push_back(new_column);
  }
  Schema* new_schema = new Schema(tmp_column_vec);
  dberr_t if_create_success;
  if_create_success = current_db_engine->catalog_mgr_->CreateTable(new_table_name, 
                                                                   new_schema, nullptr, tmp_table_info);
  if(if_create_success != DB_SUCCESS)
    return if_create_success;
  CatalogManager* current_CMgr = dbs_[current_db_]->catalog_mgr_;
  for(string column_name_stp: column_names)
  {
    if(if_primary_key[column_name_stp])
    {// 这里应该要在主键向量和unique键向量中间pushback一下；
      tmp_table_info->table_meta_->primary_key_name = pri_keys;
      tmp_table_info->table_meta_->unique_key_name = uni_keys;
      string stp_index_name = column_name_stp + "_index";
      vector<string> index_columns_stp = {column_name_stp};
      IndexInfo* stp_index_info;
      dberr_t if_create_index_success 
          = current_CMgr->CreateIndex(new_table_name, stp_index_name, index_columns_stp,
                                      nullptr, stp_index_info);
      if(if_create_index_success != DB_SUCCESS)
        return if_create_index_success;
    }
  }
  return DB_SUCCESS;
  // return DB_FAILED;
}

// filled
dberr_t ExecuteEngine::ExecuteDropTable(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropTable" << std::endl;
#endif
  if(ast == nullptr || ast->child_ == nullptr)
    return DB_FAILED;
  string drop_table_name(ast->child_->val_);

  return dbs_[current_db_]->catalog_mgr_->DropTable(drop_table_name);
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteShowIndexes(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowIndexes" << std::endl;
#endif
  vector<TableInfo*> vec_tableinfo;
  // vector<vector<IndexInfo*>> vvec_indexinfo;
  dberr_t if_gettable_success = dbs_[current_db_]->catalog_mgr_->GetTables(vec_tableinfo);
  if(if_gettable_success != DB_SUCCESS)
    return if_gettable_success;
  CatalogManager* current_CMgr = dbs_[current_db_]->catalog_mgr_;
  for(TableInfo* tmp_tableinfo: vec_tableinfo)
  {
    vector<IndexInfo*> vec_tmp_indexinfo;
    string table_name = tmp_tableinfo->GetTableName();
    dberr_t if_getindex_success = current_CMgr->GetTableIndexes(table_name, vec_tmp_indexinfo);
    if(if_getindex_success != DB_SUCCESS)
      return if_getindex_success;
    for(IndexInfo* tmp_indexinfo: vec_tmp_indexinfo)
    {
      string index_name = tmp_indexinfo->GetIndexName();
      std::cout << index_name << endl;
    }
  }

  return DB_SUCCESS;
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteCreateIndex(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateIndex" << std::endl;
#endif
  if(ast == nullptr || current_db_ == "")
    return DB_FAILED;
  CatalogManager* current_CMgr = dbs_[current_db_]->catalog_mgr_;
  string table_name(ast->child_->next_->val_);
  string index_name(ast->child_->val_);
  TableInfo* target_table;
  dberr_t if_gettable_success = current_CMgr->GetTable(table_name, target_table);
  if(if_gettable_success != DB_SUCCESS)
    return if_gettable_success;
  vector<string> vec_index_colum_lists;
  pSyntaxNode pSnode_colum_list = ast->child_->next_->next_->child_;
  while(pSnode_colum_list)
  {
    vec_index_colum_lists.push_back(string(pSnode_colum_list->val_));
    pSnode_colum_list = pSnode_colum_list->next_;
  }
  Schema* target_schema = target_table->GetSchema();
  for(string tmp_colum_name: vec_index_colum_lists)
  {
    int if_could = 0;
    vector<string> uni_colum_list 
    = target_table->table_meta_->unique_key_name;
    for(string name: uni_colum_list)
    {
      if(name == tmp_colum_name)
      {
        if_could = 1;
        break;
      }
    }
    if(!if_could)
    {
      std::cout << "can not build index on column(s) not unique" << endl;
      return DB_FAILED;
    }
    uint32_t tmp_index;
    dberr_t if_getcolum_success = target_schema->GetColumnIndex(tmp_colum_name, tmp_index);
    if(if_getcolum_success != DB_SUCCESS)
      return if_getcolum_success;
  }
  IndexInfo* new_indexinfo;
  dberr_t if_createindex_success = current_CMgr->CreateIndex(table_name, 
        index_name, vec_index_colum_lists, nullptr, new_indexinfo);
  if(if_createindex_success != DB_SUCCESS)
    return if_createindex_success;
  return DB_SUCCESS;
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteDropIndex(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropIndex" << std::endl;
#endif
  if(ast == nullptr || current_db_ == "")
    return DB_FAILED;
  CatalogManager* current_CMgr = dbs_[current_db_]->catalog_mgr_;
  string index_name(ast->child_->val_);
  vector<TableInfo*> table_names;
  current_CMgr->GetTables(table_names);
  string table_name;
  for(TableInfo* tmp_info: table_names)
  {
    vector<IndexInfo*> index_infos;
    current_CMgr->GetTableIndexes(tmp_info->GetTableName(), index_infos);
    for(IndexInfo* stp_idx_info: index_infos)
    {
      if(stp_idx_info->GetIndexName() == index_name)
      {
        table_name = tmp_info->GetTableName();
        goto out;
      }
    }
  }
  out:;
  IndexInfo* tmp_indexinfo;
  dberr_t if_getindex_success = current_CMgr->GetIndex(table_name, index_name, tmp_indexinfo);
  if(if_getindex_success != DB_SUCCESS)
  {
    std::cout << "no index: " << index_name << endl;
    return if_getindex_success;
  }
  dberr_t if_create_success = current_CMgr->DropIndex(table_name, index_name);
  if(if_create_success != DB_SUCCESS)
  {
    std::cout << "fail to drop index: " << index_name << endl;
    return if_create_success;
  }
    
  return DB_SUCCESS;
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteSelect(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteSelect" << std::endl;
#endif
  string select_table_name(ast->child_->next_->val_);
  // ExeSelectAll(select_table_name);
  // string select_target(ast->child_->val_); 
  SyntaxNodeType select_target = ast->child_->type_;
  vector<string> column_names;
  if(dbs_[current_db_] == nullptr)
  {
    std::cout << "no database selected." << endl;
    return DB_FAILED;
  }
  CatalogManager* current_CMgr = dbs_[current_db_]->catalog_mgr_;
  TableInfo* select_table_info;
  dberr_t if_select_table_success
      = current_CMgr->GetTable(select_table_name, select_table_info);
  if(if_select_table_success != DB_SUCCESS)
    return if_select_table_success;
  Schema* select_table_schema = select_table_info->GetSchema();
  // select_table_info->GetTableHeap();
  
  if(select_target == SyntaxNodeType::kNodeAllColumns)
  {
    // ExeSelectAll(select_table_name);
    // pSyntaxNode kNIdfiniers = ast->child_->child_;
    // while(kNIdfiniers)
    // {
    //   string tmp_column(kNIdfiniers->val_);
    //   uint32_t tmp_column_index;
    //   dberr_t if_get_column_success
    //       = select_table_schema->GetColumnIndex(tmp_column, tmp_column_index);
    //   if(if_get_column_success != DB_SUCCESS)
    //     return if_get_column_success;
    //   column_names.push_back(tmp_column);
    //   kNIdfiniers = kNIdfiniers->next_;
    // }
    vector<Column*> tmp_columns = select_table_schema->GetColumns();
    for(Column* stp_cloumn: tmp_columns)
    {
      string column_name = stp_cloumn->GetName();
      column_names.push_back(column_name);
    }
  }
  else 
  {
    pSyntaxNode kNIdfiniers = ast->child_->child_;
    while(kNIdfiniers)
    {
      string tmp_column(kNIdfiniers->val_);
      uint32_t tmp_column_index;
      dberr_t if_get_column_success
          = select_table_schema->GetColumnIndex(tmp_column, tmp_column_index);
      if(if_get_column_success != DB_SUCCESS)
        return if_get_column_success;
      column_names.push_back(tmp_column);
      kNIdfiniers = kNIdfiniers->next_;
    }
  }
  
  pSyntaxNode where_cmd = ast->child_->next_->next_;
  if(where_cmd != nullptr)
    where_cmd = where_cmd->child_;
  vector<int64_t> all_rows = FitchAllRows(select_table_name);
  vector<int64_t> rows_selected;
  dberr_t if_whereperform_success 
      = WherePerform(where_cmd, select_table_name, all_rows, rows_selected);
  if(if_whereperform_success != DB_SUCCESS)
    return DB_FAILED;
  PrintRows(select_table_info, rows_selected, column_names);
  
  return DB_SUCCESS;
}

dberr_t ExecuteEngine::ExecuteInsert(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteInsert" << std::endl;
#endif
  string table_name = ast->child_->val_;
  // 语法树还没处理
  // return DB_SUCCESS;
  vector<Field> newfields;
  Row newrow(newfields);
  TableInfo* target_table_info;
  dberr_t if_gettable_success =
      dbs_[current_db_]->catalog_mgr_->GetTable(table_name, target_table_info);
  if(if_gettable_success != DB_SUCCESS)
    return if_gettable_success;
  dberr_t if_getfield_success = 
      GetField(ast->child_->next_->child_, target_table_info);
  if(if_getfield_success != DB_SUCCESS)
  {
    std::cout << "fail to insert" << endl;
    return DB_FAILED;
  }
  
  return DB_SUCCESS;
}

dberr_t ExecuteEngine::ExecuteDelete(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDelete" << std::endl;
#endif
  // 语法树还没处理
  string table_name = ast->child_->val_;
  vector<int64_t> target_rows;
  pSyntaxNode conditon_cmd = ast->child_->next_;
  if(conditon_cmd != nullptr)
    conditon_cmd = conditon_cmd->child_;
  dberr_t if_perform_success =
    WherePerform(conditon_cmd, table_name, FitchAllRows(table_name) ,target_rows);
  if(if_perform_success != DB_SUCCESS)
    return if_perform_success;
  TableInfo* target_table_info;
  dberr_t if_gettable_success =
      dbs_[current_db_]->catalog_mgr_->GetTable(table_name, target_table_info);
  if(if_gettable_success != DB_SUCCESS)
    return if_gettable_success;
  TableHeap* target_table_heap = target_table_info->GetTableHeap();
  vector<Column*> vec_cols = target_table_info->GetSchema()->GetColumns();
  uint32_t index_counter = 0;
  for(Column* column_stp: vec_cols)
  {
    IndexInfo* tmp_index_info;
    if(ifhaveIndex(table_name, column_stp->GetName(), tmp_index_info))
    {
      for(int64_t tmp_row: target_rows)
      {
        RowId tmp_rowid(tmp_row);
        Row dec_row(tmp_rowid);
        target_table_heap->GetTuple(&dec_row, nullptr);
        vector<Field> tmpf={*dec_row.GetField(index_counter)};
        Row ddec_row(tmpf);
        tmp_index_info->GetIndex()->RemoveEntry(ddec_row, tmp_rowid, nullptr);
      }
    }
    index_counter ++;
  }
  for(int64_t tmp_row: target_rows)
  {
    RowId r_tmp_row(tmp_row);
    if(!target_table_heap->MarkDelete(r_tmp_row, nullptr))
    {
      std::cout << "DELETE FAILED!!!" << endl;
      return DB_FAILED;
    }
    target_table_heap->ApplyDelete(r_tmp_row, nullptr);
  }
  
  std::cout << "affects row(s) of number: " << target_rows.size() << endl;
  return DB_SUCCESS;
}

dberr_t ExecuteEngine::ExecuteUpdate(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteUpdate" << std::endl;
#endif
  // 语法树还没处理
  string table_name;
  vector<Field> new_field;
  Row new_row(new_field);
  vector<RowId> target_rows;
  TableInfo* target_table_info;
  dberr_t if_gettable_success =
      dbs_[current_db_]->catalog_mgr_->GetTable(table_name, target_table_info);
  if(if_gettable_success != DB_SUCCESS)
    return if_gettable_success;
  TableHeap* target_table_heap = target_table_info->GetTableHeap();
  for(RowId tmp_row: target_rows)
  {
    if(!target_table_heap->UpdateTuple(new_row, tmp_row, nullptr))
      std::cout << "UPDATE FAILED!!!" << endl;
  }
  return DB_SUCCESS;
}

// 可不实现
dberr_t ExecuteEngine::ExecuteTrxBegin(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxBegin" << std::endl;
#endif
  return DB_FAILED;
}


// 可不实现
dberr_t ExecuteEngine::ExecuteTrxCommit(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxCommit" << std::endl;
#endif
  return DB_FAILED;
}


// 可不实现
dberr_t ExecuteEngine::ExecuteTrxRollback(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxRollback" << std::endl;
#endif
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteExecfile(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteExecfile" << std::endl;
#endif
  // for(int i=0; ; i++)
  // {
  //   if(ast->child_->val_[i] == '\177')
  //   {
  //     ast->child_->val_[i] = 0;
  //     break;
  //   }
  // }
  string filename(ast->child_->val_);
  fstream exefstream(filename);
  if(!exefstream.is_open())
  {
    std::cout << "fail to open '" << filename << "'" << endl;
    return DB_FAILED;
  }
  int buffer_size = 1024;
  char* cmd = new char[buffer_size];
  
  
  while (1) 
  {
    // read from file
    char tmp_char;
    int tmp_counter = 0;
    do 
    { 
      if(exefstream.eof())
      {
        delete []cmd;
        return DB_SUCCESS;
      }
      // exefstream >> tmp_char;
      tmp_char = exefstream.get();
      cmd[tmp_counter++] = tmp_char;
      if(tmp_counter >= buffer_size)
      {
        std::cout << "buffer overflow" << endl;
        return DB_FAILED;
      } 
    }while(tmp_char != ';');
    cmd[tmp_counter] = 0;
    // create buffer for sql input
    YY_BUFFER_STATE bp = yy_scan_string(cmd);
    if (bp == nullptr) 
    {
      LOG(ERROR) << "Failed to create yy buffer state." << std::endl;
      exit(1);
    }
    yy_switch_to_buffer(bp);

    // init parser module
    MinisqlParserInit();

    // parse
    yyparse();

    // parse result handle
    if (MinisqlParserGetError()) 
    {
      // error
      printf("%s\n", MinisqlParserGetErrorMessage());
    } 
    else 
    {
      // #ifdef ENABLE_PARSER_DEBUG
      // printf("[INFO] Sql syntax parse ok!\n");
      // SyntaxTreePrinter printer(MinisqlGetParserRootNode());
      // MinisqlGetParserRootNode()->
      // printer.PrintTree(syntax_tree_file_mgr[syntax_tree_id++]);
      // #endif
    }

    this->Execute(MinisqlGetParserRootNode(), context);

    // clean memory after parse
    MinisqlParserFinish();
    yy_delete_buffer(bp);
    yylex_destroy();

    // quit condition
    if (context->flag_quit_) 
    {
      printf("bye!\n");
      break;
    }

  }


  return DB_FAILED;
}


dberr_t ExecuteEngine::ExecuteQuit(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteQuit" << std::endl;
#endif
  ASSERT(ast->type_ == kNodeQuit, "Unexpected node type.");
  context->flag_quit_ = true;
  return DB_SUCCESS;
}


dberr_t ExecuteEngine::SelectPerform(string table_name, string column_name, 
                                     string comparator_name, char* cval, 
                                     std::vector<int64_t> org_rows, 
                                     std::vector<int64_t>& reslt_rows)
{
  string val(cval);
  TableInfo* target_table_info;
  dberr_t if_get_table_success =
  dbs_[current_db_]->catalog_mgr_->GetTable(table_name, target_table_info);
  if(if_get_table_success != DB_SUCCESS)
    return DB_FAILED;
  if(comparator_name == "=")
  {
    IndexInfo* tmp_indexinfo;
    if(ifhaveIndex(table_name, column_name, tmp_indexinfo))
    {
      uint32_t col_index;
      dberr_t if_get_column_success =
      target_table_info->GetSchema()->GetColumnIndex(column_name, col_index);
      if(if_get_column_success != DB_SUCCESS)
        return if_get_column_success;
      TypeId col_type = target_table_info->GetSchema()->GetColumn(col_index)->GetType();
      vector<Field> tmp_fieles;
      if(col_type == TypeId::kTypeChar)
      {
        int col_lenth =
        target_table_info->GetSchema()->GetColumn(col_index)->GetLength();
        tmp_fieles.push_back(Field(TypeId::kTypeChar, const_cast<char*>(cval), 
                                col_lenth, true));
      }
      else if(col_type == TypeId::kTypeFloat)
      {
        float atoval = atof(val.data());
        tmp_fieles.push_back(Field(TypeId::kTypeFloat, atoval));
      }
      else if(col_type == TypeId::kTypeInt)
      {
        int atoval = atof(val.data());
        tmp_fieles.push_back(Field(TypeId::kTypeInt, atoval));
      }
      else 
        return DB_FAILED;
      Row search_key(tmp_fieles);
      vector<RowId> res;
      dberr_t if_get_key =
      tmp_indexinfo->GetIndex()->ScanKey(search_key, res, nullptr);
      if(if_get_key == DB_SUCCESS)
        reslt_rows.push_back(res[0].Get());
    }
    else 
    {
      // TableIterator itor = target_table_info->GetTableHeap()->Begin(nullptr);
      // TableIterator enditor = target_table_info->GetTableHeap()->End();
      // while(itor != enditor)
      // {
      //   // itor.row->GetRowId().Get();
      //   int64_t rowid_get = itor->GetRowId().Get();
      //   if(ifSameVal(target_table_info, column_name, val, rowid_get))
      //     reslt_rows.push_back(rowid_get);
      //   itor++;
      // }
      for(int64_t rowid_val: org_rows)
      {
        if(ifSameVal(target_table_info, column_name, val, rowid_val))
        {
          reslt_rows.push_back(rowid_val);
        }
      }
    }
  }
  else if(comparator_name == "<")
  {
    for(int64_t rowid_val: org_rows)
    {
      if(ifLess(target_table_info, column_name, val, rowid_val))
      {
        reslt_rows.push_back(rowid_val);
      }
    }
  }
  else if(comparator_name == "<=")
  {
    for(int64_t rowid_val: org_rows)
    {
      if(ifLessEque(target_table_info, column_name, val, rowid_val))
      {
        reslt_rows.push_back(rowid_val);
      }
    }
  }
  else if(comparator_name == ">")
  {
    for(int64_t rowid_val: org_rows)
    {
      if(ifBigger(target_table_info, column_name, val, rowid_val))
      {
        reslt_rows.push_back(rowid_val);
      }
    }
  }
  else if(comparator_name == ">=")
  {
    for(int64_t rowid_val: org_rows)
    {
      if(ifBiggerEque(target_table_info, column_name, val, rowid_val))
      {
        reslt_rows.push_back(rowid_val);
      }
    }
  }
  else if(comparator_name == "<>")
  {
    IndexInfo* tmp_indexinfo;
    if(ifhaveIndex(table_name, column_name, tmp_indexinfo))
    {
      uint32_t col_index;
      dberr_t if_get_column_success =
      target_table_info->GetSchema()->GetColumnIndex(column_name, col_index);
      if(if_get_column_success != DB_SUCCESS)
        return if_get_column_success;
      TypeId col_type = target_table_info->GetSchema()->GetColumn(col_index)->GetType();
      vector<Field> tmp_fieles;
      if(col_type == TypeId::kTypeChar)
      {
        int col_lenth =
        target_table_info->GetSchema()->GetColumn(col_index)->GetLength();
        tmp_fieles.push_back(Field(TypeId::kTypeChar, const_cast<char*>(cval), col_lenth, true));
      }
      else if(col_type == TypeId::kTypeFloat)
      {
        float atoval = atof(val.data());
        tmp_fieles.push_back(Field(TypeId::kTypeFloat, atoval));
      }
      else if(col_type == TypeId::kTypeInt)
      {
        int atoval = atof(val.data());
        tmp_fieles.push_back(Field(TypeId::kTypeInt, atoval));
      }
      else 
        return DB_FAILED;
      Row search_key(tmp_fieles);
      vector<RowId> res;
      dberr_t if_get_key =
      tmp_indexinfo->GetIndex()->ScanKey(search_key, res, nullptr);
      if(if_get_key == DB_SUCCESS)
      {
        int64_t del_val = res[0].Get();
        for(int i=0; i<int(org_rows.size()); i++)
        {
          if(del_val == org_rows[i])
          {
            org_rows.erase(i+org_rows.begin());
            i--;
          }
        }  
      }
      reslt_rows = org_rows;
    }
    else 
    {
      for(int64_t rowid_val: org_rows)
      {
        if(!ifSameVal(target_table_info, column_name, val, rowid_val))
        {
          reslt_rows.push_back(rowid_val);
        }
      }
    }
  }

  return DB_SUCCESS;
}

std::vector<int64_t> ExecuteEngine::FitchAllRows(std::string tablename)
{
  vector<int64_t> vec_allrows;
  DBStorageEngine* current_db_engine = dbs_[current_db_];
  TableInfo* tableinfo;
  current_db_engine->catalog_mgr_->GetTable(tablename, tableinfo);
  TableIterator itor = tableinfo->GetTableHeap()->Begin(nullptr);
  // TableIterator enditor = ;
  while(itor != tableinfo->GetTableHeap()->End())
  {
    vec_allrows.push_back(itor->GetRowId().Get());
    itor++;
  }
  // vec_allrows.push_back(enditor->GetRowId().Get());
  return vec_allrows;
}

// added @hz 
// 不需要了

vector<int64_t> ExecuteEngine::RowIdMerge(vector<int64_t> vec_fir, vector<int64_t> vec_sec)
{
  vector<int64_t> vec_new;
  return vec_new;
}

void ExecuteEngine::SaveDBs()
{
  fstream out_file_stream(dbs_name_file, ios::out);
  for(std::pair<std::string, DBStorageEngine*> db_info : dbs_)
  {
    out_file_stream << db_info.first << endl;
    // std::cout << db_info.first.length() << endl;
  }
  out_file_stream.close();
}

dberr_t ExecuteEngine::WherePerform(pSyntaxNode condition_node, string tablename, 
                vector<int64_t> org_rows, vector<int64_t>& new_rows)
{
  if(condition_node == nullptr)
  {
    new_rows = org_rows;
    return DB_SUCCESS; // all node performed;
  }
  if(condition_node->type_ != SyntaxNodeType::kNodeCompareOperator
      &&condition_node->type_ != SyntaxNodeType::kNodeConnector)
    return DB_FAILED;
  if(condition_node->type_ == SyntaxNodeType::kNodeCompareOperator)
  {
    string cmp_name(condition_node->val_);
    string column_name(condition_node->child_->val_);
    char* val = condition_node->child_->next_->val_;

    dberr_t if_select_success 
        = SelectPerform(tablename, column_name, cmp_name, val, org_rows, new_rows);
    return if_select_success;
  }
  else 
  {
    string connector_name(condition_node->val_);
    if(connector_name == "and")
    {
      vector<int64_t> tmp_new_rows;
      dberr_t if_whereperform_success 
          = WherePerform(condition_node->child_, tablename, org_rows, tmp_new_rows);
      if(if_whereperform_success != DB_SUCCESS)
        return if_whereperform_success;
      if_whereperform_success
          = WherePerform(condition_node->child_->next_, tablename, tmp_new_rows, new_rows);
      return if_whereperform_success;
    }
  }
  return DB_FAILED;
}

void ExecuteEngine::PrintRow(TableInfo* tmp_table_info, int64_t rowid, vector<string> print_columns)
{
  int COUNT_WIDTH_SET = 10;
  for(string column_name: print_columns)
  {
    uint32_t index_val;
    RowId r_rowid(rowid);
    Row row(r_rowid);
    tmp_table_info->GetTableHeap()->GetTuple(&row, nullptr);
    dberr_t if_getcolumn_success =
        tmp_table_info->GetSchema()->GetColumnIndex(column_name, index_val);
    if(if_getcolumn_success == DB_COLUMN_NAME_NOT_EXIST)
    {
      std::cout << endl << "column '" << column_name << "' does not exist." << endl;
      continue;
    }
    if(row.GetField(index_val)->GetType() == TypeId::kTypeInt)
    {
      std::cout.width(COUNT_WIDTH_SET);
      std::cout << row.GetField(index_val)->GetInt() << " ";
    }
    else if(row.GetField(index_val)->GetType() == TypeId::kTypeFloat)
    {
      std::cout.width(COUNT_WIDTH_SET);
      std::cout << row.GetField(index_val)->GetFloat() << " ";
    }
    else if(row.GetField(index_val)->GetType() == TypeId::kTypeChar)
    {
      std::cout << row.GetField(index_val)->GetData() << " ";
    }
    else 
      std::cout << "null" << endl;
  }
  std::cout << endl;
}

void ExecuteEngine::PrintRows(TableInfo* tmp_table_info, vector<int64_t> rowids, 
                              vector<string> print_columns)
{
  for(int64_t rowid: rowids)
  {
    PrintRow(tmp_table_info, rowid, print_columns);
  }
  std::cout << "Row(s) for number " << rowids.size() << endl;
}

dberr_t ExecuteEngine::GetField(pSyntaxNode ast, TableInfo* table_info)
{
  vector<Column*> vec_columns = table_info->GetSchema()->GetColumns();
  vector<Field> fields;
  vector<Index*> maintain_indexes;
  vector<Row> insert_rows;
  
  TableHeap* target_table_heap = table_info->GetTableHeap();
  for(Column* tmp_column: vec_columns)
  {
    IndexInfo* column_index;
    if(ast == nullptr
        ||(ast->type_ != SyntaxNodeType::kNodeString
            &&ast->type_ != SyntaxNodeType::kNodeNumber))
    {
      std::cout << "get  field wrong" << endl;
      return DB_FAILED;
    }
    if(ast->type_ == SyntaxNodeType::kNodeString
        && tmp_column->GetType() == TypeId::kTypeChar)
    {
      Field tmp_f(TypeId::kTypeChar, const_cast<char *>(ast->val_), tmp_column->GetLength(), true);
      fields.push_back(tmp_f);
      if(ifhaveIndex(table_info->GetTableName(), tmp_column->GetName(), column_index))
      {
        vector<Field> vec_tmpf;
        vec_tmpf.push_back(tmp_f);
        Row index_row_def(vec_tmpf);
        vector<RowId> res;
        dberr_t if_get_key =
        column_index->GetIndex()->ScanKey(index_row_def, res, nullptr);
        if(if_get_key == DB_SUCCESS)
        {
          std::cout << "duplicate value " << ast->val_ << " in " << tmp_column->GetName()  
                << tmp_column->GetName() << endl;
          return DB_FAILED;
        }
        
        // column_index->GetIndex()->InsertEntry()
        insert_rows.push_back(index_row_def);
        maintain_indexes.push_back(column_index->GetIndex());
      }
      else if(ifUnique(table_info, tmp_column->GetName()))
      {
        vector<int64_t> vac_val = FitchAllRows(table_info->GetTableName());
        for(int64_t val_stp: vac_val)
        {
          if(ifSameVal(table_info, tmp_column->GetName(), string(ast->val_), val_stp))
          {
            std::cout << "duplicate value " << ast->val_ << " in " << tmp_column->GetName()  
              << tmp_column->GetName() << endl;
            return DB_FAILED;
          }
        }
      }
    }
    else if(ast->type_ == SyntaxNodeType::kNodeNumber
             && tmp_column->GetType() == TypeId::kTypeFloat)
    {
      float val = atof(ast->val_);
      Field tmp_f(TypeId::kTypeFloat, val);
      fields.push_back(tmp_f);
      if(ifhaveIndex(table_info->GetTableName(), tmp_column->GetName(), column_index))
      {
        vector<Field> vec_tmpf;
        vec_tmpf.push_back(tmp_f);
        Row index_row_def(vec_tmpf);
        vector<RowId> res;
        dberr_t if_get_key =
        column_index->GetIndex()->ScanKey(index_row_def, res, nullptr);
        if(if_get_key == DB_SUCCESS)
        {
          std::cout << "duplicate value " << ast->val_ << " in " << tmp_column->GetName()  
                << tmp_column->GetName() << endl;
          return DB_FAILED;
        }
        insert_rows.push_back(index_row_def);
        maintain_indexes.push_back(column_index->GetIndex());
      }
      else if(ifUnique(table_info, tmp_column->GetName()))
      {
        vector<int64_t> vac_val = FitchAllRows(table_info->GetTableName());
        for(int64_t val_stp: vac_val)
        {
          if(ifSameVal(table_info, tmp_column->GetName(), string(ast->val_), val_stp))
          {
            std::cout << "duplicate value " << ast->val_ << " in " << tmp_column->GetName()  
               << tmp_column->GetName() << endl;
            return DB_FAILED;
          }
        }
      }
    }
    else if(ast->type_ == SyntaxNodeType::kNodeNumber
             && tmp_column->GetType() == TypeId::kTypeInt)
    {
      int val = atof(ast->val_);
      Field tmp_f(TypeId::kTypeInt, val);
      fields.push_back(tmp_f);
      if(ifhaveIndex(table_info->GetTableName(), tmp_column->GetName(), column_index))
      {
        vector<Field> vec_tmpf;
        vec_tmpf.push_back(tmp_f);
        Row index_row_def(vec_tmpf);
        vector<RowId> res;
        dberr_t if_get_key =
        column_index->GetIndex()->ScanKey(index_row_def, res, nullptr);
        if(if_get_key == DB_SUCCESS)
        {
          std::cout << "duplicate value " << ast->val_ << " in " << tmp_column->GetName()  
                << tmp_column->GetName() << endl;
          return DB_FAILED;
        }
        insert_rows.push_back(index_row_def);
        maintain_indexes.push_back(column_index->GetIndex());
      }
      else if(ifUnique(table_info, tmp_column->GetName()))
      {
        vector<int64_t> vac_val = FitchAllRows(table_info->GetTableName());
        for(int64_t val_stp: vac_val)
        {
          if(ifSameVal(table_info, tmp_column->GetName(), string(ast->val_), val_stp))
          {
            std::cout << "duplicate value " << ast->val_ << " in " << tmp_column->GetName()  
               << tmp_column->GetName() << endl;
            return DB_FAILED;
          }
        }
      }
    }
    else 
    {
      std::cout << "get  field wrong" << endl;
      return DB_FAILED;
    }
    ast = ast->next_;
  }
  Row newrow(fields);
  bool if_insert_success = target_table_heap->InsertTuple(newrow, nullptr);
  if(if_insert_success == false)
    return DB_FAILED;
  for(int i=0; i<int(maintain_indexes.size()); i++)
  {
    maintain_indexes[i]->InsertEntry(insert_rows[i], newrow.GetRowId(), nullptr);
  }
  
  return DB_SUCCESS;
}

void ExecuteEngine::ExeSelectAll(string tablename)
{
  CatalogManager* cunnernt_CMger = dbs_[current_db_]->catalog_mgr_;
  TableInfo* tmp_table_info;
  dberr_t if_gettable_success =
      cunnernt_CMger->GetTable(tablename, tmp_table_info);
  if(if_gettable_success != DB_SUCCESS)
  {
    std::cout << "select all fail" << endl;
    return ;
  }
  vector<Column*> columns =
      tmp_table_info->GetSchema()->GetColumns();
  vector<string> str_columns;
  for(Column* tmp_column: columns)
  {
    str_columns.push_back(tmp_column->GetName());
  }
  PrintRows(tmp_table_info, FitchAllRows(tablename), str_columns);
}

bool ExecuteEngine::ifhaveIndex(string table_name, string column_name, 
                          IndexInfo*& indexinfo)
{
  vector<IndexInfo*> table_indexes;
  dberr_t if_get_indexes_success =
  dbs_[current_db_]->catalog_mgr_->GetTableIndexes(table_name, table_indexes);
  if(if_get_indexes_success != DB_SUCCESS)
    return false;
  for(IndexInfo* stp_index_info: table_indexes)
  {
    vector<Column*> columns =
    stp_index_info->GetIndexKeySchema()->GetColumns();
    if(columns.size() != 1 || columns[0]->GetName() != column_name)
      continue;
    else 
    {
      indexinfo = stp_index_info;
      return true;
    }
  }
  return false;
}


int ExecuteEngine::ifSameVal(TableInfo* table_info, string column_name, 
                  string val, int64_t rowid)
{
  uint32_t index_val;
  RowId r_rowid(rowid);
  Row row(r_rowid);
  table_info->GetTableHeap()->GetTuple(&row, nullptr);
  dberr_t if_getcolumn_success =
      table_info->GetSchema()->GetColumnIndex(column_name, index_val);
  if(if_getcolumn_success == DB_COLUMN_NAME_NOT_EXIST)
  {
    return -1;
  }
  if(row.GetField(index_val)->GetType() == TypeId::kTypeInt)
  {
    int val_int = atof(val.data());
    if(val_int == row.GetField(index_val)->GetInt())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeFloat)
  {
    float val_float = atof(val.data());
    if(val_float == row.GetField(index_val)->GetFloat())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeChar)
  {
    string str_val = row.GetField(index_val)->GetData();
    if(str_val == val)
      return 1;
    else 
      return 0;
  }
  else 
    return -1;
}


int ExecuteEngine::ifBiggerEque(TableInfo* table_info, string column_name, 
                  string val, int64_t rowid)
{
  uint32_t index_val;
  RowId r_rowid(rowid);
  Row row(r_rowid);
  table_info->GetTableHeap()->GetTuple(&row, nullptr);
  dberr_t if_getcolumn_success =
      table_info->GetSchema()->GetColumnIndex(column_name, index_val);
  if(if_getcolumn_success == DB_COLUMN_NAME_NOT_EXIST)
  {
    return -1;
  }
  if(row.GetField(index_val)->GetType() == TypeId::kTypeInt)
  {
    int val_int = atof(val.data());
    if(val_int <= row.GetField(index_val)->GetInt())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeFloat)
  {
    float val_float = atof(val.data());
    if(val_float <= row.GetField(index_val)->GetFloat())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeChar)
  {
    string str_val = row.GetField(index_val)->GetData();
    if(str_val <= val)
      return 1;
    else 
      return 0;
  }
  else 
    return -1;
}


int ExecuteEngine::ifBigger(TableInfo* table_info, string column_name, 
                  string val, int64_t rowid)
{
  uint32_t index_val;
  RowId r_rowid(rowid);
  Row row(r_rowid);
  table_info->GetTableHeap()->GetTuple(&row, nullptr);
  dberr_t if_getcolumn_success =
      table_info->GetSchema()->GetColumnIndex(column_name, index_val);
  if(if_getcolumn_success == DB_COLUMN_NAME_NOT_EXIST)
  {
    return -1;
  }
  if(row.GetField(index_val)->GetType() == TypeId::kTypeInt)
  {
    int val_int = atof(val.data());
    if(val_int < row.GetField(index_val)->GetInt())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeFloat)
  {
    float val_float = atof(val.data());
    if(val_float < row.GetField(index_val)->GetFloat())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeChar)
  {
    string str_val = row.GetField(index_val)->GetData();
    if(str_val < val)
      return 1;
    else 
      return 0;
  }
  else 
    return -1;
}


int ExecuteEngine::ifLessEque(TableInfo* table_info, string column_name, 
                  string val, int64_t rowid)
{
  uint32_t index_val;
  RowId r_rowid(rowid);
  Row row(r_rowid);
  table_info->GetTableHeap()->GetTuple(&row, nullptr);
  dberr_t if_getcolumn_success =
      table_info->GetSchema()->GetColumnIndex(column_name, index_val);
  if(if_getcolumn_success == DB_COLUMN_NAME_NOT_EXIST)
  {
    return -1;
  }
  if(row.GetField(index_val)->GetType() == TypeId::kTypeInt)
  {
    int val_int = atof(val.data());
    if(val_int >= row.GetField(index_val)->GetInt())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeFloat)
  {
    float val_float = atof(val.data());
    if(val_float >= row.GetField(index_val)->GetFloat())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeChar)
  {
    string str_val = row.GetField(index_val)->GetData();
    if(str_val >= val)
      return 1;
    else 
      return 0;
  }
  else 
    return -1;
}


int ExecuteEngine::ifLess(TableInfo* table_info, string column_name, 
                  string val, int64_t rowid)
{
  uint32_t index_val;
  RowId r_rowid(rowid);
  Row row(r_rowid);
  table_info->GetTableHeap()->GetTuple(&row, nullptr);
  dberr_t if_getcolumn_success =
      table_info->GetSchema()->GetColumnIndex(column_name, index_val);
  if(if_getcolumn_success == DB_COLUMN_NAME_NOT_EXIST)
  {
    return -1;
  }
  if(row.GetField(index_val)->GetType() == TypeId::kTypeInt)
  {
    int val_int = atof(val.data());
    if(val_int > row.GetField(index_val)->GetInt())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeFloat)
  {
    float val_float = atof(val.data());
    if(val_float > row.GetField(index_val)->GetFloat())
      return 1;
    else 
      return 0;
  }
  else if(row.GetField(index_val)->GetType() == TypeId::kTypeChar)
  {
    string str_val = row.GetField(index_val)->GetData();
    if(str_val > val)
      return 1;
    else 
      return 0;
  }
  else 
    return -1;
}

int ExecuteEngine::ifUnique(TableInfo* table_info, string column_name)
{
  for(string name: table_info->table_meta_->unique_key_name)
  {
    if(column_name == name)
      return 1;
  }
  return 0;
}