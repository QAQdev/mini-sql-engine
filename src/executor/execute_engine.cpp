#include "executor/execute_engine.h"
#include "glog/logging.h"

// @hz 22.5.23  22:58 看了半天有点想明白了
// 按好括号的回车，把不用写的函数都标出来了
// 感觉了一下如果能比较熟悉相应的函数操作应该能很快写好

// @hz 22.5.24  13:14
// 忍不了了
using namespace std;

ExecuteEngine::ExecuteEngine() 
{
  // buffer_pool_manager_ = new BufferPoolManager()
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
  if(dbs_[db_name] == nullptr)
    return DB_FAILED;
  DBStorageEngine* new_database = new DBStorageEngine(db_name, true);
  if(new_database == nullptr)
    return DB_FAILED;

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

  return DB_SUCCESS;
  // return DB_FAILED;
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
    return DB_FAILED; 
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
    return DB_FAILED;
  std::vector<TableInfo*> vec_table_info;
  current_db_engine->catalog_mgr_->GetTables(vec_table_info);
  for(int i=0; i<int(vec_table_info.size()); i++)
  {
    std::cout << vec_table_info[i]->GetTableName() << std::endl;
  }
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
  TableInfo* tmp_table_info;
  pSyntaxNode kNCD_List = ast->child_->next_;
  pSyntaxNode kNCD_node = kNCD_List->child_;
  vector<Column*> tmp_column_vec;
  vector<string> column_names;
  unordered_map<string, bool> if_unique;
  unordered_map<string, bool> if_primary_key;
  unordered_map<string, string> type_of_column;
  unordered_map<string, int> char_size;
  while(kNCD_node != nullptr && kNCD_node->type_ == kNodeColumnDefinition)
  {
    string kNCD_ifunique(kNCD_node->val_);
    string kNCD_typename(kNCD_node->child_->next_->val_);
    string kNCD_columname(kNCD_node->child_->val_);
    column_names.push_back(kNCD_columname);
    type_of_column[kNCD_columname] = kNCD_typename;
    if_unique[kNCD_columname] = false;
    if_primary_key[kNCD_columname] = false;
    if(kNCD_ifunique == "unique")
      if_unique[kNCD_columname] = true;
    else 
      if_unique[kNCD_columname] = false;
    if(kNCD_typename == "char")
    {
      pSyntaxNode kNCD_char_sizenode = kNCD_node->child_->next_->child_;
      string str_char_size (kNCD_char_sizenode->val_);
      char_size[kNCD_columname] = atoi(str_char_size.data());
      if(char_size[kNCD_columname] <= 0)
      {
        cout << "char size < 0 !" << endl;
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
    else if(type_of_column[column_name_stp] == "char")
    {
      if(if_unique[column_name_stp] || if_primary_key[column_name_stp])
      {
        new_column = new Column(column_name_stp, TypeId::kTypeChar, column_index_counter, 
                                                        false, true);
      }
      else 
      {
        new_column = new Column(column_name_stp, TypeId::kTypeChar, column_index_counter, 
                                                        false, false);
      } 
    }
    else 
    {
      cout << "unknow typename" << column_name_stp << endl;
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
    if(if_unique[column_name_stp] || if_primary_key[column_name_stp])
    {
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
      cout << index_name << endl;
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
  string table_name(ast->child_[1].val_);
  string index_name(ast->child_[0].val_);
  TableInfo* target_table;
  dberr_t if_gettable_success = current_CMgr->GetTable(table_name, target_table);
  if(if_gettable_success != DB_SUCCESS)
    return if_gettable_success;
  vector<string> vec_index_colum_lists;
  pSyntaxNode pSnode_colum_list = ast->child_[2].child_;
  while(pSnode_colum_list)
  {
    vec_index_colum_lists.push_back(string(pSnode_colum_list->val_));
    pSnode_colum_list = pSnode_colum_list->next_;
  }
  Schema* target_schema = target_table->GetSchema();
  for(string tmp_colum_name: vec_index_colum_lists)
  {
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
  string index_name(ast->child_[0].val_);
  string table_name;
  IndexInfo* tmp_indexinfo;
  dberr_t if_getindex_success = current_CMgr->GetIndex(table_name, index_name, tmp_indexinfo);
  if(if_getindex_success != DB_SUCCESS)
    return if_getindex_success;
  dberr_t if_create_success = current_CMgr->DropIndex(table_name, index_name);
  if(if_create_success != DB_SUCCESS)
    return if_create_success;
    
  return DB_SUCCESS;
  // return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteSelect(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteSelect" << std::endl;
#endif
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteInsert(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteInsert" << std::endl;
#endif
// 语法树还没处理
  string table_name;
  vector<Field> newfields;
  Row newrow(newfields);
  TableInfo* target_table_info;
  dberr_t if_gettable_success =
  dbs_[current_db_]->catalog_mgr_->GetTable(table_name, target_table_info);
  if(if_gettable_success != DB_SUCCESS)
    return if_gettable_success;
  TableHeap* target_table_heap = target_table_info->GetTableHeap();
  bool if_insert_success = target_table_heap->InsertTuple(newrow, nullptr);
  if(if_insert_success == false)
    return DB_FAILED;
  return DB_SUCCESS;
}

dberr_t ExecuteEngine::ExecuteDelete(pSyntaxNode ast, ExecuteContext *context) 
{
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDelete" << std::endl;
#endif
// 语法树还没处理
  string table_name;
  vector<RowId> target_rows;
  TableInfo* target_table_info;
  dberr_t if_gettable_success =
  dbs_[current_db_]->catalog_mgr_->GetTable(table_name, target_table_info);
  if(if_gettable_success != DB_SUCCESS)
    return if_gettable_success;
  TableHeap* target_table_heap = target_table_info->GetTableHeap();
  for(RowId tmp_row: target_rows)
  {
    if(!target_table_heap->MarkDelete(tmp_row, nullptr))
      cout << "DELETE FAILED!!!" << endl;
  }
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
      cout << "UPDATE FAILED!!!" << endl;
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


std::vector<RowId> ExecuteEngine::SelectPerform(pSyntaxNode ast, 
                  ExecuteContext* context, vector<RowId> org_rows)
{
  vector<RowId> selected_rows;
  return selected_rows;
}

std::vector<RowId> ExecuteEngine::FitchAllRows(std::string tablename)
{
  vector<RowId> vec_allrows;
  DBStorageEngine* current_db_engine = dbs_[current_db_];
  TableInfo* tableinfo;
  current_db_engine->catalog_mgr_->GetTable(tablename, tableinfo);
  TableIterator itor = tableinfo->GetTableHeap()->Begin(nullptr);
  TableIterator enditor = tableinfo->GetTableHeap()->End();
  do 
  {
    vec_allrows.push_back(itor->GetRowId());
    itor++;
  }while(itor != enditor);
  return vec_allrows;
}