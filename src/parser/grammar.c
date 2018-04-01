/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 9 "grammar.y"

	#include <stdlib.h>
	#include <stdio.h>
	#include <assert.h>
	#include "token.h"	
	#include "grammar.h"
	#include "ast.h"
	#include "parse.h"
	#include "../value.h"

	void yyerror(char *s);
#line 40 "grammar.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYNTOKEN           Number of terminal symbols
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
**    YY_MIN_REDUCE      Minimum value for reduce actions
**    YY_MAX_REDUCE      Maximum value for reduce actions
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 80
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  AST_LinkEntity* yy1;
  AST_Query* yy8;
  AST_MatchNode* yy9;
  AST_ColumnNode* yy14;
  AST_ReturnNode* yy16;
  AST_SetNode* yy26;
  AST_ArithmeticExpressionNode* yy28;
  AST_LimitNode* yy29;
  AST_SetElement* yy34;
  Vector* yy38;
  AST_ReturnElementNode* yy44;
  int yy60;
  AST_CreateNode* yy82;
  SIValue yy102;
  AST_FilterNode* yy110;
  AST_WhereNode* yy111;
  AST_OrderNode* yy114;
  AST_Variable* yy134;
  AST_NodeEntity* yy135;
  AST_DeleteNode * yy147;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  parseCtx *ctx ;
#define ParseARG_PDECL , parseCtx *ctx 
#define ParseARG_FETCH  parseCtx *ctx  = yypParser->ctx 
#define ParseARG_STORE yypParser->ctx  = ctx 
#define YYNSTATE             110
#define YYNRULE              86
#define YYNTOKEN             46
#define YY_MAX_SHIFT         109
#define YY_MIN_SHIFTREDUCE   169
#define YY_MAX_SHIFTREDUCE   254
#define YY_ERROR_ACTION      255
#define YY_ACCEPT_ACTION     256
#define YY_NO_ACTION         257
#define YY_MIN_REDUCE        258
#define YY_MAX_REDUCE        343
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X.
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (238)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   300,  109,  256,   54,   64,  264,   11,    9,    8,    7,
 /*    10 */   274,   82,  242,  243,  246,  244,  245,   66,   10,  317,
 /*    20 */    56,  328,   64,   11,    9,    8,    7,  106,  316,   86,
 /*    30 */    31,  104,  307,    2,   25,   66,   10,  326,   64,  221,
 /*    40 */   248,   45,  250,  251,  253,  254,  247,   64,  101,   13,
 /*    50 */    21,   77,   70,   30,  276,  317,   56,   14,  248,   96,
 /*    60 */   250,  251,  253,  254,  316,   53,   90,  102,  307,  261,
 /*    70 */    50,  317,   57,   16,  248,   92,  250,  251,  253,  254,
 /*    80 */   316,  328,   15,  248,   38,  250,  251,  253,  254,   11,
 /*    90 */     9,    8,    7,   75,  317,   59,   61,  327,  317,   56,
 /*   100 */    68,  317,   58,  316,   74,   30,  276,  316,   65,   25,
 /*   110 */   316,  306,  317,  314,  317,  313,   94,   32,  317,   67,
 /*   120 */   103,  316,   89,  316,  317,   55,   20,  316,   45,   42,
 /*   130 */   317,   63,   60,  316,   83,  273,   82,   26,   28,  316,
 /*   140 */    88,  235,  236,   33,  276,   79,   27,  100,   26,   28,
 /*   150 */    39,   47,   45,    8,    7,  212,    3,  249,  226,   45,
 /*   160 */   252,    6,  302,   95,   62,  266,   71,   73,   34,  105,
 /*   170 */   265,   36,  192,   81,   84,   97,   85,   24,   87,  296,
 /*   180 */    45,  108,   91,   93,  263,  277,   98,   99,  107,    1,
 /*   190 */    48,   29,   49,   19,   12,  259,   51,   52,   35,   69,
 /*   200 */   178,  179,   72,   37,   28,  210,   76,   22,   78,  193,
 /*   210 */     5,  199,   40,   80,  202,   18,   41,   17,  241,  197,
 /*   220 */   195,  203,  198,  201,  200,   43,   44,   23,  196,  194,
 /*   230 */    46,  258,  220,  232,  205,  257,    4,  105,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    71,   47,   48,   49,    4,   51,    3,    4,    5,    6,
 /*    10 */    61,   62,    7,    8,    9,   10,   11,   17,   18,   62,
 /*    20 */    63,   62,    4,    3,    4,    5,    6,   17,   71,   16,
 /*    30 */    17,   74,   75,   33,   18,   17,   18,   78,    4,   19,
 /*    40 */    40,   28,   42,   43,   44,   45,   41,    4,   59,   12,
 /*    50 */    73,   17,   15,   64,   65,   62,   63,   20,   40,   71,
 /*    60 */    42,   43,   44,   45,   71,   51,   68,   74,   75,   55,
 /*    70 */    56,   62,   63,   12,   40,   68,   42,   43,   44,   45,
 /*    80 */    71,   62,   21,   40,   23,   42,   43,   44,   45,    3,
 /*    90 */     4,    5,    6,   72,   62,   63,   77,   78,   62,   63,
 /*   100 */    59,   62,   63,   71,   13,   64,   65,   71,   76,   18,
 /*   110 */    71,   75,   62,   63,   62,   63,   16,   17,   62,   63,
 /*   120 */    34,   71,   68,   71,   62,   63,   22,   71,   28,    4,
 /*   130 */    62,   63,   72,   71,   60,   61,   62,    1,    2,   71,
 /*   140 */    16,   37,   38,   64,   65,   17,   18,   16,    1,    2,
 /*   150 */    25,   66,   28,    5,    6,   19,   18,   40,   19,   28,
 /*   160 */    43,   22,   72,   68,   72,   58,   16,   18,   57,   31,
 /*   170 */    58,   57,   17,   67,   69,   17,   68,   26,   68,   70,
 /*   180 */    28,   39,   69,   68,   54,   65,   70,   68,   35,   32,
 /*   190 */    53,   30,   52,   22,   50,   54,   53,   52,   14,   13,
 /*   200 */    17,   19,   17,   14,    2,   17,   31,   17,   31,   17,
 /*   210 */     7,    4,   17,   22,   27,   22,   17,   36,   40,   19,
 /*   220 */    19,   27,   24,   27,   27,   17,   22,   16,   19,   19,
 /*   230 */    17,    0,   17,   17,   29,   79,   22,   31,   79,   79,
 /*   240 */    79,   79,   79,   79,   79,   79,   79,   79,   79,   79,
 /*   250 */    79,   79,   79,   79,   79,   79,   79,   79,   79,   79,
 /*   260 */    79,   79,   79,   79,   79,   79,   79,   79,   79,   79,
 /*   270 */    79,   79,   79,   79,   79,   79,   79,   79,   79,   79,
 /*   280 */    79,   79,   79,   79,
};
#define YY_SHIFT_COUNT    (109)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (231)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */    37,    0,   18,   18,   18,   18,   18,   18,   18,   18,
 /*    10 */    18,   18,   61,   91,   16,   10,   16,   10,   10,   16,
 /*    20 */    10,   34,    5,   43,   13,  100,  128,  128,  128,  128,
 /*    30 */   125,  124,  131,  125,  149,  150,  149,  150,  155,  151,
 /*    40 */   152,  152,  151,  152,  158,  158,  152,   16,  142,  153,
 /*    50 */   157,  142,  153,  157,  161,   20,   86,    3,    3,    3,
 /*    60 */   136,  104,  147,  148,  117,  139,  138,  148,  171,  184,
 /*    70 */   186,  183,  182,  185,  189,  202,  188,  175,  190,  177,
 /*    80 */   192,  191,  203,  193,  207,  187,  195,  194,  199,  196,
 /*    90 */   197,  198,  200,  201,  208,  209,  204,  211,  205,  210,
 /*   100 */   213,  171,  214,  215,  214,  216,  206,  181,  178,  231,
};
#define YY_REDUCE_COUNT (54)
#define YY_REDUCE_MIN   (-71)
#define YY_REDUCE_MAX   (145)
static const short yy_reduce_ofst[] = {
 /*     0 */   -46,  -43,   -7,   32,   36,    9,   39,   50,   52,   56,
 /*    10 */    62,   68,   14,  -11,   41,   74,  -11,   19,  -51,   79,
 /*    20 */   -41,  -71,  -23,  -12,   -2,    7,   21,   60,   90,   92,
 /*    30 */    85,   54,   95,   85,  107,  111,  112,  114,  106,  105,
 /*    40 */   108,  110,  113,  115,  109,  116,  119,  120,  130,  137,
 /*    50 */   140,  141,  143,  145,  144,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   270,  255,  255,  255,  255,  255,  255,  255,  255,  255,
 /*    10 */   255,  255,  270,  255,  255,  255,  255,  255,  255,  255,
 /*    20 */   255,  255,  255,  255,  293,  293,  255,  255,  255,  255,
 /*    30 */   278,  293,  293,  279,  255,  255,  255,  255,  255,  255,
 /*    40 */   293,  293,  255,  293,  255,  255,  293,  255,  329,  322,
 /*    50 */   262,  329,  322,  260,  297,  255,  308,  275,  318,  319,
 /*    60 */   255,  323,  298,  311,  255,  255,  320,  312,  269,  255,
 /*    70 */   255,  255,  255,  255,  255,  303,  255,  255,  255,  255,
 /*    80 */   255,  280,  255,  272,  255,  255,  255,  255,  255,  255,
 /*    90 */   255,  255,  255,  255,  255,  255,  295,  255,  255,  255,
 /*   100 */   255,  271,  305,  255,  304,  255,  320,  255,  255,  255,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#if defined(YYCOVERAGE) || !defined(NDEBUG)
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  /*    0 */ "$",
  /*    1 */ "OR",
  /*    2 */ "AND",
  /*    3 */ "ADD",
  /*    4 */ "DASH",
  /*    5 */ "MUL",
  /*    6 */ "DIV",
  /*    7 */ "EQ",
  /*    8 */ "GT",
  /*    9 */ "GE",
  /*   10 */ "LT",
  /*   11 */ "LE",
  /*   12 */ "CREATE",
  /*   13 */ "INDEX",
  /*   14 */ "ON",
  /*   15 */ "DROP",
  /*   16 */ "COLON",
  /*   17 */ "UQSTRING",
  /*   18 */ "LEFT_PARENTHESIS",
  /*   19 */ "RIGHT_PARENTHESIS",
  /*   20 */ "MATCH",
  /*   21 */ "SET",
  /*   22 */ "COMMA",
  /*   23 */ "DELETE",
  /*   24 */ "RIGHT_ARROW",
  /*   25 */ "LEFT_ARROW",
  /*   26 */ "LEFT_BRACKET",
  /*   27 */ "RIGHT_BRACKET",
  /*   28 */ "LEFT_CURLY_BRACKET",
  /*   29 */ "RIGHT_CURLY_BRACKET",
  /*   30 */ "WHERE",
  /*   31 */ "DOT",
  /*   32 */ "RETURN",
  /*   33 */ "DISTINCT",
  /*   34 */ "AS",
  /*   35 */ "ORDER",
  /*   36 */ "BY",
  /*   37 */ "ASC",
  /*   38 */ "DESC",
  /*   39 */ "LIMIT",
  /*   40 */ "INTEGER",
  /*   41 */ "NE",
  /*   42 */ "STRING",
  /*   43 */ "FLOAT",
  /*   44 */ "TRUE",
  /*   45 */ "FALSE",
  /*   46 */ "error",
  /*   47 */ "expr",
  /*   48 */ "query",
  /*   49 */ "matchClause",
  /*   50 */ "whereClause",
  /*   51 */ "createClause",
  /*   52 */ "returnClause",
  /*   53 */ "orderClause",
  /*   54 */ "limitClause",
  /*   55 */ "deleteClause",
  /*   56 */ "setClause",
  /*   57 */ "indexLabel",
  /*   58 */ "indexProp",
  /*   59 */ "chains",
  /*   60 */ "setList",
  /*   61 */ "setElement",
  /*   62 */ "variable",
  /*   63 */ "arithmetic_expression",
  /*   64 */ "chain",
  /*   65 */ "node",
  /*   66 */ "link",
  /*   67 */ "deleteExpression",
  /*   68 */ "properties",
  /*   69 */ "edge",
  /*   70 */ "mapLiteral",
  /*   71 */ "value",
  /*   72 */ "cond",
  /*   73 */ "relation",
  /*   74 */ "returnElements",
  /*   75 */ "returnElement",
  /*   76 */ "arithmetic_expression_list",
  /*   77 */ "columnNameList",
  /*   78 */ "columnName",
};
#endif /* defined(YYCOVERAGE) || !defined(NDEBUG) */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "query ::= expr",
 /*   1 */ "expr ::= matchClause whereClause createClause returnClause orderClause limitClause",
 /*   2 */ "expr ::= matchClause whereClause createClause",
 /*   3 */ "expr ::= matchClause whereClause deleteClause",
 /*   4 */ "expr ::= matchClause whereClause setClause",
 /*   5 */ "expr ::= matchClause whereClause setClause returnClause orderClause limitClause",
 /*   6 */ "expr ::= createClause",
 /*   7 */ "expr ::= CREATE INDEX ON indexLabel indexProp",
 /*   8 */ "expr ::= DROP INDEX ON indexLabel indexProp",
 /*   9 */ "indexLabel ::= COLON UQSTRING",
 /*  10 */ "indexProp ::= LEFT_PARENTHESIS UQSTRING RIGHT_PARENTHESIS",
 /*  11 */ "matchClause ::= MATCH chains",
 /*  12 */ "createClause ::=",
 /*  13 */ "createClause ::= CREATE chains",
 /*  14 */ "setClause ::= SET setList",
 /*  15 */ "setList ::= setElement",
 /*  16 */ "setList ::= setList COMMA setElement",
 /*  17 */ "setElement ::= variable EQ arithmetic_expression",
 /*  18 */ "chain ::= node",
 /*  19 */ "chain ::= chain link node",
 /*  20 */ "chains ::= chain",
 /*  21 */ "chains ::= chains COMMA chain",
 /*  22 */ "deleteClause ::= DELETE deleteExpression",
 /*  23 */ "deleteExpression ::= UQSTRING",
 /*  24 */ "deleteExpression ::= deleteExpression COMMA UQSTRING",
 /*  25 */ "node ::= LEFT_PARENTHESIS UQSTRING COLON UQSTRING properties RIGHT_PARENTHESIS",
 /*  26 */ "node ::= LEFT_PARENTHESIS COLON UQSTRING properties RIGHT_PARENTHESIS",
 /*  27 */ "node ::= LEFT_PARENTHESIS UQSTRING properties RIGHT_PARENTHESIS",
 /*  28 */ "node ::= LEFT_PARENTHESIS properties RIGHT_PARENTHESIS",
 /*  29 */ "link ::= DASH edge RIGHT_ARROW",
 /*  30 */ "link ::= LEFT_ARROW edge DASH",
 /*  31 */ "edge ::= LEFT_BRACKET properties RIGHT_BRACKET",
 /*  32 */ "edge ::= LEFT_BRACKET UQSTRING properties RIGHT_BRACKET",
 /*  33 */ "edge ::= LEFT_BRACKET COLON UQSTRING properties RIGHT_BRACKET",
 /*  34 */ "edge ::= LEFT_BRACKET UQSTRING COLON UQSTRING properties RIGHT_BRACKET",
 /*  35 */ "properties ::=",
 /*  36 */ "properties ::= LEFT_CURLY_BRACKET mapLiteral RIGHT_CURLY_BRACKET",
 /*  37 */ "mapLiteral ::= UQSTRING COLON value",
 /*  38 */ "mapLiteral ::= UQSTRING COLON value COMMA mapLiteral",
 /*  39 */ "whereClause ::=",
 /*  40 */ "whereClause ::= WHERE cond",
 /*  41 */ "cond ::= UQSTRING DOT UQSTRING relation UQSTRING DOT UQSTRING",
 /*  42 */ "cond ::= UQSTRING DOT UQSTRING relation value",
 /*  43 */ "cond ::= LEFT_PARENTHESIS cond RIGHT_PARENTHESIS",
 /*  44 */ "cond ::= cond AND cond",
 /*  45 */ "cond ::= cond OR cond",
 /*  46 */ "returnClause ::= RETURN returnElements",
 /*  47 */ "returnClause ::= RETURN DISTINCT returnElements",
 /*  48 */ "returnElements ::= returnElements COMMA returnElement",
 /*  49 */ "returnElements ::= returnElement",
 /*  50 */ "returnElement ::= arithmetic_expression",
 /*  51 */ "returnElement ::= arithmetic_expression AS UQSTRING",
 /*  52 */ "arithmetic_expression ::= LEFT_PARENTHESIS arithmetic_expression RIGHT_PARENTHESIS",
 /*  53 */ "arithmetic_expression ::= arithmetic_expression ADD arithmetic_expression",
 /*  54 */ "arithmetic_expression ::= arithmetic_expression DASH arithmetic_expression",
 /*  55 */ "arithmetic_expression ::= arithmetic_expression MUL arithmetic_expression",
 /*  56 */ "arithmetic_expression ::= arithmetic_expression DIV arithmetic_expression",
 /*  57 */ "arithmetic_expression ::= UQSTRING LEFT_PARENTHESIS arithmetic_expression_list RIGHT_PARENTHESIS",
 /*  58 */ "arithmetic_expression ::= value",
 /*  59 */ "arithmetic_expression ::= variable",
 /*  60 */ "arithmetic_expression_list ::= arithmetic_expression_list COMMA arithmetic_expression",
 /*  61 */ "arithmetic_expression_list ::= arithmetic_expression",
 /*  62 */ "variable ::= UQSTRING",
 /*  63 */ "variable ::= UQSTRING DOT UQSTRING",
 /*  64 */ "orderClause ::=",
 /*  65 */ "orderClause ::= ORDER BY columnNameList",
 /*  66 */ "orderClause ::= ORDER BY columnNameList ASC",
 /*  67 */ "orderClause ::= ORDER BY columnNameList DESC",
 /*  68 */ "columnNameList ::= columnNameList COMMA columnName",
 /*  69 */ "columnNameList ::= columnName",
 /*  70 */ "columnName ::= variable",
 /*  71 */ "limitClause ::=",
 /*  72 */ "limitClause ::= LIMIT INTEGER",
 /*  73 */ "relation ::= EQ",
 /*  74 */ "relation ::= GT",
 /*  75 */ "relation ::= LT",
 /*  76 */ "relation ::= LE",
 /*  77 */ "relation ::= GE",
 /*  78 */ "relation ::= NE",
 /*  79 */ "value ::= INTEGER",
 /*  80 */ "value ::= DASH INTEGER",
 /*  81 */ "value ::= STRING",
 /*  82 */ "value ::= FLOAT",
 /*  83 */ "value ::= DASH FLOAT",
 /*  84 */ "value ::= TRUE",
 /*  85 */ "value ::= FALSE",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void ParseInit(void *yypParser){
  yyParser *pParser = (yyParser*)yypParser;
#ifdef YYTRACKMAXSTACKDEPTH
  pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  pParser->yytos = NULL;
  pParser->yystack = NULL;
  pParser->yystksz = 0;
  if( yyGrowStack(pParser) ){
    pParser->yystack = &pParser->yystk0;
    pParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  pParser->yyerrcnt = -1;
#endif
  pParser->yytos = pParser->yystack;
  pParser->yystack[0].stateno = 0;
  pParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  pParser->yystackEnd = &pParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef Parse_ENGINEALWAYSONSTACK
/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) ParseInit(pParser);
  return pParser;
}
#endif /* Parse_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
    case 72: /* cond */
{
#line 286 "grammar.y"
 Free_AST_FilterNode((yypminor->yy110)); 
#line 710 "grammar.c"
}
      break;
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void ParseFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef Parse_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  ParseFinalize(p);
  (*freeProc)(p);
}
#endif /* Parse_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/* This array of booleans keeps track of the parser statement
** coverage.  The element yycoverage[X][Y] is set when the parser
** is in state X and has a lookahead token Y.  In a well-tested
** systems, every element of this matrix should end up being set.
*/
#if defined(YYCOVERAGE)
static unsigned char yycoverage[YYNSTATE][YYNTOKEN];
#endif

/*
** Write into out a description of every state/lookahead combination that
**
**   (1)  has not been used by the parser, and
**   (2)  is not a syntax error.
**
** Return the number of missed state/lookahead combinations.
*/
#if defined(YYCOVERAGE)
int ParseCoverage(FILE *out){
  int stateno, iLookAhead, i;
  int nMissed = 0;
  for(stateno=0; stateno<YYNSTATE; stateno++){
    i = yy_shift_ofst[stateno];
    for(iLookAhead=0; iLookAhead<YYNTOKEN; iLookAhead++){
      if( yy_lookahead[i+iLookAhead]!=iLookAhead ) continue;
      if( yycoverage[stateno][iLookAhead]==0 ) nMissed++;
      if( out ){
        fprintf(out,"State %d lookahead %s %s\n", stateno,
                yyTokenName[iLookAhead],
                yycoverage[stateno][iLookAhead] ? "ok" : "missed");
      }
    }
  }
  return nMissed;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>YY_MAX_SHIFT ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
#if defined(YYCOVERAGE)
  yycoverage[stateno][iLookAhead] = 1;
#endif
  do{
    i = yy_shift_ofst[stateno];
    assert( i>=0 );
    assert( i+YYNTOKEN<=(int)sizeof(yy_lookahead)/sizeof(yy_lookahead[0]) );
    assert( iLookAhead!=YYNOCODE );
    assert( iLookAhead < YYNTOKEN );
    i += iLookAhead;
    if( yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState, const char *zTag){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%s%s '%s', go to state %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%s%s '%s', pending reduce %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState - YY_MIN_REDUCE);
    }
  }
}
#else
# define yyTraceShift(X,Y,Z)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState, "Shift");
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  {   48,   -1 }, /* (0) query ::= expr */
  {   47,   -6 }, /* (1) expr ::= matchClause whereClause createClause returnClause orderClause limitClause */
  {   47,   -3 }, /* (2) expr ::= matchClause whereClause createClause */
  {   47,   -3 }, /* (3) expr ::= matchClause whereClause deleteClause */
  {   47,   -3 }, /* (4) expr ::= matchClause whereClause setClause */
  {   47,   -6 }, /* (5) expr ::= matchClause whereClause setClause returnClause orderClause limitClause */
  {   47,   -1 }, /* (6) expr ::= createClause */
  {   47,   -5 }, /* (7) expr ::= CREATE INDEX ON indexLabel indexProp */
  {   47,   -5 }, /* (8) expr ::= DROP INDEX ON indexLabel indexProp */
  {   57,   -2 }, /* (9) indexLabel ::= COLON UQSTRING */
  {   58,   -3 }, /* (10) indexProp ::= LEFT_PARENTHESIS UQSTRING RIGHT_PARENTHESIS */
  {   49,   -2 }, /* (11) matchClause ::= MATCH chains */
  {   51,    0 }, /* (12) createClause ::= */
  {   51,   -2 }, /* (13) createClause ::= CREATE chains */
  {   56,   -2 }, /* (14) setClause ::= SET setList */
  {   60,   -1 }, /* (15) setList ::= setElement */
  {   60,   -3 }, /* (16) setList ::= setList COMMA setElement */
  {   61,   -3 }, /* (17) setElement ::= variable EQ arithmetic_expression */
  {   64,   -1 }, /* (18) chain ::= node */
  {   64,   -3 }, /* (19) chain ::= chain link node */
  {   59,   -1 }, /* (20) chains ::= chain */
  {   59,   -3 }, /* (21) chains ::= chains COMMA chain */
  {   55,   -2 }, /* (22) deleteClause ::= DELETE deleteExpression */
  {   67,   -1 }, /* (23) deleteExpression ::= UQSTRING */
  {   67,   -3 }, /* (24) deleteExpression ::= deleteExpression COMMA UQSTRING */
  {   65,   -6 }, /* (25) node ::= LEFT_PARENTHESIS UQSTRING COLON UQSTRING properties RIGHT_PARENTHESIS */
  {   65,   -5 }, /* (26) node ::= LEFT_PARENTHESIS COLON UQSTRING properties RIGHT_PARENTHESIS */
  {   65,   -4 }, /* (27) node ::= LEFT_PARENTHESIS UQSTRING properties RIGHT_PARENTHESIS */
  {   65,   -3 }, /* (28) node ::= LEFT_PARENTHESIS properties RIGHT_PARENTHESIS */
  {   66,   -3 }, /* (29) link ::= DASH edge RIGHT_ARROW */
  {   66,   -3 }, /* (30) link ::= LEFT_ARROW edge DASH */
  {   69,   -3 }, /* (31) edge ::= LEFT_BRACKET properties RIGHT_BRACKET */
  {   69,   -4 }, /* (32) edge ::= LEFT_BRACKET UQSTRING properties RIGHT_BRACKET */
  {   69,   -5 }, /* (33) edge ::= LEFT_BRACKET COLON UQSTRING properties RIGHT_BRACKET */
  {   69,   -6 }, /* (34) edge ::= LEFT_BRACKET UQSTRING COLON UQSTRING properties RIGHT_BRACKET */
  {   68,    0 }, /* (35) properties ::= */
  {   68,   -3 }, /* (36) properties ::= LEFT_CURLY_BRACKET mapLiteral RIGHT_CURLY_BRACKET */
  {   70,   -3 }, /* (37) mapLiteral ::= UQSTRING COLON value */
  {   70,   -5 }, /* (38) mapLiteral ::= UQSTRING COLON value COMMA mapLiteral */
  {   50,    0 }, /* (39) whereClause ::= */
  {   50,   -2 }, /* (40) whereClause ::= WHERE cond */
  {   72,   -7 }, /* (41) cond ::= UQSTRING DOT UQSTRING relation UQSTRING DOT UQSTRING */
  {   72,   -5 }, /* (42) cond ::= UQSTRING DOT UQSTRING relation value */
  {   72,   -3 }, /* (43) cond ::= LEFT_PARENTHESIS cond RIGHT_PARENTHESIS */
  {   72,   -3 }, /* (44) cond ::= cond AND cond */
  {   72,   -3 }, /* (45) cond ::= cond OR cond */
  {   52,   -2 }, /* (46) returnClause ::= RETURN returnElements */
  {   52,   -3 }, /* (47) returnClause ::= RETURN DISTINCT returnElements */
  {   74,   -3 }, /* (48) returnElements ::= returnElements COMMA returnElement */
  {   74,   -1 }, /* (49) returnElements ::= returnElement */
  {   75,   -1 }, /* (50) returnElement ::= arithmetic_expression */
  {   75,   -3 }, /* (51) returnElement ::= arithmetic_expression AS UQSTRING */
  {   63,   -3 }, /* (52) arithmetic_expression ::= LEFT_PARENTHESIS arithmetic_expression RIGHT_PARENTHESIS */
  {   63,   -3 }, /* (53) arithmetic_expression ::= arithmetic_expression ADD arithmetic_expression */
  {   63,   -3 }, /* (54) arithmetic_expression ::= arithmetic_expression DASH arithmetic_expression */
  {   63,   -3 }, /* (55) arithmetic_expression ::= arithmetic_expression MUL arithmetic_expression */
  {   63,   -3 }, /* (56) arithmetic_expression ::= arithmetic_expression DIV arithmetic_expression */
  {   63,   -4 }, /* (57) arithmetic_expression ::= UQSTRING LEFT_PARENTHESIS arithmetic_expression_list RIGHT_PARENTHESIS */
  {   63,   -1 }, /* (58) arithmetic_expression ::= value */
  {   63,   -1 }, /* (59) arithmetic_expression ::= variable */
  {   76,   -3 }, /* (60) arithmetic_expression_list ::= arithmetic_expression_list COMMA arithmetic_expression */
  {   76,   -1 }, /* (61) arithmetic_expression_list ::= arithmetic_expression */
  {   62,   -1 }, /* (62) variable ::= UQSTRING */
  {   62,   -3 }, /* (63) variable ::= UQSTRING DOT UQSTRING */
  {   53,    0 }, /* (64) orderClause ::= */
  {   53,   -3 }, /* (65) orderClause ::= ORDER BY columnNameList */
  {   53,   -4 }, /* (66) orderClause ::= ORDER BY columnNameList ASC */
  {   53,   -4 }, /* (67) orderClause ::= ORDER BY columnNameList DESC */
  {   77,   -3 }, /* (68) columnNameList ::= columnNameList COMMA columnName */
  {   77,   -1 }, /* (69) columnNameList ::= columnName */
  {   78,   -1 }, /* (70) columnName ::= variable */
  {   54,    0 }, /* (71) limitClause ::= */
  {   54,   -2 }, /* (72) limitClause ::= LIMIT INTEGER */
  {   73,   -1 }, /* (73) relation ::= EQ */
  {   73,   -1 }, /* (74) relation ::= GT */
  {   73,   -1 }, /* (75) relation ::= LT */
  {   73,   -1 }, /* (76) relation ::= LE */
  {   73,   -1 }, /* (77) relation ::= GE */
  {   73,   -1 }, /* (78) relation ::= NE */
  {   71,   -1 }, /* (79) value ::= INTEGER */
  {   71,   -2 }, /* (80) value ::= DASH INTEGER */
  {   71,   -1 }, /* (81) value ::= STRING */
  {   71,   -1 }, /* (82) value ::= FLOAT */
  {   71,   -2 }, /* (83) value ::= DASH FLOAT */
  {   71,   -1 }, /* (84) value ::= TRUE */
  {   71,   -1 }, /* (85) value ::= FALSE */
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
**
** The yyLookahead and yyLookaheadToken parameters provide reduce actions
** access to the lookahead token (if any).  The yyLookahead will be YYNOCODE
** if the lookahead token has already been consumed.  As this procedure is
** only called from one place, optimizing compilers will in-line it, which
** means that the extra parameters have no performance impact.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno,       /* Number of the rule by which to reduce */
  int yyLookahead,             /* Lookahead token, or YYNOCODE if none */
  ParseTOKENTYPE yyLookaheadToken  /* Value of the lookahead token */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  (void)yyLookahead;
  (void)yyLookaheadToken;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    if( yysize ){
      fprintf(yyTraceFILE, "%sReduce %d [%s], go to state %d.\n",
        yyTracePrompt,
        yyruleno, yyRuleName[yyruleno], yymsp[yysize].stateno);
    }else{
      fprintf(yyTraceFILE, "%sReduce %d [%s].\n",
        yyTracePrompt, yyruleno, yyRuleName[yyruleno]);
    }
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* query ::= expr */
#line 35 "grammar.y"
{ ctx->root = yymsp[0].minor.yy8; }
#line 1173 "grammar.c"
        break;
      case 1: /* expr ::= matchClause whereClause createClause returnClause orderClause limitClause */
#line 37 "grammar.y"
{
  yylhsminor.yy8 = Allocate_AST_Query();
	yylhsminor.yy8->ast = New_AST_QueryExpressionNode(yymsp[-5].minor.yy9, yymsp[-4].minor.yy111, yymsp[-3].minor.yy82, NULL, NULL, yymsp[-2].minor.yy16, yymsp[-1].minor.yy114, yymsp[0].minor.yy29);
  yylhsminor.yy8->type = T_EXPRESSION;
}
#line 1182 "grammar.c"
  yymsp[-5].minor.yy8 = yylhsminor.yy8;
        break;
      case 2: /* expr ::= matchClause whereClause createClause */
#line 43 "grammar.y"
{
  yylhsminor.yy8 = Allocate_AST_Query();
	yylhsminor.yy8->ast = New_AST_QueryExpressionNode(yymsp[-2].minor.yy9, yymsp[-1].minor.yy111, yymsp[0].minor.yy82, NULL, NULL, NULL, NULL, NULL);
  yylhsminor.yy8->type = T_EXPRESSION;
}
#line 1192 "grammar.c"
  yymsp[-2].minor.yy8 = yylhsminor.yy8;
        break;
      case 3: /* expr ::= matchClause whereClause deleteClause */
#line 49 "grammar.y"
{
  yylhsminor.yy8 = Allocate_AST_Query();
	yylhsminor.yy8->ast = New_AST_QueryExpressionNode(yymsp[-2].minor.yy9, yymsp[-1].minor.yy111, NULL, NULL, yymsp[0].minor.yy147, NULL, NULL, NULL);
  yylhsminor.yy8->type = T_EXPRESSION;
}
#line 1202 "grammar.c"
  yymsp[-2].minor.yy8 = yylhsminor.yy8;
        break;
      case 4: /* expr ::= matchClause whereClause setClause */
#line 55 "grammar.y"
{
  yylhsminor.yy8 = Allocate_AST_Query();
	yylhsminor.yy8->ast = New_AST_QueryExpressionNode(yymsp[-2].minor.yy9, yymsp[-1].minor.yy111, NULL, yymsp[0].minor.yy26, NULL, NULL, NULL, NULL);
  yylhsminor.yy8->type = T_EXPRESSION;
}
#line 1212 "grammar.c"
  yymsp[-2].minor.yy8 = yylhsminor.yy8;
        break;
      case 5: /* expr ::= matchClause whereClause setClause returnClause orderClause limitClause */
#line 61 "grammar.y"
{
  yylhsminor.yy8 = Allocate_AST_Query();
	yylhsminor.yy8->ast = New_AST_QueryExpressionNode(yymsp[-5].minor.yy9, yymsp[-4].minor.yy111, NULL, yymsp[-3].minor.yy26, NULL, yymsp[-2].minor.yy16, yymsp[-1].minor.yy114, yymsp[0].minor.yy29);
  yylhsminor.yy8->type = T_EXPRESSION;
}
#line 1222 "grammar.c"
  yymsp[-5].minor.yy8 = yylhsminor.yy8;
        break;
      case 6: /* expr ::= createClause */
#line 67 "grammar.y"
{
  yylhsminor.yy8 = Allocate_AST_Query();
	yylhsminor.yy8->ast = New_AST_QueryExpressionNode(NULL, NULL, yymsp[0].minor.yy82, NULL, NULL, NULL, NULL, NULL);
  yylhsminor.yy8->type = T_EXPRESSION;
}
#line 1232 "grammar.c"
  yymsp[0].minor.yy8 = yylhsminor.yy8;
        break;
      case 7: /* expr ::= CREATE INDEX ON indexLabel indexProp */
#line 73 "grammar.y"
{
  yymsp[-4].minor.yy8 = Allocate_AST_Query();
  yymsp[-4].minor.yy8->indexOp = AST_IndexOp(yymsp[-1].minor.yy0.strval, yymsp[0].minor.yy0.strval);
  yymsp[-4].minor.yy8->indexOp->operation = T_CREATE;
  yymsp[-4].minor.yy8->type = T_INDEX;
}
#line 1243 "grammar.c"
        break;
      case 8: /* expr ::= DROP INDEX ON indexLabel indexProp */
#line 81 "grammar.y"
{
  yymsp[-4].minor.yy8 = Allocate_AST_Query();
  yymsp[-4].minor.yy8->indexOp = AST_IndexOp(yymsp[-1].minor.yy0.strval, yymsp[0].minor.yy0.strval);
  yymsp[-4].minor.yy8->indexOp->operation = T_DROP;
  yymsp[-4].minor.yy8->type = T_INDEX;
}
#line 1253 "grammar.c"
        break;
      case 9: /* indexLabel ::= COLON UQSTRING */
#line 88 "grammar.y"
{
  yymsp[-1].minor.yy0 = yymsp[0].minor.yy0;
}
#line 1260 "grammar.c"
        break;
      case 10: /* indexProp ::= LEFT_PARENTHESIS UQSTRING RIGHT_PARENTHESIS */
#line 92 "grammar.y"
{
  yymsp[-2].minor.yy0 = yymsp[-1].minor.yy0;
}
#line 1267 "grammar.c"
        break;
      case 11: /* matchClause ::= MATCH chains */
#line 98 "grammar.y"
{
	yymsp[-1].minor.yy9 = New_AST_MatchNode(yymsp[0].minor.yy38);
}
#line 1274 "grammar.c"
        break;
      case 12: /* createClause ::= */
#line 105 "grammar.y"
{
	yymsp[1].minor.yy82 = NULL;
}
#line 1281 "grammar.c"
        break;
      case 13: /* createClause ::= CREATE chains */
#line 109 "grammar.y"
{
	yymsp[-1].minor.yy82 = New_AST_CreateNode(yymsp[0].minor.yy38);
}
#line 1288 "grammar.c"
        break;
      case 14: /* setClause ::= SET setList */
#line 114 "grammar.y"
{
	yymsp[-1].minor.yy26 = New_AST_SetNode(yymsp[0].minor.yy38);
}
#line 1295 "grammar.c"
        break;
      case 15: /* setList ::= setElement */
#line 119 "grammar.y"
{
	yylhsminor.yy38 = NewVector(AST_SetElement*, 1);
	Vector_Push(yylhsminor.yy38, yymsp[0].minor.yy34);
}
#line 1303 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 16: /* setList ::= setList COMMA setElement */
#line 123 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy38, yymsp[0].minor.yy34);
	yylhsminor.yy38 = yymsp[-2].minor.yy38;
}
#line 1312 "grammar.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 17: /* setElement ::= variable EQ arithmetic_expression */
#line 129 "grammar.y"
{
	yylhsminor.yy34 = New_AST_SetElement(yymsp[-2].minor.yy134, yymsp[0].minor.yy28);
}
#line 1320 "grammar.c"
  yymsp[-2].minor.yy34 = yylhsminor.yy34;
        break;
      case 18: /* chain ::= node */
#line 135 "grammar.y"
{
	yylhsminor.yy38 = NewVector(AST_GraphEntity*, 1);
	Vector_Push(yylhsminor.yy38, yymsp[0].minor.yy135);
}
#line 1329 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 19: /* chain ::= chain link node */
#line 140 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy38, yymsp[-1].minor.yy1);
	Vector_Push(yymsp[-2].minor.yy38, yymsp[0].minor.yy135);
	yylhsminor.yy38 = yymsp[-2].minor.yy38;
}
#line 1339 "grammar.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 20: /* chains ::= chain */
#line 147 "grammar.y"
{
	yylhsminor.yy38 = yymsp[0].minor.yy38;
}
#line 1347 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 21: /* chains ::= chains COMMA chain */
#line 151 "grammar.y"
{
	for(int i = 0; i < Vector_Size(yymsp[0].minor.yy38); i++) {
		AST_GraphEntity *entity;
		Vector_Get(yymsp[0].minor.yy38, i, &entity);
		Vector_Push(yymsp[-2].minor.yy38, entity);
	}
	Vector_Free(yymsp[0].minor.yy38);
	yylhsminor.yy38 = yymsp[-2].minor.yy38;
}
#line 1361 "grammar.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 22: /* deleteClause ::= DELETE deleteExpression */
#line 164 "grammar.y"
{
	yymsp[-1].minor.yy147 = New_AST_DeleteNode(yymsp[0].minor.yy38);
}
#line 1369 "grammar.c"
        break;
      case 23: /* deleteExpression ::= UQSTRING */
#line 170 "grammar.y"
{
	yylhsminor.yy38 = NewVector(char*, 1);
	Vector_Push(yylhsminor.yy38, yymsp[0].minor.yy0.strval);
}
#line 1377 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 24: /* deleteExpression ::= deleteExpression COMMA UQSTRING */
#line 175 "grammar.y"
{
  Vector_Push(yymsp[-2].minor.yy38, yymsp[0].minor.yy0.strval);
	yylhsminor.yy38 = yymsp[-2].minor.yy38;
}
#line 1386 "grammar.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 25: /* node ::= LEFT_PARENTHESIS UQSTRING COLON UQSTRING properties RIGHT_PARENTHESIS */
#line 183 "grammar.y"
{
	yymsp[-5].minor.yy135 = New_AST_NodeEntity(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy38);
}
#line 1394 "grammar.c"
        break;
      case 26: /* node ::= LEFT_PARENTHESIS COLON UQSTRING properties RIGHT_PARENTHESIS */
#line 188 "grammar.y"
{
	yymsp[-4].minor.yy135 = New_AST_NodeEntity(NULL, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy38);
}
#line 1401 "grammar.c"
        break;
      case 27: /* node ::= LEFT_PARENTHESIS UQSTRING properties RIGHT_PARENTHESIS */
#line 193 "grammar.y"
{
	yymsp[-3].minor.yy135 = New_AST_NodeEntity(yymsp[-2].minor.yy0.strval, NULL, yymsp[-1].minor.yy38);
}
#line 1408 "grammar.c"
        break;
      case 28: /* node ::= LEFT_PARENTHESIS properties RIGHT_PARENTHESIS */
#line 198 "grammar.y"
{
	yymsp[-2].minor.yy135 = New_AST_NodeEntity(NULL, NULL, yymsp[-1].minor.yy38);
}
#line 1415 "grammar.c"
        break;
      case 29: /* link ::= DASH edge RIGHT_ARROW */
#line 205 "grammar.y"
{
	yymsp[-2].minor.yy1 = yymsp[-1].minor.yy1;
	yymsp[-2].minor.yy1->direction = N_LEFT_TO_RIGHT;
}
#line 1423 "grammar.c"
        break;
      case 30: /* link ::= LEFT_ARROW edge DASH */
#line 211 "grammar.y"
{
	yymsp[-2].minor.yy1 = yymsp[-1].minor.yy1;
	yymsp[-2].minor.yy1->direction = N_RIGHT_TO_LEFT;
}
#line 1431 "grammar.c"
        break;
      case 31: /* edge ::= LEFT_BRACKET properties RIGHT_BRACKET */
#line 218 "grammar.y"
{ 
	yymsp[-2].minor.yy1 = New_AST_LinkEntity(NULL, NULL, yymsp[-1].minor.yy38, N_DIR_UNKNOWN);
}
#line 1438 "grammar.c"
        break;
      case 32: /* edge ::= LEFT_BRACKET UQSTRING properties RIGHT_BRACKET */
#line 223 "grammar.y"
{ 
	yymsp[-3].minor.yy1 = New_AST_LinkEntity(yymsp[-2].minor.yy0.strval, NULL, yymsp[-1].minor.yy38, N_DIR_UNKNOWN);
}
#line 1445 "grammar.c"
        break;
      case 33: /* edge ::= LEFT_BRACKET COLON UQSTRING properties RIGHT_BRACKET */
#line 228 "grammar.y"
{ 
	yymsp[-4].minor.yy1 = New_AST_LinkEntity(NULL, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy38, N_DIR_UNKNOWN);
}
#line 1452 "grammar.c"
        break;
      case 34: /* edge ::= LEFT_BRACKET UQSTRING COLON UQSTRING properties RIGHT_BRACKET */
#line 233 "grammar.y"
{ 
	yymsp[-5].minor.yy1 = New_AST_LinkEntity(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy38, N_DIR_UNKNOWN);
}
#line 1459 "grammar.c"
        break;
      case 35: /* properties ::= */
#line 239 "grammar.y"
{
	yymsp[1].minor.yy38 = NULL;
}
#line 1466 "grammar.c"
        break;
      case 36: /* properties ::= LEFT_CURLY_BRACKET mapLiteral RIGHT_CURLY_BRACKET */
#line 243 "grammar.y"
{
	yymsp[-2].minor.yy38 = yymsp[-1].minor.yy38;
}
#line 1473 "grammar.c"
        break;
      case 37: /* mapLiteral ::= UQSTRING COLON value */
#line 249 "grammar.y"
{
	yylhsminor.yy38 = NewVector(SIValue*, 2);

	SIValue *key = malloc(sizeof(SIValue));
	*key = SI_StringVal(yymsp[-2].minor.yy0.strval);

	Vector_Push(yylhsminor.yy38, key);

	SIValue *val = malloc(sizeof(SIValue));
	*val = yymsp[0].minor.yy102;
	Vector_Push(yylhsminor.yy38, val);
}
#line 1489 "grammar.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 38: /* mapLiteral ::= UQSTRING COLON value COMMA mapLiteral */
#line 262 "grammar.y"
{
	SIValue *key = malloc(sizeof(SIValue));
	*key = SI_StringVal(yymsp[-4].minor.yy0.strval);

	Vector_Push(yymsp[0].minor.yy38, key);

	SIValue *val = malloc(sizeof(SIValue));
	*val = yymsp[-2].minor.yy102;
	Vector_Push(yymsp[0].minor.yy38, val);
	
	yylhsminor.yy38 = yymsp[0].minor.yy38;
}
#line 1506 "grammar.c"
  yymsp[-4].minor.yy38 = yylhsminor.yy38;
        break;
      case 39: /* whereClause ::= */
#line 277 "grammar.y"
{ 
	yymsp[1].minor.yy111 = NULL;
}
#line 1514 "grammar.c"
        break;
      case 40: /* whereClause ::= WHERE cond */
#line 280 "grammar.y"
{
	yymsp[-1].minor.yy111 = New_AST_WhereNode(yymsp[0].minor.yy110);
}
#line 1521 "grammar.c"
        break;
      case 41: /* cond ::= UQSTRING DOT UQSTRING relation UQSTRING DOT UQSTRING */
#line 289 "grammar.y"
{ yylhsminor.yy110 = New_AST_VaryingPredicateNode(yymsp[-6].minor.yy0.strval, yymsp[-4].minor.yy0.strval, yymsp[-3].minor.yy60, yymsp[-2].minor.yy0.strval, yymsp[0].minor.yy0.strval); }
#line 1526 "grammar.c"
  yymsp[-6].minor.yy110 = yylhsminor.yy110;
        break;
      case 42: /* cond ::= UQSTRING DOT UQSTRING relation value */
#line 292 "grammar.y"
{ yylhsminor.yy110 = New_AST_ConstantPredicateNode(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy60, yymsp[0].minor.yy102); }
#line 1532 "grammar.c"
  yymsp[-4].minor.yy110 = yylhsminor.yy110;
        break;
      case 43: /* cond ::= LEFT_PARENTHESIS cond RIGHT_PARENTHESIS */
#line 293 "grammar.y"
{ yymsp[-2].minor.yy110 = yymsp[-1].minor.yy110; }
#line 1538 "grammar.c"
        break;
      case 44: /* cond ::= cond AND cond */
#line 294 "grammar.y"
{ yylhsminor.yy110 = New_AST_ConditionNode(yymsp[-2].minor.yy110, AND, yymsp[0].minor.yy110); }
#line 1543 "grammar.c"
  yymsp[-2].minor.yy110 = yylhsminor.yy110;
        break;
      case 45: /* cond ::= cond OR cond */
#line 295 "grammar.y"
{ yylhsminor.yy110 = New_AST_ConditionNode(yymsp[-2].minor.yy110, OR, yymsp[0].minor.yy110); }
#line 1549 "grammar.c"
  yymsp[-2].minor.yy110 = yylhsminor.yy110;
        break;
      case 46: /* returnClause ::= RETURN returnElements */
#line 300 "grammar.y"
{
	yymsp[-1].minor.yy16 = New_AST_ReturnNode(yymsp[0].minor.yy38, 0);
}
#line 1557 "grammar.c"
        break;
      case 47: /* returnClause ::= RETURN DISTINCT returnElements */
#line 303 "grammar.y"
{
	yymsp[-2].minor.yy16 = New_AST_ReturnNode(yymsp[0].minor.yy38, 1);
}
#line 1564 "grammar.c"
        break;
      case 48: /* returnElements ::= returnElements COMMA returnElement */
#line 310 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy38, yymsp[0].minor.yy44);
	yylhsminor.yy38 = yymsp[-2].minor.yy38;
}
#line 1572 "grammar.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 49: /* returnElements ::= returnElement */
#line 315 "grammar.y"
{
	yylhsminor.yy38 = NewVector(AST_ReturnElementNode*, 1);
	Vector_Push(yylhsminor.yy38, yymsp[0].minor.yy44);
}
#line 1581 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 50: /* returnElement ::= arithmetic_expression */
#line 322 "grammar.y"
{
	yylhsminor.yy44 = New_AST_ReturnElementNode(yymsp[0].minor.yy28, NULL);
}
#line 1589 "grammar.c"
  yymsp[0].minor.yy44 = yylhsminor.yy44;
        break;
      case 51: /* returnElement ::= arithmetic_expression AS UQSTRING */
#line 327 "grammar.y"
{
	yylhsminor.yy44 = New_AST_ReturnElementNode(yymsp[-2].minor.yy28, yymsp[0].minor.yy0.strval);
}
#line 1597 "grammar.c"
  yymsp[-2].minor.yy44 = yylhsminor.yy44;
        break;
      case 52: /* arithmetic_expression ::= LEFT_PARENTHESIS arithmetic_expression RIGHT_PARENTHESIS */
#line 334 "grammar.y"
{
	yymsp[-2].minor.yy28 = yymsp[-1].minor.yy28;
}
#line 1605 "grammar.c"
        break;
      case 53: /* arithmetic_expression ::= arithmetic_expression ADD arithmetic_expression */
#line 346 "grammar.y"
{
	Vector *args = NewVector(AST_ArithmeticExpressionNode*, 2);
	Vector_Push(args, yymsp[-2].minor.yy28);
	Vector_Push(args, yymsp[0].minor.yy28);
	yylhsminor.yy28 = New_AST_AR_EXP_OpNode("ADD", args);
}
#line 1615 "grammar.c"
  yymsp[-2].minor.yy28 = yylhsminor.yy28;
        break;
      case 54: /* arithmetic_expression ::= arithmetic_expression DASH arithmetic_expression */
#line 353 "grammar.y"
{
	Vector *args = NewVector(AST_ArithmeticExpressionNode*, 2);
	Vector_Push(args, yymsp[-2].minor.yy28);
	Vector_Push(args, yymsp[0].minor.yy28);
	yylhsminor.yy28 = New_AST_AR_EXP_OpNode("SUB", args);
}
#line 1626 "grammar.c"
  yymsp[-2].minor.yy28 = yylhsminor.yy28;
        break;
      case 55: /* arithmetic_expression ::= arithmetic_expression MUL arithmetic_expression */
#line 360 "grammar.y"
{
	Vector *args = NewVector(AST_ArithmeticExpressionNode*, 2);
	Vector_Push(args, yymsp[-2].minor.yy28);
	Vector_Push(args, yymsp[0].minor.yy28);
	yylhsminor.yy28 = New_AST_AR_EXP_OpNode("MUL", args);
}
#line 1637 "grammar.c"
  yymsp[-2].minor.yy28 = yylhsminor.yy28;
        break;
      case 56: /* arithmetic_expression ::= arithmetic_expression DIV arithmetic_expression */
#line 367 "grammar.y"
{
	Vector *args = NewVector(AST_ArithmeticExpressionNode*, 2);
	Vector_Push(args, yymsp[-2].minor.yy28);
	Vector_Push(args, yymsp[0].minor.yy28);
	yylhsminor.yy28 = New_AST_AR_EXP_OpNode("DIV", args);
}
#line 1648 "grammar.c"
  yymsp[-2].minor.yy28 = yylhsminor.yy28;
        break;
      case 57: /* arithmetic_expression ::= UQSTRING LEFT_PARENTHESIS arithmetic_expression_list RIGHT_PARENTHESIS */
#line 375 "grammar.y"
{
	yylhsminor.yy28 = New_AST_AR_EXP_OpNode(yymsp[-3].minor.yy0.strval, yymsp[-1].minor.yy38);
}
#line 1656 "grammar.c"
  yymsp[-3].minor.yy28 = yylhsminor.yy28;
        break;
      case 58: /* arithmetic_expression ::= value */
#line 380 "grammar.y"
{
	yylhsminor.yy28 = New_AST_AR_EXP_ConstOperandNode(yymsp[0].minor.yy102);
}
#line 1664 "grammar.c"
  yymsp[0].minor.yy28 = yylhsminor.yy28;
        break;
      case 59: /* arithmetic_expression ::= variable */
#line 385 "grammar.y"
{
	yylhsminor.yy28 = New_AST_AR_EXP_VariableOperandNode(yymsp[0].minor.yy134->alias, yymsp[0].minor.yy134->property);
}
#line 1672 "grammar.c"
  yymsp[0].minor.yy28 = yylhsminor.yy28;
        break;
      case 60: /* arithmetic_expression_list ::= arithmetic_expression_list COMMA arithmetic_expression */
#line 391 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy38, yymsp[0].minor.yy28);
	yylhsminor.yy38 = yymsp[-2].minor.yy38;
}
#line 1681 "grammar.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 61: /* arithmetic_expression_list ::= arithmetic_expression */
#line 395 "grammar.y"
{
	yylhsminor.yy38 = NewVector(AST_ArithmeticExpressionNode*, 1);
	Vector_Push(yylhsminor.yy38, yymsp[0].minor.yy28);
}
#line 1690 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 62: /* variable ::= UQSTRING */
#line 402 "grammar.y"
{
	yylhsminor.yy134 = New_AST_Variable(yymsp[0].minor.yy0.strval, NULL);
}
#line 1698 "grammar.c"
  yymsp[0].minor.yy134 = yylhsminor.yy134;
        break;
      case 63: /* variable ::= UQSTRING DOT UQSTRING */
#line 406 "grammar.y"
{
	yylhsminor.yy134 = New_AST_Variable(yymsp[-2].minor.yy0.strval, yymsp[0].minor.yy0.strval);
}
#line 1706 "grammar.c"
  yymsp[-2].minor.yy134 = yylhsminor.yy134;
        break;
      case 64: /* orderClause ::= */
#line 412 "grammar.y"
{
	yymsp[1].minor.yy114 = NULL;
}
#line 1714 "grammar.c"
        break;
      case 65: /* orderClause ::= ORDER BY columnNameList */
#line 415 "grammar.y"
{
	yymsp[-2].minor.yy114 = New_AST_OrderNode(yymsp[0].minor.yy38, ORDER_DIR_ASC);
}
#line 1721 "grammar.c"
        break;
      case 66: /* orderClause ::= ORDER BY columnNameList ASC */
#line 418 "grammar.y"
{
	yymsp[-3].minor.yy114 = New_AST_OrderNode(yymsp[-1].minor.yy38, ORDER_DIR_ASC);
}
#line 1728 "grammar.c"
        break;
      case 67: /* orderClause ::= ORDER BY columnNameList DESC */
#line 421 "grammar.y"
{
	yymsp[-3].minor.yy114 = New_AST_OrderNode(yymsp[-1].minor.yy38, ORDER_DIR_DESC);
}
#line 1735 "grammar.c"
        break;
      case 68: /* columnNameList ::= columnNameList COMMA columnName */
#line 426 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy38, yymsp[0].minor.yy14);
	yylhsminor.yy38 = yymsp[-2].minor.yy38;
}
#line 1743 "grammar.c"
  yymsp[-2].minor.yy38 = yylhsminor.yy38;
        break;
      case 69: /* columnNameList ::= columnName */
#line 430 "grammar.y"
{
	yylhsminor.yy38 = NewVector(AST_ColumnNode*, 1);
	Vector_Push(yylhsminor.yy38, yymsp[0].minor.yy14);
}
#line 1752 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 70: /* columnName ::= variable */
#line 436 "grammar.y"
{
	if(yymsp[0].minor.yy134->property != NULL) {
		yylhsminor.yy14 = AST_ColumnNodeFromVariable(yymsp[0].minor.yy134);
	} else {
		yylhsminor.yy14 = AST_ColumnNodeFromAlias(yymsp[0].minor.yy134->alias);
	}

	Free_AST_Variable(yymsp[0].minor.yy134);
}
#line 1766 "grammar.c"
  yymsp[0].minor.yy14 = yylhsminor.yy14;
        break;
      case 71: /* limitClause ::= */
#line 448 "grammar.y"
{
	yymsp[1].minor.yy29 = NULL;
}
#line 1774 "grammar.c"
        break;
      case 72: /* limitClause ::= LIMIT INTEGER */
#line 451 "grammar.y"
{
	yymsp[-1].minor.yy29 = New_AST_LimitNode(yymsp[0].minor.yy0.intval);
}
#line 1781 "grammar.c"
        break;
      case 73: /* relation ::= EQ */
#line 457 "grammar.y"
{ yymsp[0].minor.yy60 = EQ; }
#line 1786 "grammar.c"
        break;
      case 74: /* relation ::= GT */
#line 458 "grammar.y"
{ yymsp[0].minor.yy60 = GT; }
#line 1791 "grammar.c"
        break;
      case 75: /* relation ::= LT */
#line 459 "grammar.y"
{ yymsp[0].minor.yy60 = LT; }
#line 1796 "grammar.c"
        break;
      case 76: /* relation ::= LE */
#line 460 "grammar.y"
{ yymsp[0].minor.yy60 = LE; }
#line 1801 "grammar.c"
        break;
      case 77: /* relation ::= GE */
#line 461 "grammar.y"
{ yymsp[0].minor.yy60 = GE; }
#line 1806 "grammar.c"
        break;
      case 78: /* relation ::= NE */
#line 462 "grammar.y"
{ yymsp[0].minor.yy60 = NE; }
#line 1811 "grammar.c"
        break;
      case 79: /* value ::= INTEGER */
#line 473 "grammar.y"
{  yylhsminor.yy102 = SI_DoubleVal(yymsp[0].minor.yy0.intval); }
#line 1816 "grammar.c"
  yymsp[0].minor.yy102 = yylhsminor.yy102;
        break;
      case 80: /* value ::= DASH INTEGER */
#line 474 "grammar.y"
{  yymsp[-1].minor.yy102 = SI_DoubleVal(-yymsp[0].minor.yy0.intval); }
#line 1822 "grammar.c"
        break;
      case 81: /* value ::= STRING */
#line 475 "grammar.y"
{  yylhsminor.yy102 = SI_StringVal(yymsp[0].minor.yy0.strval); }
#line 1827 "grammar.c"
  yymsp[0].minor.yy102 = yylhsminor.yy102;
        break;
      case 82: /* value ::= FLOAT */
#line 476 "grammar.y"
{  yylhsminor.yy102 = SI_DoubleVal(yymsp[0].minor.yy0.dval); }
#line 1833 "grammar.c"
  yymsp[0].minor.yy102 = yylhsminor.yy102;
        break;
      case 83: /* value ::= DASH FLOAT */
#line 477 "grammar.y"
{  yymsp[-1].minor.yy102 = SI_DoubleVal(-yymsp[0].minor.yy0.dval); }
#line 1839 "grammar.c"
        break;
      case 84: /* value ::= TRUE */
#line 478 "grammar.y"
{ yymsp[0].minor.yy102 = SI_BoolVal(1); }
#line 1844 "grammar.c"
        break;
      case 85: /* value ::= FALSE */
#line 479 "grammar.y"
{ yymsp[0].minor.yy102 = SI_BoolVal(0); }
#line 1849 "grammar.c"
        break;
      default:
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  yymsp += yysize+1;
  yypParser->yytos = yymsp;
  yymsp->stateno = (YYACTIONTYPE)yyact;
  yymsp->major = (YYCODETYPE)yygoto;
  yyTraceShift(yypParser, yyact, "... then shift");
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 22 "grammar.y"

	char buf[256];
	snprintf(buf, 256, "Syntax error at offset %d near '%s'\n", TOKEN.pos, TOKEN.s);

	ctx->ok = 0;
	ctx->errorMsg = strdup(buf);
#line 1914 "grammar.c"
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    int stateno = yypParser->yytos->stateno;
    if( stateno < YY_MIN_REDUCE ){
      fprintf(yyTraceFILE,"%sInput '%s' in state %d\n",
              yyTracePrompt,yyTokenName[yymajor],stateno);
    }else{
      fprintf(yyTraceFILE,"%sInput '%s' with pending reduce %d\n",
              yyTracePrompt,yyTokenName[yymajor],stateno-YY_MIN_REDUCE);
    }
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact >= YY_MIN_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE,yymajor,yyminor);
    }else if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact==YY_ACCEPT_ACTION ){
      yypParser->yytos--;
      yy_accept(yypParser);
      return;
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
#line 481 "grammar.y"


	/* Definitions of flex stuff */
	typedef struct yy_buffer_state *YY_BUFFER_STATE;
	int             yylex( void );
	YY_BUFFER_STATE yy_scan_string( const char * );
  	YY_BUFFER_STATE yy_scan_bytes( const char *, size_t );
  	extern int yylineno;
  	extern char *yytext;
	extern int yycolumn;

	AST_Query* Query_Parse(const char *q, size_t len, char **err) {
		yycolumn = 1;	// Reset lexer's token tracking position
		yy_scan_bytes(q, len);
  		void* pParser = ParseAlloc(malloc);
  		int t = 0;

		parseCtx ctx = {.root = NULL, .ok = 1, .errorMsg = NULL};

  		while( (t = yylex()) != 0) {
			Parse(pParser, t, tok, &ctx);
		}
		if (ctx.ok) {
			Parse(pParser, 0, tok, &ctx);
  		}
		ParseFree(pParser, free);
		if (err) {
			*err = ctx.errorMsg;
		}
		return ctx.root;
	}
#line 2160 "grammar.c"
