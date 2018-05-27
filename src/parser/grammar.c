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
	#include "./clauses/clauses.h"
	#include "parse.h"
	#include "../value.h"

	void yyerror(char *s);
#line 41 "grammar.c"
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
#define YYNOCODE 81
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  AST_Variable* yy20;
  SIValue yy38;
  AST_LinkEntity* yy45;
  AST_LimitNode* yy47;
  AST_WhereNode* yy51;
  AST_FilterNode* yy66;
  AST_OrderNode* yy68;
  AST_NodeEntity* yy69;
  AST_ColumnNode* yy70;
  AST_CreateNode* yy76;
  AST_MatchNode* yy85;
  int yy92;
  AST_DeleteNode * yy95;
  AST_SetElement* yy104;
  AST_IndexOpType yy105;
  AST_ArithmeticExpressionNode* yy114;
  AST_ReturnNode* yy128;
  AST_SetNode* yy140;
  Vector* yy146;
  AST_Query* yy152;
  AST_ReturnElementNode* yy154;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  parseCtx *ctx ;
#define ParseARG_PDECL , parseCtx *ctx 
#define ParseARG_FETCH  parseCtx *ctx  = yypParser->ctx 
#define ParseARG_STORE yypParser->ctx  = ctx 
#define YYNSTATE             107
#define YYNRULE              87
#define YYNTOKEN             46
#define YY_MAX_SHIFT         106
#define YY_MIN_SHIFTREDUCE   166
#define YY_MAX_SHIFTREDUCE   252
#define YY_ERROR_ACTION      253
#define YY_ACCEPT_ACTION     254
#define YY_NO_ACTION         255
#define YY_MIN_REDUCE        256
#define YY_MAX_REDUCE        342
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
#define YY_ACTTAB_COUNT (242)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   299,  106,  254,   52,   62,  262,   40,   11,    9,    8,
 /*    10 */     7,   71,  240,  241,  244,  242,  243,   64,   10,  316,
 /*    20 */    54,  327,   62,  219,   66,  316,   57,   37,  315,   30,
 /*    30 */   275,  101,  306,    2,  315,   64,   10,  325,   62,   63,
 /*    40 */   246,   25,  248,  249,  251,  252,  245,   62,  316,   55,
 /*    50 */   103,   74,   21,   14,  175,  316,   54,  315,  246,   13,
 /*    60 */   248,  249,  251,  252,  315,   51,  327,   99,  306,  259,
 /*    70 */    48,  316,   54,   93,  246,   87,  248,  249,  251,  252,
 /*    80 */   315,   59,  326,  246,  305,  248,  249,  251,  252,   11,
 /*    90 */     9,    8,    7,   11,    9,    8,    7,   89,  316,   56,
 /*   100 */   316,  313,   72,  316,  312,   83,   31,  315,   45,  315,
 /*   110 */   316,   65,  315,  316,   53,  316,   61,   43,   16,  315,
 /*   120 */   100,   58,  315,   98,  315,   15,  301,   36,   30,  275,
 /*   130 */    80,  272,   79,   91,   32,   26,   28,  273,   79,   85,
 /*   140 */    20,  247,   33,  275,  250,   43,   76,   27,   97,   26,
 /*   150 */    28,   43,   60,  210,    3,  233,  234,    8,    7,  224,
 /*   160 */    43,   86,    6,   92,   69,  263,   67,  102,   34,   24,
 /*   170 */   190,   78,   81,   43,   82,   94,   84,  105,   88,  261,
 /*   180 */    90,  276,  104,  295,   95,   96,   46,    1,   49,   47,
 /*   190 */    29,   12,   19,  257,   50,  176,  177,   68,   35,   70,
 /*   200 */    28,  208,   73,   22,   75,  191,    5,  197,  200,   38,
 /*   210 */   201,   77,   18,   39,  239,   17,  195,   41,  199,  196,
 /*   220 */   198,  193,   23,  194,  256,   44,  192,   42,  218,  230,
 /*   230 */   203,    4,  255,  255,  255,  255,  255,  255,  255,  255,
 /*   240 */   255,  102,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    72,   47,   48,   49,    4,   51,    4,    3,    4,    5,
 /*    10 */     6,   57,    7,    8,    9,   10,   11,   17,   18,   63,
 /*    20 */    64,   63,    4,   19,   60,   63,   64,   25,   72,   65,
 /*    30 */    66,   75,   76,   33,   72,   17,   18,   79,    4,   77,
 /*    40 */    40,   18,   42,   43,   44,   45,   41,    4,   63,   64,
 /*    50 */    17,   17,   74,   14,   15,   63,   64,   72,   40,   20,
 /*    60 */    42,   43,   44,   45,   72,   51,   63,   75,   76,   55,
 /*    70 */    56,   63,   64,   72,   40,   69,   42,   43,   44,   45,
 /*    80 */    72,   78,   79,   40,   76,   42,   43,   44,   45,    3,
 /*    90 */     4,    5,    6,    3,    4,    5,    6,   69,   63,   64,
 /*   100 */    63,   64,   73,   63,   64,   16,   17,   72,   67,   72,
 /*   110 */    63,   64,   72,   63,   64,   63,   64,   28,   14,   72,
 /*   120 */    34,   73,   72,   60,   72,   21,   73,   23,   65,   66,
 /*   130 */    61,   62,   63,   16,   17,    1,    2,   62,   63,   16,
 /*   140 */    22,   40,   65,   66,   43,   28,   17,   18,   16,    1,
 /*   150 */     2,   28,   73,   19,   18,   37,   38,    5,    6,   19,
 /*   160 */    28,   69,   22,   69,   18,   59,   16,   31,   58,   26,
 /*   170 */    17,   68,   70,   28,   69,   17,   69,   39,   70,   54,
 /*   180 */    69,   66,   35,   71,   71,   69,   53,   32,   53,   52,
 /*   190 */    30,   50,   22,   54,   52,   17,   19,   17,   13,   12,
 /*   200 */     2,   17,   31,   17,   31,   17,    7,    4,   27,   17,
 /*   210 */    27,   22,   22,   17,   40,   36,   19,   17,   27,   24,
 /*   220 */    27,   19,   16,   19,    0,   17,   19,   22,   17,   17,
 /*   230 */    29,   22,   80,   80,   80,   80,   80,   80,   80,   80,
 /*   240 */    80,   31,   80,   80,   80,   80,   80,   80,   80,   80,
 /*   250 */    80,   80,   80,   80,   80,   80,   80,   80,   80,   80,
 /*   260 */    80,   80,   80,   80,   80,   80,   80,   80,   80,   80,
 /*   270 */    80,   80,   80,   80,   80,   80,   80,   80,   80,   80,
 /*   280 */    80,   80,   80,   80,   80,   80,   80,   80,
};
#define YY_SHIFT_COUNT    (106)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (224)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */    39,    0,   18,   18,   18,   18,   18,   18,   18,   18,
 /*    10 */    18,   18,  104,   23,   23,   33,   23,   33,   33,   23,
 /*    20 */    33,   34,    5,   43,   89,  117,  129,  129,  129,  129,
 /*    30 */     2,  123,  132,    2,  146,  150,  153,  143,  145,  145,
 /*    40 */   143,  145,  158,  158,  145,   23,  138,  147,  155,  138,
 /*    50 */   147,  155,  160,    4,   86,   90,   90,   90,  134,  118,
 /*    60 */   148,  152,  101,  140,  136,  152,  170,  178,  177,  180,
 /*    70 */   185,  187,  198,  184,  171,  186,  173,  188,  189,  199,
 /*    80 */   190,  203,  181,  192,  183,  196,  191,  193,  195,  197,
 /*    90 */   202,  200,  204,  205,  206,  201,  207,  208,  170,  209,
 /*   100 */   211,  209,  212,  210,  179,  174,  224,
};
#define YY_REDUCE_COUNT (52)
#define YY_REDUCE_MIN   (-72)
#define YY_REDUCE_MAX   (142)
static const short yy_reduce_ofst[] = {
 /*     0 */   -46,  -44,   -8,  -38,    8,  -15,   35,   37,   40,   47,
 /*    10 */    50,   52,   14,  -36,   63,   69,   63,    3,   75,   77,
 /*    20 */   -42,  -72,  -22,    1,    6,   28,   29,   48,   53,   79,
 /*    30 */    41,   92,   94,   41,  106,  110,  103,  102,  105,  107,
 /*    40 */   108,  111,  112,  113,  116,  115,  125,  133,  137,  139,
 /*    50 */   135,  142,  141,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   269,  253,  253,  253,  253,  253,  253,  253,  253,  253,
 /*    10 */   253,  253,  269,  253,  264,  253,  253,  253,  253,  253,
 /*    20 */   253,  253,  253,  253,  292,  292,  253,  253,  253,  253,
 /*    30 */   277,  292,  292,  278,  253,  253,  253,  253,  292,  292,
 /*    40 */   253,  292,  253,  253,  292,  253,  328,  321,  260,  328,
 /*    50 */   321,  258,  296,  253,  307,  274,  317,  318,  253,  322,
 /*    60 */   297,  310,  253,  253,  319,  311,  268,  253,  253,  253,
 /*    70 */   253,  253,  302,  253,  253,  253,  253,  253,  279,  253,
 /*    80 */   271,  253,  253,  253,  253,  253,  253,  253,  253,  253,
 /*    90 */   253,  253,  253,  294,  253,  253,  253,  253,  270,  304,
 /*   100 */   253,  303,  253,  319,  253,  253,  253,
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
  /*   12 */ "INDEX",
  /*   13 */ "ON",
  /*   14 */ "CREATE",
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
  /*   57 */ "indexOpToken",
  /*   58 */ "indexLabel",
  /*   59 */ "indexProp",
  /*   60 */ "chains",
  /*   61 */ "setList",
  /*   62 */ "setElement",
  /*   63 */ "variable",
  /*   64 */ "arithmetic_expression",
  /*   65 */ "chain",
  /*   66 */ "node",
  /*   67 */ "link",
  /*   68 */ "deleteExpression",
  /*   69 */ "properties",
  /*   70 */ "edge",
  /*   71 */ "mapLiteral",
  /*   72 */ "value",
  /*   73 */ "cond",
  /*   74 */ "relation",
  /*   75 */ "returnElements",
  /*   76 */ "returnElement",
  /*   77 */ "arithmetic_expression_list",
  /*   78 */ "columnNameList",
  /*   79 */ "columnName",
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
 /*   7 */ "expr ::= indexOpToken INDEX ON indexLabel indexProp",
 /*   8 */ "indexOpToken ::= CREATE",
 /*   9 */ "indexOpToken ::= DROP",
 /*  10 */ "indexLabel ::= COLON UQSTRING",
 /*  11 */ "indexProp ::= LEFT_PARENTHESIS UQSTRING RIGHT_PARENTHESIS",
 /*  12 */ "matchClause ::= MATCH chains",
 /*  13 */ "createClause ::=",
 /*  14 */ "createClause ::= CREATE chains",
 /*  15 */ "setClause ::= SET setList",
 /*  16 */ "setList ::= setElement",
 /*  17 */ "setList ::= setList COMMA setElement",
 /*  18 */ "setElement ::= variable EQ arithmetic_expression",
 /*  19 */ "chain ::= node",
 /*  20 */ "chain ::= chain link node",
 /*  21 */ "chains ::= chain",
 /*  22 */ "chains ::= chains COMMA chain",
 /*  23 */ "deleteClause ::= DELETE deleteExpression",
 /*  24 */ "deleteExpression ::= UQSTRING",
 /*  25 */ "deleteExpression ::= deleteExpression COMMA UQSTRING",
 /*  26 */ "node ::= LEFT_PARENTHESIS UQSTRING COLON UQSTRING properties RIGHT_PARENTHESIS",
 /*  27 */ "node ::= LEFT_PARENTHESIS COLON UQSTRING properties RIGHT_PARENTHESIS",
 /*  28 */ "node ::= LEFT_PARENTHESIS UQSTRING properties RIGHT_PARENTHESIS",
 /*  29 */ "node ::= LEFT_PARENTHESIS properties RIGHT_PARENTHESIS",
 /*  30 */ "link ::= DASH edge RIGHT_ARROW",
 /*  31 */ "link ::= LEFT_ARROW edge DASH",
 /*  32 */ "edge ::= LEFT_BRACKET properties RIGHT_BRACKET",
 /*  33 */ "edge ::= LEFT_BRACKET UQSTRING properties RIGHT_BRACKET",
 /*  34 */ "edge ::= LEFT_BRACKET COLON UQSTRING properties RIGHT_BRACKET",
 /*  35 */ "edge ::= LEFT_BRACKET UQSTRING COLON UQSTRING properties RIGHT_BRACKET",
 /*  36 */ "properties ::=",
 /*  37 */ "properties ::= LEFT_CURLY_BRACKET mapLiteral RIGHT_CURLY_BRACKET",
 /*  38 */ "mapLiteral ::= UQSTRING COLON value",
 /*  39 */ "mapLiteral ::= UQSTRING COLON value COMMA mapLiteral",
 /*  40 */ "whereClause ::=",
 /*  41 */ "whereClause ::= WHERE cond",
 /*  42 */ "cond ::= UQSTRING DOT UQSTRING relation UQSTRING DOT UQSTRING",
 /*  43 */ "cond ::= UQSTRING DOT UQSTRING relation value",
 /*  44 */ "cond ::= LEFT_PARENTHESIS cond RIGHT_PARENTHESIS",
 /*  45 */ "cond ::= cond AND cond",
 /*  46 */ "cond ::= cond OR cond",
 /*  47 */ "returnClause ::= RETURN returnElements",
 /*  48 */ "returnClause ::= RETURN DISTINCT returnElements",
 /*  49 */ "returnElements ::= returnElements COMMA returnElement",
 /*  50 */ "returnElements ::= returnElement",
 /*  51 */ "returnElement ::= arithmetic_expression",
 /*  52 */ "returnElement ::= arithmetic_expression AS UQSTRING",
 /*  53 */ "arithmetic_expression ::= LEFT_PARENTHESIS arithmetic_expression RIGHT_PARENTHESIS",
 /*  54 */ "arithmetic_expression ::= arithmetic_expression ADD arithmetic_expression",
 /*  55 */ "arithmetic_expression ::= arithmetic_expression DASH arithmetic_expression",
 /*  56 */ "arithmetic_expression ::= arithmetic_expression MUL arithmetic_expression",
 /*  57 */ "arithmetic_expression ::= arithmetic_expression DIV arithmetic_expression",
 /*  58 */ "arithmetic_expression ::= UQSTRING LEFT_PARENTHESIS arithmetic_expression_list RIGHT_PARENTHESIS",
 /*  59 */ "arithmetic_expression ::= value",
 /*  60 */ "arithmetic_expression ::= variable",
 /*  61 */ "arithmetic_expression_list ::= arithmetic_expression_list COMMA arithmetic_expression",
 /*  62 */ "arithmetic_expression_list ::= arithmetic_expression",
 /*  63 */ "variable ::= UQSTRING",
 /*  64 */ "variable ::= UQSTRING DOT UQSTRING",
 /*  65 */ "orderClause ::=",
 /*  66 */ "orderClause ::= ORDER BY columnNameList",
 /*  67 */ "orderClause ::= ORDER BY columnNameList ASC",
 /*  68 */ "orderClause ::= ORDER BY columnNameList DESC",
 /*  69 */ "columnNameList ::= columnNameList COMMA columnName",
 /*  70 */ "columnNameList ::= columnName",
 /*  71 */ "columnName ::= variable",
 /*  72 */ "limitClause ::=",
 /*  73 */ "limitClause ::= LIMIT INTEGER",
 /*  74 */ "relation ::= EQ",
 /*  75 */ "relation ::= GT",
 /*  76 */ "relation ::= LT",
 /*  77 */ "relation ::= LE",
 /*  78 */ "relation ::= GE",
 /*  79 */ "relation ::= NE",
 /*  80 */ "value ::= INTEGER",
 /*  81 */ "value ::= DASH INTEGER",
 /*  82 */ "value ::= STRING",
 /*  83 */ "value ::= FLOAT",
 /*  84 */ "value ::= DASH FLOAT",
 /*  85 */ "value ::= TRUE",
 /*  86 */ "value ::= FALSE",
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
    case 73: /* cond */
{
#line 285 "grammar.y"
 Free_AST_FilterNode((yypminor->yy66)); 
#line 715 "grammar.c"
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
  {   47,   -5 }, /* (7) expr ::= indexOpToken INDEX ON indexLabel indexProp */
  {   57,   -1 }, /* (8) indexOpToken ::= CREATE */
  {   57,   -1 }, /* (9) indexOpToken ::= DROP */
  {   58,   -2 }, /* (10) indexLabel ::= COLON UQSTRING */
  {   59,   -3 }, /* (11) indexProp ::= LEFT_PARENTHESIS UQSTRING RIGHT_PARENTHESIS */
  {   49,   -2 }, /* (12) matchClause ::= MATCH chains */
  {   51,    0 }, /* (13) createClause ::= */
  {   51,   -2 }, /* (14) createClause ::= CREATE chains */
  {   56,   -2 }, /* (15) setClause ::= SET setList */
  {   61,   -1 }, /* (16) setList ::= setElement */
  {   61,   -3 }, /* (17) setList ::= setList COMMA setElement */
  {   62,   -3 }, /* (18) setElement ::= variable EQ arithmetic_expression */
  {   65,   -1 }, /* (19) chain ::= node */
  {   65,   -3 }, /* (20) chain ::= chain link node */
  {   60,   -1 }, /* (21) chains ::= chain */
  {   60,   -3 }, /* (22) chains ::= chains COMMA chain */
  {   55,   -2 }, /* (23) deleteClause ::= DELETE deleteExpression */
  {   68,   -1 }, /* (24) deleteExpression ::= UQSTRING */
  {   68,   -3 }, /* (25) deleteExpression ::= deleteExpression COMMA UQSTRING */
  {   66,   -6 }, /* (26) node ::= LEFT_PARENTHESIS UQSTRING COLON UQSTRING properties RIGHT_PARENTHESIS */
  {   66,   -5 }, /* (27) node ::= LEFT_PARENTHESIS COLON UQSTRING properties RIGHT_PARENTHESIS */
  {   66,   -4 }, /* (28) node ::= LEFT_PARENTHESIS UQSTRING properties RIGHT_PARENTHESIS */
  {   66,   -3 }, /* (29) node ::= LEFT_PARENTHESIS properties RIGHT_PARENTHESIS */
  {   67,   -3 }, /* (30) link ::= DASH edge RIGHT_ARROW */
  {   67,   -3 }, /* (31) link ::= LEFT_ARROW edge DASH */
  {   70,   -3 }, /* (32) edge ::= LEFT_BRACKET properties RIGHT_BRACKET */
  {   70,   -4 }, /* (33) edge ::= LEFT_BRACKET UQSTRING properties RIGHT_BRACKET */
  {   70,   -5 }, /* (34) edge ::= LEFT_BRACKET COLON UQSTRING properties RIGHT_BRACKET */
  {   70,   -6 }, /* (35) edge ::= LEFT_BRACKET UQSTRING COLON UQSTRING properties RIGHT_BRACKET */
  {   69,    0 }, /* (36) properties ::= */
  {   69,   -3 }, /* (37) properties ::= LEFT_CURLY_BRACKET mapLiteral RIGHT_CURLY_BRACKET */
  {   71,   -3 }, /* (38) mapLiteral ::= UQSTRING COLON value */
  {   71,   -5 }, /* (39) mapLiteral ::= UQSTRING COLON value COMMA mapLiteral */
  {   50,    0 }, /* (40) whereClause ::= */
  {   50,   -2 }, /* (41) whereClause ::= WHERE cond */
  {   73,   -7 }, /* (42) cond ::= UQSTRING DOT UQSTRING relation UQSTRING DOT UQSTRING */
  {   73,   -5 }, /* (43) cond ::= UQSTRING DOT UQSTRING relation value */
  {   73,   -3 }, /* (44) cond ::= LEFT_PARENTHESIS cond RIGHT_PARENTHESIS */
  {   73,   -3 }, /* (45) cond ::= cond AND cond */
  {   73,   -3 }, /* (46) cond ::= cond OR cond */
  {   52,   -2 }, /* (47) returnClause ::= RETURN returnElements */
  {   52,   -3 }, /* (48) returnClause ::= RETURN DISTINCT returnElements */
  {   75,   -3 }, /* (49) returnElements ::= returnElements COMMA returnElement */
  {   75,   -1 }, /* (50) returnElements ::= returnElement */
  {   76,   -1 }, /* (51) returnElement ::= arithmetic_expression */
  {   76,   -3 }, /* (52) returnElement ::= arithmetic_expression AS UQSTRING */
  {   64,   -3 }, /* (53) arithmetic_expression ::= LEFT_PARENTHESIS arithmetic_expression RIGHT_PARENTHESIS */
  {   64,   -3 }, /* (54) arithmetic_expression ::= arithmetic_expression ADD arithmetic_expression */
  {   64,   -3 }, /* (55) arithmetic_expression ::= arithmetic_expression DASH arithmetic_expression */
  {   64,   -3 }, /* (56) arithmetic_expression ::= arithmetic_expression MUL arithmetic_expression */
  {   64,   -3 }, /* (57) arithmetic_expression ::= arithmetic_expression DIV arithmetic_expression */
  {   64,   -4 }, /* (58) arithmetic_expression ::= UQSTRING LEFT_PARENTHESIS arithmetic_expression_list RIGHT_PARENTHESIS */
  {   64,   -1 }, /* (59) arithmetic_expression ::= value */
  {   64,   -1 }, /* (60) arithmetic_expression ::= variable */
  {   77,   -3 }, /* (61) arithmetic_expression_list ::= arithmetic_expression_list COMMA arithmetic_expression */
  {   77,   -1 }, /* (62) arithmetic_expression_list ::= arithmetic_expression */
  {   63,   -1 }, /* (63) variable ::= UQSTRING */
  {   63,   -3 }, /* (64) variable ::= UQSTRING DOT UQSTRING */
  {   53,    0 }, /* (65) orderClause ::= */
  {   53,   -3 }, /* (66) orderClause ::= ORDER BY columnNameList */
  {   53,   -4 }, /* (67) orderClause ::= ORDER BY columnNameList ASC */
  {   53,   -4 }, /* (68) orderClause ::= ORDER BY columnNameList DESC */
  {   78,   -3 }, /* (69) columnNameList ::= columnNameList COMMA columnName */
  {   78,   -1 }, /* (70) columnNameList ::= columnName */
  {   79,   -1 }, /* (71) columnName ::= variable */
  {   54,    0 }, /* (72) limitClause ::= */
  {   54,   -2 }, /* (73) limitClause ::= LIMIT INTEGER */
  {   74,   -1 }, /* (74) relation ::= EQ */
  {   74,   -1 }, /* (75) relation ::= GT */
  {   74,   -1 }, /* (76) relation ::= LT */
  {   74,   -1 }, /* (77) relation ::= LE */
  {   74,   -1 }, /* (78) relation ::= GE */
  {   74,   -1 }, /* (79) relation ::= NE */
  {   72,   -1 }, /* (80) value ::= INTEGER */
  {   72,   -2 }, /* (81) value ::= DASH INTEGER */
  {   72,   -1 }, /* (82) value ::= STRING */
  {   72,   -1 }, /* (83) value ::= FLOAT */
  {   72,   -2 }, /* (84) value ::= DASH FLOAT */
  {   72,   -1 }, /* (85) value ::= TRUE */
  {   72,   -1 }, /* (86) value ::= FALSE */
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
#line 36 "grammar.y"
{ ctx->root = yymsp[0].minor.yy152; }
#line 1179 "grammar.c"
        break;
      case 1: /* expr ::= matchClause whereClause createClause returnClause orderClause limitClause */
#line 38 "grammar.y"
{
	yylhsminor.yy152 = New_AST_Query(yymsp[-5].minor.yy85, yymsp[-4].minor.yy51, yymsp[-3].minor.yy76, NULL, NULL, NULL, yymsp[-2].minor.yy128, yymsp[-1].minor.yy68, yymsp[0].minor.yy47, NULL);
}
#line 1186 "grammar.c"
  yymsp[-5].minor.yy152 = yylhsminor.yy152;
        break;
      case 2: /* expr ::= matchClause whereClause createClause */
#line 42 "grammar.y"
{
	yylhsminor.yy152 = New_AST_Query(yymsp[-2].minor.yy85, yymsp[-1].minor.yy51, yymsp[0].minor.yy76, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
#line 1194 "grammar.c"
  yymsp[-2].minor.yy152 = yylhsminor.yy152;
        break;
      case 3: /* expr ::= matchClause whereClause deleteClause */
#line 46 "grammar.y"
{
	yylhsminor.yy152 = New_AST_Query(yymsp[-2].minor.yy85, yymsp[-1].minor.yy51, NULL, NULL, NULL, yymsp[0].minor.yy95, NULL, NULL, NULL, NULL);
}
#line 1202 "grammar.c"
  yymsp[-2].minor.yy152 = yylhsminor.yy152;
        break;
      case 4: /* expr ::= matchClause whereClause setClause */
#line 50 "grammar.y"
{
	yylhsminor.yy152 = New_AST_Query(yymsp[-2].minor.yy85, yymsp[-1].minor.yy51, NULL, NULL, yymsp[0].minor.yy140, NULL, NULL, NULL, NULL, NULL);
}
#line 1210 "grammar.c"
  yymsp[-2].minor.yy152 = yylhsminor.yy152;
        break;
      case 5: /* expr ::= matchClause whereClause setClause returnClause orderClause limitClause */
#line 54 "grammar.y"
{
	yylhsminor.yy152 = New_AST_Query(yymsp[-5].minor.yy85, yymsp[-4].minor.yy51, NULL, NULL, yymsp[-3].minor.yy140, NULL, yymsp[-2].minor.yy128, yymsp[-1].minor.yy68, yymsp[0].minor.yy47, NULL);
}
#line 1218 "grammar.c"
  yymsp[-5].minor.yy152 = yylhsminor.yy152;
        break;
      case 6: /* expr ::= createClause */
#line 58 "grammar.y"
{
	yylhsminor.yy152 = New_AST_Query(NULL, NULL, yymsp[0].minor.yy76, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
#line 1226 "grammar.c"
  yymsp[0].minor.yy152 = yylhsminor.yy152;
        break;
      case 7: /* expr ::= indexOpToken INDEX ON indexLabel indexProp */
#line 66 "grammar.y"
{
	yylhsminor.yy152 = New_AST_Query(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, AST_IndexOp(yymsp[-1].minor.yy0.strval, yymsp[0].minor.yy0.strval, yymsp[-4].minor.yy105));
}
#line 1234 "grammar.c"
  yymsp[-4].minor.yy152 = yylhsminor.yy152;
        break;
      case 8: /* indexOpToken ::= CREATE */
#line 72 "grammar.y"
{ yymsp[0].minor.yy105 = CREATE_INDEX; }
#line 1240 "grammar.c"
        break;
      case 9: /* indexOpToken ::= DROP */
#line 73 "grammar.y"
{ yymsp[0].minor.yy105 = DROP_INDEX; }
#line 1245 "grammar.c"
        break;
      case 10: /* indexLabel ::= COLON UQSTRING */
#line 75 "grammar.y"
{
  yymsp[-1].minor.yy0 = yymsp[0].minor.yy0;
}
#line 1252 "grammar.c"
        break;
      case 11: /* indexProp ::= LEFT_PARENTHESIS UQSTRING RIGHT_PARENTHESIS */
#line 79 "grammar.y"
{
  yymsp[-2].minor.yy0 = yymsp[-1].minor.yy0;
}
#line 1259 "grammar.c"
        break;
      case 12: /* matchClause ::= MATCH chains */
#line 85 "grammar.y"
{
	yymsp[-1].minor.yy85 = New_AST_MatchNode(yymsp[0].minor.yy146);
}
#line 1266 "grammar.c"
        break;
      case 13: /* createClause ::= */
#line 92 "grammar.y"
{
	yymsp[1].minor.yy76 = NULL;
}
#line 1273 "grammar.c"
        break;
      case 14: /* createClause ::= CREATE chains */
#line 96 "grammar.y"
{
	yymsp[-1].minor.yy76 = New_AST_CreateNode(yymsp[0].minor.yy146);
}
#line 1280 "grammar.c"
        break;
      case 15: /* setClause ::= SET setList */
#line 113 "grammar.y"
{
	yymsp[-1].minor.yy140 = New_AST_SetNode(yymsp[0].minor.yy146);
}
#line 1287 "grammar.c"
        break;
      case 16: /* setList ::= setElement */
#line 118 "grammar.y"
{
	yylhsminor.yy146 = NewVector(AST_SetElement*, 1);
	Vector_Push(yylhsminor.yy146, yymsp[0].minor.yy104);
}
#line 1295 "grammar.c"
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 17: /* setList ::= setList COMMA setElement */
#line 122 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy146, yymsp[0].minor.yy104);
	yylhsminor.yy146 = yymsp[-2].minor.yy146;
}
#line 1304 "grammar.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 18: /* setElement ::= variable EQ arithmetic_expression */
#line 128 "grammar.y"
{
	yylhsminor.yy104 = New_AST_SetElement(yymsp[-2].minor.yy20, yymsp[0].minor.yy114);
}
#line 1312 "grammar.c"
  yymsp[-2].minor.yy104 = yylhsminor.yy104;
        break;
      case 19: /* chain ::= node */
#line 134 "grammar.y"
{
	yylhsminor.yy146 = NewVector(AST_GraphEntity*, 1);
	Vector_Push(yylhsminor.yy146, yymsp[0].minor.yy69);
}
#line 1321 "grammar.c"
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 20: /* chain ::= chain link node */
#line 139 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy146, yymsp[-1].minor.yy45);
	Vector_Push(yymsp[-2].minor.yy146, yymsp[0].minor.yy69);
	yylhsminor.yy146 = yymsp[-2].minor.yy146;
}
#line 1331 "grammar.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 21: /* chains ::= chain */
#line 146 "grammar.y"
{
	yylhsminor.yy146 = yymsp[0].minor.yy146;
}
#line 1339 "grammar.c"
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 22: /* chains ::= chains COMMA chain */
#line 150 "grammar.y"
{
	for(int i = 0; i < Vector_Size(yymsp[0].minor.yy146); i++) {
		AST_GraphEntity *entity;
		Vector_Get(yymsp[0].minor.yy146, i, &entity);
		Vector_Push(yymsp[-2].minor.yy146, entity);
	}
	Vector_Free(yymsp[0].minor.yy146);
	yylhsminor.yy146 = yymsp[-2].minor.yy146;
}
#line 1353 "grammar.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 23: /* deleteClause ::= DELETE deleteExpression */
#line 163 "grammar.y"
{
	yymsp[-1].minor.yy95 = New_AST_DeleteNode(yymsp[0].minor.yy146);
}
#line 1361 "grammar.c"
        break;
      case 24: /* deleteExpression ::= UQSTRING */
#line 169 "grammar.y"
{
	yylhsminor.yy146 = NewVector(char*, 1);
	Vector_Push(yylhsminor.yy146, yymsp[0].minor.yy0.strval);
}
#line 1369 "grammar.c"
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 25: /* deleteExpression ::= deleteExpression COMMA UQSTRING */
#line 174 "grammar.y"
{
  Vector_Push(yymsp[-2].minor.yy146, yymsp[0].minor.yy0.strval);
	yylhsminor.yy146 = yymsp[-2].minor.yy146;
}
#line 1378 "grammar.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 26: /* node ::= LEFT_PARENTHESIS UQSTRING COLON UQSTRING properties RIGHT_PARENTHESIS */
#line 182 "grammar.y"
{
	yymsp[-5].minor.yy69 = New_AST_NodeEntity(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy146);
}
#line 1386 "grammar.c"
        break;
      case 27: /* node ::= LEFT_PARENTHESIS COLON UQSTRING properties RIGHT_PARENTHESIS */
#line 187 "grammar.y"
{
	yymsp[-4].minor.yy69 = New_AST_NodeEntity(NULL, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy146);
}
#line 1393 "grammar.c"
        break;
      case 28: /* node ::= LEFT_PARENTHESIS UQSTRING properties RIGHT_PARENTHESIS */
#line 192 "grammar.y"
{
	yymsp[-3].minor.yy69 = New_AST_NodeEntity(yymsp[-2].minor.yy0.strval, NULL, yymsp[-1].minor.yy146);
}
#line 1400 "grammar.c"
        break;
      case 29: /* node ::= LEFT_PARENTHESIS properties RIGHT_PARENTHESIS */
#line 197 "grammar.y"
{
	yymsp[-2].minor.yy69 = New_AST_NodeEntity(NULL, NULL, yymsp[-1].minor.yy146);
}
#line 1407 "grammar.c"
        break;
      case 30: /* link ::= DASH edge RIGHT_ARROW */
#line 204 "grammar.y"
{
	yymsp[-2].minor.yy45 = yymsp[-1].minor.yy45;
	yymsp[-2].minor.yy45->direction = N_LEFT_TO_RIGHT;
}
#line 1415 "grammar.c"
        break;
      case 31: /* link ::= LEFT_ARROW edge DASH */
#line 210 "grammar.y"
{
	yymsp[-2].minor.yy45 = yymsp[-1].minor.yy45;
	yymsp[-2].minor.yy45->direction = N_RIGHT_TO_LEFT;
}
#line 1423 "grammar.c"
        break;
      case 32: /* edge ::= LEFT_BRACKET properties RIGHT_BRACKET */
#line 217 "grammar.y"
{ 
	yymsp[-2].minor.yy45 = New_AST_LinkEntity(NULL, NULL, yymsp[-1].minor.yy146, N_DIR_NOT_SPECIFIED);
}
#line 1430 "grammar.c"
        break;
      case 33: /* edge ::= LEFT_BRACKET UQSTRING properties RIGHT_BRACKET */
#line 222 "grammar.y"
{ 
	yymsp[-3].minor.yy45 = New_AST_LinkEntity(yymsp[-2].minor.yy0.strval, NULL, yymsp[-1].minor.yy146, N_DIR_NOT_SPECIFIED);
}
#line 1437 "grammar.c"
        break;
      case 34: /* edge ::= LEFT_BRACKET COLON UQSTRING properties RIGHT_BRACKET */
#line 227 "grammar.y"
{ 
	yymsp[-4].minor.yy45 = New_AST_LinkEntity(NULL, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy146, N_DIR_NOT_SPECIFIED);
}
#line 1444 "grammar.c"
        break;
      case 35: /* edge ::= LEFT_BRACKET UQSTRING COLON UQSTRING properties RIGHT_BRACKET */
#line 232 "grammar.y"
{ 
	yymsp[-5].minor.yy45 = New_AST_LinkEntity(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy146, N_DIR_NOT_SPECIFIED);
}
#line 1451 "grammar.c"
        break;
      case 36: /* properties ::= */
#line 238 "grammar.y"
{
	yymsp[1].minor.yy146 = NULL;
}
#line 1458 "grammar.c"
        break;
      case 37: /* properties ::= LEFT_CURLY_BRACKET mapLiteral RIGHT_CURLY_BRACKET */
#line 242 "grammar.y"
{
	yymsp[-2].minor.yy146 = yymsp[-1].minor.yy146;
}
#line 1465 "grammar.c"
        break;
      case 38: /* mapLiteral ::= UQSTRING COLON value */
#line 248 "grammar.y"
{
	yylhsminor.yy146 = NewVector(SIValue*, 2);

	SIValue *key = malloc(sizeof(SIValue));
	*key = SI_StringVal(yymsp[-2].minor.yy0.strval);

	Vector_Push(yylhsminor.yy146, key);

	SIValue *val = malloc(sizeof(SIValue));
	*val = yymsp[0].minor.yy38;
	Vector_Push(yylhsminor.yy146, val);
}
#line 1481 "grammar.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 39: /* mapLiteral ::= UQSTRING COLON value COMMA mapLiteral */
#line 261 "grammar.y"
{
	SIValue *key = malloc(sizeof(SIValue));
	*key = SI_StringVal(yymsp[-4].minor.yy0.strval);

	Vector_Push(yymsp[0].minor.yy146, key);

	SIValue *val = malloc(sizeof(SIValue));
	*val = yymsp[-2].minor.yy38;
	Vector_Push(yymsp[0].minor.yy146, val);
	
	yylhsminor.yy146 = yymsp[0].minor.yy146;
}
#line 1498 "grammar.c"
  yymsp[-4].minor.yy146 = yylhsminor.yy146;
        break;
      case 40: /* whereClause ::= */
#line 276 "grammar.y"
{ 
	yymsp[1].minor.yy51 = NULL;
}
#line 1506 "grammar.c"
        break;
      case 41: /* whereClause ::= WHERE cond */
#line 279 "grammar.y"
{
	yymsp[-1].minor.yy51 = New_AST_WhereNode(yymsp[0].minor.yy66);
}
#line 1513 "grammar.c"
        break;
      case 42: /* cond ::= UQSTRING DOT UQSTRING relation UQSTRING DOT UQSTRING */
#line 288 "grammar.y"
{ yylhsminor.yy66 = New_AST_VaryingPredicateNode(yymsp[-6].minor.yy0.strval, yymsp[-4].minor.yy0.strval, yymsp[-3].minor.yy92, yymsp[-2].minor.yy0.strval, yymsp[0].minor.yy0.strval); }
#line 1518 "grammar.c"
  yymsp[-6].minor.yy66 = yylhsminor.yy66;
        break;
      case 43: /* cond ::= UQSTRING DOT UQSTRING relation value */
#line 291 "grammar.y"
{ yylhsminor.yy66 = New_AST_ConstantPredicateNode(yymsp[-4].minor.yy0.strval, yymsp[-2].minor.yy0.strval, yymsp[-1].minor.yy92, yymsp[0].minor.yy38); }
#line 1524 "grammar.c"
  yymsp[-4].minor.yy66 = yylhsminor.yy66;
        break;
      case 44: /* cond ::= LEFT_PARENTHESIS cond RIGHT_PARENTHESIS */
#line 292 "grammar.y"
{ yymsp[-2].minor.yy66 = yymsp[-1].minor.yy66; }
#line 1530 "grammar.c"
        break;
      case 45: /* cond ::= cond AND cond */
#line 293 "grammar.y"
{ yylhsminor.yy66 = New_AST_ConditionNode(yymsp[-2].minor.yy66, AND, yymsp[0].minor.yy66); }
#line 1535 "grammar.c"
  yymsp[-2].minor.yy66 = yylhsminor.yy66;
        break;
      case 46: /* cond ::= cond OR cond */
#line 294 "grammar.y"
{ yylhsminor.yy66 = New_AST_ConditionNode(yymsp[-2].minor.yy66, OR, yymsp[0].minor.yy66); }
#line 1541 "grammar.c"
  yymsp[-2].minor.yy66 = yylhsminor.yy66;
        break;
      case 47: /* returnClause ::= RETURN returnElements */
#line 299 "grammar.y"
{
	yymsp[-1].minor.yy128 = New_AST_ReturnNode(yymsp[0].minor.yy146, 0);
}
#line 1549 "grammar.c"
        break;
      case 48: /* returnClause ::= RETURN DISTINCT returnElements */
#line 302 "grammar.y"
{
	yymsp[-2].minor.yy128 = New_AST_ReturnNode(yymsp[0].minor.yy146, 1);
}
#line 1556 "grammar.c"
        break;
      case 49: /* returnElements ::= returnElements COMMA returnElement */
#line 309 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy146, yymsp[0].minor.yy154);
	yylhsminor.yy146 = yymsp[-2].minor.yy146;
}
#line 1564 "grammar.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 50: /* returnElements ::= returnElement */
#line 314 "grammar.y"
{
	yylhsminor.yy146 = NewVector(AST_ReturnElementNode*, 1);
	Vector_Push(yylhsminor.yy146, yymsp[0].minor.yy154);
}
#line 1573 "grammar.c"
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 51: /* returnElement ::= arithmetic_expression */
#line 321 "grammar.y"
{
	yylhsminor.yy154 = New_AST_ReturnElementNode(yymsp[0].minor.yy114, NULL);
}
#line 1581 "grammar.c"
  yymsp[0].minor.yy154 = yylhsminor.yy154;
        break;
      case 52: /* returnElement ::= arithmetic_expression AS UQSTRING */
#line 326 "grammar.y"
{
	yylhsminor.yy154 = New_AST_ReturnElementNode(yymsp[-2].minor.yy114, yymsp[0].minor.yy0.strval);
}
#line 1589 "grammar.c"
  yymsp[-2].minor.yy154 = yylhsminor.yy154;
        break;
      case 53: /* arithmetic_expression ::= LEFT_PARENTHESIS arithmetic_expression RIGHT_PARENTHESIS */
#line 333 "grammar.y"
{
	yymsp[-2].minor.yy114 = yymsp[-1].minor.yy114;
}
#line 1597 "grammar.c"
        break;
      case 54: /* arithmetic_expression ::= arithmetic_expression ADD arithmetic_expression */
#line 345 "grammar.y"
{
	Vector *args = NewVector(AST_ArithmeticExpressionNode*, 2);
	Vector_Push(args, yymsp[-2].minor.yy114);
	Vector_Push(args, yymsp[0].minor.yy114);
	yylhsminor.yy114 = New_AST_AR_EXP_OpNode("ADD", args);
}
#line 1607 "grammar.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 55: /* arithmetic_expression ::= arithmetic_expression DASH arithmetic_expression */
#line 352 "grammar.y"
{
	Vector *args = NewVector(AST_ArithmeticExpressionNode*, 2);
	Vector_Push(args, yymsp[-2].minor.yy114);
	Vector_Push(args, yymsp[0].minor.yy114);
	yylhsminor.yy114 = New_AST_AR_EXP_OpNode("SUB", args);
}
#line 1618 "grammar.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 56: /* arithmetic_expression ::= arithmetic_expression MUL arithmetic_expression */
#line 359 "grammar.y"
{
	Vector *args = NewVector(AST_ArithmeticExpressionNode*, 2);
	Vector_Push(args, yymsp[-2].minor.yy114);
	Vector_Push(args, yymsp[0].minor.yy114);
	yylhsminor.yy114 = New_AST_AR_EXP_OpNode("MUL", args);
}
#line 1629 "grammar.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 57: /* arithmetic_expression ::= arithmetic_expression DIV arithmetic_expression */
#line 366 "grammar.y"
{
	Vector *args = NewVector(AST_ArithmeticExpressionNode*, 2);
	Vector_Push(args, yymsp[-2].minor.yy114);
	Vector_Push(args, yymsp[0].minor.yy114);
	yylhsminor.yy114 = New_AST_AR_EXP_OpNode("DIV", args);
}
#line 1640 "grammar.c"
  yymsp[-2].minor.yy114 = yylhsminor.yy114;
        break;
      case 58: /* arithmetic_expression ::= UQSTRING LEFT_PARENTHESIS arithmetic_expression_list RIGHT_PARENTHESIS */
#line 374 "grammar.y"
{
	yylhsminor.yy114 = New_AST_AR_EXP_OpNode(yymsp[-3].minor.yy0.strval, yymsp[-1].minor.yy146);
}
#line 1648 "grammar.c"
  yymsp[-3].minor.yy114 = yylhsminor.yy114;
        break;
      case 59: /* arithmetic_expression ::= value */
#line 379 "grammar.y"
{
	yylhsminor.yy114 = New_AST_AR_EXP_ConstOperandNode(yymsp[0].minor.yy38);
}
#line 1656 "grammar.c"
  yymsp[0].minor.yy114 = yylhsminor.yy114;
        break;
      case 60: /* arithmetic_expression ::= variable */
#line 384 "grammar.y"
{
	yylhsminor.yy114 = New_AST_AR_EXP_VariableOperandNode(yymsp[0].minor.yy20->alias, yymsp[0].minor.yy20->property);
}
#line 1664 "grammar.c"
  yymsp[0].minor.yy114 = yylhsminor.yy114;
        break;
      case 61: /* arithmetic_expression_list ::= arithmetic_expression_list COMMA arithmetic_expression */
#line 390 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy146, yymsp[0].minor.yy114);
	yylhsminor.yy146 = yymsp[-2].minor.yy146;
}
#line 1673 "grammar.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 62: /* arithmetic_expression_list ::= arithmetic_expression */
#line 394 "grammar.y"
{
	yylhsminor.yy146 = NewVector(AST_ArithmeticExpressionNode*, 1);
	Vector_Push(yylhsminor.yy146, yymsp[0].minor.yy114);
}
#line 1682 "grammar.c"
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 63: /* variable ::= UQSTRING */
#line 401 "grammar.y"
{
	yylhsminor.yy20 = New_AST_Variable(yymsp[0].minor.yy0.strval, NULL);
}
#line 1690 "grammar.c"
  yymsp[0].minor.yy20 = yylhsminor.yy20;
        break;
      case 64: /* variable ::= UQSTRING DOT UQSTRING */
#line 405 "grammar.y"
{
	yylhsminor.yy20 = New_AST_Variable(yymsp[-2].minor.yy0.strval, yymsp[0].minor.yy0.strval);
}
#line 1698 "grammar.c"
  yymsp[-2].minor.yy20 = yylhsminor.yy20;
        break;
      case 65: /* orderClause ::= */
#line 411 "grammar.y"
{
	yymsp[1].minor.yy68 = NULL;
}
#line 1706 "grammar.c"
        break;
      case 66: /* orderClause ::= ORDER BY columnNameList */
#line 414 "grammar.y"
{
	yymsp[-2].minor.yy68 = New_AST_OrderNode(yymsp[0].minor.yy146, ORDER_DIR_ASC);
}
#line 1713 "grammar.c"
        break;
      case 67: /* orderClause ::= ORDER BY columnNameList ASC */
#line 417 "grammar.y"
{
	yymsp[-3].minor.yy68 = New_AST_OrderNode(yymsp[-1].minor.yy146, ORDER_DIR_ASC);
}
#line 1720 "grammar.c"
        break;
      case 68: /* orderClause ::= ORDER BY columnNameList DESC */
#line 420 "grammar.y"
{
	yymsp[-3].minor.yy68 = New_AST_OrderNode(yymsp[-1].minor.yy146, ORDER_DIR_DESC);
}
#line 1727 "grammar.c"
        break;
      case 69: /* columnNameList ::= columnNameList COMMA columnName */
#line 425 "grammar.y"
{
	Vector_Push(yymsp[-2].minor.yy146, yymsp[0].minor.yy70);
	yylhsminor.yy146 = yymsp[-2].minor.yy146;
}
#line 1735 "grammar.c"
  yymsp[-2].minor.yy146 = yylhsminor.yy146;
        break;
      case 70: /* columnNameList ::= columnName */
#line 429 "grammar.y"
{
	yylhsminor.yy146 = NewVector(AST_ColumnNode*, 1);
	Vector_Push(yylhsminor.yy146, yymsp[0].minor.yy70);
}
#line 1744 "grammar.c"
  yymsp[0].minor.yy146 = yylhsminor.yy146;
        break;
      case 71: /* columnName ::= variable */
#line 435 "grammar.y"
{
	if(yymsp[0].minor.yy20->property != NULL) {
		yylhsminor.yy70 = AST_ColumnNodeFromVariable(yymsp[0].minor.yy20);
	} else {
		yylhsminor.yy70 = AST_ColumnNodeFromAlias(yymsp[0].minor.yy20->alias);
	}

	Free_AST_Variable(yymsp[0].minor.yy20);
}
#line 1758 "grammar.c"
  yymsp[0].minor.yy70 = yylhsminor.yy70;
        break;
      case 72: /* limitClause ::= */
#line 447 "grammar.y"
{
	yymsp[1].minor.yy47 = NULL;
}
#line 1766 "grammar.c"
        break;
      case 73: /* limitClause ::= LIMIT INTEGER */
#line 450 "grammar.y"
{
	yymsp[-1].minor.yy47 = New_AST_LimitNode(yymsp[0].minor.yy0.intval);
}
#line 1773 "grammar.c"
        break;
      case 74: /* relation ::= EQ */
#line 456 "grammar.y"
{ yymsp[0].minor.yy92 = EQ; }
#line 1778 "grammar.c"
        break;
      case 75: /* relation ::= GT */
#line 457 "grammar.y"
{ yymsp[0].minor.yy92 = GT; }
#line 1783 "grammar.c"
        break;
      case 76: /* relation ::= LT */
#line 458 "grammar.y"
{ yymsp[0].minor.yy92 = LT; }
#line 1788 "grammar.c"
        break;
      case 77: /* relation ::= LE */
#line 459 "grammar.y"
{ yymsp[0].minor.yy92 = LE; }
#line 1793 "grammar.c"
        break;
      case 78: /* relation ::= GE */
#line 460 "grammar.y"
{ yymsp[0].minor.yy92 = GE; }
#line 1798 "grammar.c"
        break;
      case 79: /* relation ::= NE */
#line 461 "grammar.y"
{ yymsp[0].minor.yy92 = NE; }
#line 1803 "grammar.c"
        break;
      case 80: /* value ::= INTEGER */
#line 472 "grammar.y"
{  yylhsminor.yy38 = SI_DoubleVal(yymsp[0].minor.yy0.intval); }
#line 1808 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 81: /* value ::= DASH INTEGER */
#line 473 "grammar.y"
{  yymsp[-1].minor.yy38 = SI_DoubleVal(-yymsp[0].minor.yy0.intval); }
#line 1814 "grammar.c"
        break;
      case 82: /* value ::= STRING */
#line 474 "grammar.y"
{  yylhsminor.yy38 = SI_StringVal(yymsp[0].minor.yy0.strval); }
#line 1819 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 83: /* value ::= FLOAT */
#line 475 "grammar.y"
{  yylhsminor.yy38 = SI_DoubleVal(yymsp[0].minor.yy0.dval); }
#line 1825 "grammar.c"
  yymsp[0].minor.yy38 = yylhsminor.yy38;
        break;
      case 84: /* value ::= DASH FLOAT */
#line 476 "grammar.y"
{  yymsp[-1].minor.yy38 = SI_DoubleVal(-yymsp[0].minor.yy0.dval); }
#line 1831 "grammar.c"
        break;
      case 85: /* value ::= TRUE */
#line 477 "grammar.y"
{ yymsp[0].minor.yy38 = SI_BoolVal(1); }
#line 1836 "grammar.c"
        break;
      case 86: /* value ::= FALSE */
#line 478 "grammar.y"
{ yymsp[0].minor.yy38 = SI_BoolVal(0); }
#line 1841 "grammar.c"
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
#line 23 "grammar.y"

	char buf[256];
	snprintf(buf, 256, "Syntax error at offset %d near '%s'\n", TOKEN.pos, TOKEN.s);

	ctx->ok = 0;
	ctx->errorMsg = strdup(buf);
#line 1906 "grammar.c"
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
#line 480 "grammar.y"


	/* Definitions of flex stuff */
	typedef struct yy_buffer_state *YY_BUFFER_STATE;
	int             yylex( void );
	YY_BUFFER_STATE yy_scan_string( const char * );
  	YY_BUFFER_STATE yy_scan_bytes( const char *, size_t );
  	extern int yylineno;
  	extern char *yytext;
	extern int yycolumn;

	AST_Query *Query_Parse(const char *q, size_t len, char **err) {
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
#line 2152 "grammar.c"
