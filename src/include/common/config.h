#ifndef MINISQL_CONFIG_H
#define MINISQL_CONFIG_H

#include <cstdint>
#include <cstring>

static constexpr int INVALID_PAGE_ID = -1;           // invalid page id
static constexpr int INVALID_FRAME_ID = -1;          // invalid transaction id
static constexpr int INVALID_TXN_ID = -1;            // invalid transaction id
static constexpr int INVALID_LSN = -1;               // invalid log sequence number

static constexpr int META_PAGE_ID = 0;               // physical page id of the disk file meta info
static constexpr int CATALOG_META_PAGE_ID = 0;       // logical page id of the catalog meta data
static constexpr int INDEX_ROOTS_PAGE_ID = 1;        // logical page id of the index roots

static constexpr int PAGE_SIZE = 4096;               // size of a data page in byte
static constexpr int DEFAULT_BUFFER_POOL_SIZE = 1024;// default size of buffer pool

static constexpr uint32_t FIELD_NULL_LEN = UINT32_MAX;
static constexpr uint32_t VARCHAR_MAX_LEN = PAGE_SIZE / 2;    // max length of varchar

// static std::string DB_META_FILE = "minisql.meta.db";

using page_id_t = int32_t;
using frame_id_t = int32_t;
using txn_id_t = int32_t;
using lsn_t = int32_t;
using column_id_t = uint32_t;
using index_id_t = uint32_t;
using table_id_t = uint32_t;

#endif  // MINISQL_CONFIG_H
