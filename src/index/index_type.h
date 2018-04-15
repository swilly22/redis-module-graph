#ifndef __INDEX_TYPE_H__
#define __INDEX_TYPE_H__

#include "index.h"
#include "../redismodule.h"

extern RedisModuleType *IndexRedisModuleType;

#define INDEX_TYPE_ENCODING_VERSION 1

int IndexType_Register(RedisModuleCtx *ctx);
void* IndexType_RdbLoad(RedisModuleIO *rdb, int encver);
void IndexType_RdbSave(RedisModuleIO *rdb, void *value);
void IndexType_AofRewrite(RedisModuleIO *aof, RedisModuleString *key, void *value);
void IndexType_Free(void *value);

SIValue loadSIValue(RedisModuleIO *rdb);
void loadSkiplist(RedisModuleIO *rdb, skiplist *sl);
void saveSIValue(RedisModuleIO *rdb, SIValue *val);

void serializeIndex(RedisModuleIO *rdb, Index *index);
void serializeSkiplist(RedisModuleIO *rdb, skiplist *sl);
void serializeSkiplistNode(RedisModuleIO *rdb, skiplistNode *sl_node);

#endif

