#ifndef _CLAUSE_INDEX_H
#define _CLAUSE_INDEX_H

typedef enum {
  DROP_INDEX,
  CREATE_INDEX
} AST_IndexOpType;

typedef struct {
  const char *label;
  const char *property;
} IndexTarget;

typedef struct {
  IndexTarget target;
  AST_IndexOpType operation;
} AST_IndexNode;

AST_IndexNode* New_AST_IndexNode(const char *label, const char *property, AST_IndexOpType optype);

#endif