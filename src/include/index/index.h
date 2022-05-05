#ifndef MINISQL_INDEX_H
#define MINISQL_INDEX_H

#include <memory>

#include "common/dberr.h"
#include "record/row.h"
#include "transaction/transaction.h"

class Index {
public:
  explicit Index(index_id_t index_id, IndexSchema *key_schema)
          : index_id_(index_id), key_schema_(key_schema) {}

  virtual ~Index() {}

  virtual dberr_t InsertEntry(const Row &key, RowId row_id, Transaction *txn) = 0;

  virtual dberr_t RemoveEntry(const Row &key, RowId row_id, Transaction *txn) = 0;

  virtual dberr_t ScanKey(const Row &key, std::vector<RowId> &result, Transaction *txn) = 0;

  virtual dberr_t Destroy() = 0;

protected:
  index_id_t index_id_;
  IndexSchema *key_schema_;
};

#endif //MINISQL_INDEX_H
