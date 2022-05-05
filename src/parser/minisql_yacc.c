/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/* Put the tokens into the symbol table, so that GDB and other debuggers
   know about them.  */
enum yytokentype {
  CREATE = 258,
  DROP = 259,
  SELECT = 260,
  INSERT = 261,
  DELETE = 262,
  UPDATE = 263,
  TRXBEGIN = 264,
  TRXCOMMIT = 265,
  TRXROLLBACK = 266,
  QUIT = 267,
  EXECFILE = 268,
  SHOW = 269,
  USE = 270,
  USING = 271,
  DATABASE = 272,
  DATABASES = 273,
  TABLE = 274,
  TABLES = 275,
  INDEX = 276,
  INDEXES = 277,
  ON = 278,
  FROM = 279,
  WHERE = 280,
  INTO = 281,
  SET = 282,
  VALUES = 283,
  PRIMARY = 284,
  KEY = 285,
  UNIQUE = 286,
  CHAR = 287,
  INT = 288,
  FLOAT = 289,
  AND = 290,
  OR = 291,
  NOT = 292,
  IS = 293,
  FLAGNULL = 294,
  IDENTIFIER = 295,
  STRING = 296,
  NUMBER = 297,
  EQ = 298,
  NE = 299,
  LE = 300,
  GE = 301
};
#endif
/* Tokens.  */
#define CREATE 258
#define DROP 259
#define SELECT 260
#define INSERT 261
#define DELETE 262
#define UPDATE 263
#define TRXBEGIN 264
#define TRXCOMMIT 265
#define TRXROLLBACK 266
#define QUIT 267
#define EXECFILE 268
#define SHOW 269
#define USE 270
#define USING 271
#define DATABASE 272
#define DATABASES 273
#define TABLE 274
#define TABLES 275
#define INDEX 276
#define INDEXES 277
#define ON 278
#define FROM 279
#define WHERE 280
#define INTO 281
#define SET 282
#define VALUES 283
#define PRIMARY 284
#define KEY 285
#define UNIQUE 286
#define CHAR 287
#define INT 288
#define FLOAT 289
#define AND 290
#define OR 291
#define NOT 292
#define IS 293
#define FLAGNULL 294
#define IDENTIFIER 295
#define STRING 296
#define NUMBER 297
#define EQ 298
#define NE 299
#define LE 300
#define GE 301




/* Copy the first part of user declarations.  */
#line 1 "minisql.y"

#include <stdio.h>
#include "parser/parser.h"

extern char *yytext;

extern int yylex(void);

int yyerror(char *error);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 10 "minisql.y"
{
  pSyntaxNode syntax_node;
}
/* Line 193 of yacc.c.  */
#line 201 "./minisql_yacc.c"
        YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 214 "./minisql_yacc.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if !defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
/* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
 /* The OS might guarantee only one guard page at the bottom of the stack,
    and a page size can be as small as 4096 bytes.  So we cannot safely
    invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
    to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && !defined _STDLIB_H \
       && !((defined YYMALLOC || defined malloc) \
 && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if !defined malloc && !defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)

void *malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if !defined free && !defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)

void free(void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (!defined yyoverflow \
     && (!defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc {
  yytype_int16 yyss;
  YYSTYPE yyvs;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
  {					\
    YYSIZE_T yyi;				\
    for (yyi = 0; yyi < (Count); yyi++)	\
      (To)[yyi] = (From)[yyi];		\
  }					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)          \
    do                  \
      {                  \
  YYSIZE_T yynewbytes;            \
  YYCOPY (&yyptr->Stack, Stack, yysize);        \
  Stack = &yyptr->Stack;            \
  yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
  yyptr += yynewbytes / sizeof (*yyptr);        \
      }                  \
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  53
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   106

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNRULES -- Number of states.  */
#define YYNSTATES  134

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)            \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
        {
                0, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                48, 49, 51, 2, 50, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 47,
                52, 2, 53, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 1, 2, 3, 4,
                5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
                35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
                45, 46
        };

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    48,    52,    55,    58,    61,    68,    72,
      74,    78,    80,    86,    90,    93,    95,    97,   102,   106,
     115,   126,   130,   133,   138,   145,   147,   149,   153,   155,
     157,   159,   163,   165,   167,   169,   171,   173,   175,   177,
     179,   181,   183,   185,   193,   197,   199,   203,   209,   214,
     221,   225,   227,   231,   233,   235,   237,   239
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      55,     0,    -1,    56,    47,    -1,    57,    -1,    58,    -1,
      59,    -1,    60,    -1,    61,    -1,    62,    -1,    67,    -1,
      68,    -1,    69,    -1,    70,    -1,    71,    -1,    78,    -1,
      80,    -1,    81,    -1,    84,    -1,    85,    -1,    86,    -1,
      87,    -1,    88,    -1,     3,    17,    40,    -1,     4,    17,
      40,    -1,    14,    18,    -1,    15,    40,    -1,    14,    20,
      -1,     3,    19,    40,    48,    64,    49,    -1,    40,    50,
      63,    -1,    40,    -1,    65,    50,    64,    -1,    65,    -1,
      29,    30,    48,    63,    49,    -1,    40,    66,    31,    -1,
      40,    66,    -1,    33,    -1,    34,    -1,    32,    48,    42,
      49,    -1,     4,    19,    40,    -1,     3,    21,    40,    23,
      40,    48,    63,    49,    -1,     3,    21,    40,    23,    40,
      48,    63,    49,    16,    40,    -1,     4,    21,    40,    -1,
      14,    22,    -1,     5,    72,    24,    40,    -1,     5,    72,
      24,    40,    25,    73,    -1,    51,    -1,    63,    -1,    73,
      74,    75,    -1,    75,    -1,    35,    -1,    36,    -1,    40,
      77,    76,    -1,    41,    -1,    42,    -1,    39,    -1,    43,
      -1,    44,    -1,    45,    -1,    46,    -1,    52,    -1,    53,
      -1,    38,    -1,    37,    -1,     6,    26,    40,    28,    48,
      79,    49,    -1,    76,    50,    79,    -1,    76,    -1,     7,
      24,    40,    -1,     7,    24,    40,    25,    73,    -1,     8,
      40,    27,    82,    -1,     8,    40,    27,    82,    25,    73,
      -1,    83,    50,    82,    -1,    83,    -1,    40,    43,    76,
      -1,     9,    -1,    10,    -1,    11,    -1,    12,    -1,    13,
      41,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    35,    35,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    64,    71,    78,    84,    91,    97,   107,   111,
     117,   121,   124,   131,   136,   144,   147,   150,   157,   164,
     172,   186,   193,   199,   204,   215,   218,   225,   230,   236,
     239,   245,   253,   256,   259,   265,   268,   271,   274,   277,
     280,   283,   286,   292,   302,   306,   312,   316,   326,   333,
     348,   352,   358,   366,   372,   378,   384,   390
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CREATE", "DROP", "SELECT", "INSERT",
  "DELETE", "UPDATE", "TRXBEGIN", "TRXCOMMIT", "TRXROLLBACK", "QUIT",
  "EXECFILE", "SHOW", "USE", "USING", "DATABASE", "DATABASES", "TABLE",
  "TABLES", "INDEX", "INDEXES", "ON", "FROM", "WHERE", "INTO", "SET",
  "VALUES", "PRIMARY", "KEY", "UNIQUE", "CHAR", "INT", "FLOAT", "AND",
  "OR", "NOT", "IS", "FLAGNULL", "IDENTIFIER", "STRING", "NUMBER", "EQ",
  "NE", "LE", "GE", "';'", "'('", "')'", "','", "'*'", "'<'", "'>'",
  "$accept", "start", "sql", "sql_create_database", "sql_drop_database",
  "sql_show_databases", "sql_use_database", "sql_show_tables",
  "sql_create_table", "column_list", "column_definition_list",
  "column_definition", "column_type", "sql_drop_table", "sql_create_index",
  "sql_drop_index", "sql_show_indexes", "sql_select", "select_columns",
  "where_conditions", "connector", "where_condition", "column_value",
  "operator", "sql_insert", "column_values", "sql_delete", "sql_update",
  "update_values", "update_value", "sql_trx_begin", "sql_trx_commit",
  "sql_trx_rollback", "sql_quit", "sql_exec_file", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,    59,    40,    41,
      44,    42,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
        {
                0, 54, 55, 56, 56, 56, 56, 56, 56, 56,
                56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
                56, 56, 57, 58, 59, 60, 61, 62, 63, 63,
                64, 64, 64, 65, 65, 66, 66, 66, 67, 68,
                68, 69, 70, 71, 71, 72, 72, 73, 73, 74,
                74, 75, 76, 76, 76, 77, 77, 77, 77, 77,
                77, 77, 77, 78, 79, 79, 80, 80, 81, 81,
                82, 82, 83, 84, 85, 86, 87, 88
        };

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
        {
                0, 2, 2, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 3, 3, 2, 2, 2, 6, 3, 1,
                3, 1, 5, 3, 2, 1, 1, 4, 3, 8,
                10, 3, 2, 4, 6, 1, 1, 3, 1, 1,
                1, 3, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 7, 3, 1, 3, 5, 4, 6,
                3, 1, 3, 1, 1, 1, 1, 2
        };

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
        {
                0, 0, 0, 0, 0, 0, 0, 73, 74, 75,
                76, 0, 0, 0, 0, 0, 3, 4, 5, 6,
                7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                17, 18, 19, 20, 21, 0, 0, 0, 0, 0,
                0, 29, 45, 46, 0, 0, 0, 0, 77, 24,
                26, 42, 25, 1, 2, 22, 0, 0, 23, 38,
                41, 0, 0, 0, 66, 0, 0, 0, 28, 43,
                0, 0, 0, 68, 71, 0, 0, 0, 31, 0,
                0, 0, 0, 67, 48, 0, 0, 0, 0, 0,
                35, 36, 34, 27, 0, 0, 44, 54, 52, 53,
                65, 0, 62, 61, 55, 56, 57, 58, 59, 60,
                0, 49, 50, 0, 72, 69, 70, 0, 0, 33,
                30, 0, 0, 63, 51, 47, 0, 0, 39, 64,
                32, 37, 0, 40
        };

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
        {
                -1, 14, 15, 16, 17, 18, 19, 20, 21, 43,
                77, 78, 92, 22, 23, 24, 25, 26, 44, 83,
                113, 84, 100, 110, 27, 101, 28, 29, 73, 74,
                30, 31, 32, 33, 34
        };

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -85
static const yytype_int8 yypact[] =
        {
                32, 2, 3, -36, -19, 8, -7, -85, -85, -85,
                -85, 10, 7, 12, 53, 11, -85, -85, -85, -85,
                -85, -85, -85, -85, -85, -85, -85, -85, -85, -85,
                -85, -85, -85, -85, -85, 14, 15, 17, 19, 20,
                21, 13, -85, -85, 38, 24, 25, 39, -85, -85,
                -85, -85, -85, -85, -85, -85, 22, 44, -85, -85,
                -85, 28, 29, 43, 47, 33, -24, 34, -85, 50,
                30, 36, 37, 52, 31, 49, 16, 35, 40, 41,
                36, -11, -35, -22, -85, -11, 36, 33, 45, 46,
                -85, -85, 51, -85, -24, 28, -22, -85, -85, -85,
                42, 48, -85, -85, -85, -85, -85, -85, -85, -85,
                -11, -85, -85, 36, -85, -22, -85, 28, 54, -85,
                -85, 55, -11, -85, -85, -85, 56, 57, 67, -85,
                -85, -85, 58, -85
        };

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
        {
                -85, -85, -85, -85, -85, -85, -85, -85, -85, -61,
                -9, -85, -85, -85, -85, -85, -85, -85, -85, -74,
                -85, -27, -84, -85, -85, -34, -85, -85, 0, -85,
                -85, -85, -85, -85, -85
        };

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
        {
                68, 114, 102, 103, 41, 75, 96, 45, 104, 105,
                106, 107, 115, 111, 112, 42, 76, 108, 109, 35,
                38, 36, 39, 37, 40, 49, 124, 50, 97, 51,
                98, 99, 46, 47, 121, 1, 2, 3, 4, 5,
                6, 7, 8, 9, 10, 11, 12, 13, 89, 90,
                91, 48, 52, 53, 55, 56, 126, 57, 54, 58,
                59, 60, 62, 61, 63, 64, 65, 67, 41, 69,
                66, 70, 71, 72, 79, 80, 82, 86, 81, 88,
                85, 87, 119, 132, 93, 120, 125, 116, 129, 95,
                94, 0, 122, 117, 118, 0, 127, 123, 133, 0,
                0, 0, 0, 0, 128, 130, 131
        };

static const yytype_int8 yycheck[] =
        {
                61, 85, 37, 38, 40, 29, 80, 26, 43, 44,
                45, 46, 86, 35, 36, 51, 40, 52, 53, 17,
                17, 19, 19, 21, 21, 18, 110, 20, 39, 22,
                41, 42, 24, 40, 95, 3, 4, 5, 6, 7,
                8, 9, 10, 11, 12, 13, 14, 15, 32, 33,
                34, 41, 40, 0, 40, 40, 117, 40, 47, 40,
                40, 40, 24, 50, 40, 40, 27, 23, 40, 40,
                48, 28, 25, 40, 40, 25, 40, 25, 48, 30,
                43, 50, 31, 16, 49, 94, 113, 87, 122, 48,
                50, -1, 50, 48, 48, -1, 42, 49, 40, -1,
                -1, -1, -1, -1, 49, 49, 49
        };

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
        {
                0, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                12, 13, 14, 15, 55, 56, 57, 58, 59, 60,
                61, 62, 67, 68, 69, 70, 71, 78, 80, 81,
                84, 85, 86, 87, 88, 17, 19, 21, 17, 19,
                21, 40, 51, 63, 72, 26, 24, 40, 41, 18,
                20, 22, 40, 0, 47, 40, 40, 40, 40, 40,
                40, 50, 24, 40, 40, 27, 48, 23, 63, 40,
                28, 25, 40, 82, 83, 29, 40, 64, 65, 40,
                25, 48, 40, 73, 75, 43, 25, 50, 30, 32,
                33, 34, 66, 49, 50, 48, 73, 39, 41, 42,
                76, 79, 37, 38, 43, 44, 45, 46, 52, 53,
                77, 35, 36, 74, 76, 73, 82, 48, 48, 31,
                64, 63, 50, 49, 76, 75, 63, 42, 49, 79,
                49, 49, 16, 40
        };

#define yyerrok    (yyerrstatus = 0)
#define yyclearin  (yychar = YYEMPTY)
#define YYEMPTY    (-2)
#define YYEOF    0

#define YYACCEPT  goto yyacceptlab
#define YYABORT    goto yyabortlab
#define YYERROR    goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL    goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)          \
do                \
  if (yychar == YYEMPTY && yylen == 1)        \
    {                \
      yychar = (Token);            \
      yylval = (Value);            \
      yytoken = YYTRANSLATE (yychar);        \
      YYPOPSTACK (1);            \
      goto yybackup;            \
    }                \
  else                \
    {                \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;              \
    }                \
while (YYID (0))


#define YYTERROR  1
#define YYERRCODE  256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)        \
    do                  \
      if (YYID (N))                                                    \
  {                \
    (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;  \
    (Current).first_column = YYRHSLOC (Rhs, 1).first_column;  \
    (Current).last_line    = YYRHSLOC (Rhs, N).last_line;    \
    (Current).last_column  = YYRHSLOC (Rhs, N).last_column;  \
  }                \
      else                \
  {                \
    (Current).first_line   = (Current).last_line   =    \
      YYRHSLOC (Rhs, 0).last_line;        \
    (Current).first_column = (Current).last_column =    \
      YYRHSLOC (Rhs, 0).last_column;        \
  }                \
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
        (Loc).first_line, (Loc).first_column,	\
        (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
      Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
  break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
       yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
           &(yyvsp[(yyi + 1) - (yynrhs)])
                      );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef  YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
  switch (*++yyp)
    {
    case '\'':
    case ',':
      goto do_not_strip_quotes;

    case '\\':
      if (*++yyp != '\\')
        goto do_not_strip_quotes;
      /* Fall through.  */
    default:
      if (yyres)
        yyres[yyn] = *yyp;
      yyn++;
      break;

    case '"':
      if (yyres)
        yyres[yyn] = '\0';
      return yyn;
    }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
   constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
        + sizeof yyexpecting - 1
        + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
           * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
   YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
  if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
    {
      if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
        {
    yycount = 1;
    yysize = yysize0;
    yyformat[sizeof yyunexpected - 1] = '\0';
    break;
        }
      yyarg[yycount++] = yytname[yyx];
      yysize1 = yysize + yytnamerr (0, yytname[yyx]);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;
      yyfmt = yystpcpy (yyfmt, yyprefix);
      yyprefix = yyor;
    }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
  return YYSIZE_MAXIMUM;

      if (yyresult)
  {
    /* Avoid sprintf, as that infringes on the user's name space.
       Don't have undefined behavior even if the translation
       produced a string with the wrong number of "%s"s.  */
    char *yyp = yyresult;
    int yyi = 0;
    while ((*yyp = *yyf) != '\0')
      {
        if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
    {
      yyp += yytnamerr (yyp, yyarg[yyi++]);
      yyf += 2;
    }
        else
    {
      yyp++;
      yyf++;
    }
      }
  }
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)

static void
yydestruct(const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype) {

    default:
      break;
  }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus

int yyparse(void);

#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)

int
yyparse(void)
#else
int
yyparse ()

#endif
#endif
{

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;


#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;    /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
  yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

  yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp) {
    /* Get the current used size of the three stacks, in elements.  */
    YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
    {
/* Give user a chance to reallocate the stack.  Use copies of
   these so that the &'s don't force the real ones into
   memory.  */
YYSTYPE *yyvs1 = yyvs;
yytype_int16 *yyss1 = yyss;


/* Each stack pointer address is followed by the size of the
   data in use in that stack, in bytes.  This used to be a
   conditional around just the two extra args, but that might
   be undefined if yyoverflow is a macro.  */
yyoverflow (YY_("memory exhausted"),
      &yyss1, yysize * sizeof (*yyssp),
      &yyvs1, yysize * sizeof (*yyvsp),

      &yystacksize);

yyss = yyss1;
yyvs = yyvs1;
    }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
    goto yyexhaustedlab;
# else
    /* Extend the stack our own way.  */
    if (YYMAXDEPTH <= yystacksize)
      goto yyexhaustedlab;
    yystacksize *= 2;
    if (YYMAXDEPTH < yystacksize)
      yystacksize = YYMAXDEPTH;

    {
      yytype_int16 *yyss1 = yyss;
      union yyalloc *yyptr =
              (union yyalloc *) YYSTACK_ALLOC(YYSTACK_BYTES (yystacksize));
      if (!yyptr)
        goto yyexhaustedlab;
      YYSTACK_RELOCATE (yyss);
      YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
      if (yyss1 != yyssa)
        YYSTACK_FREE(yyss1);
    }
# endif
#endif /* no yyoverflow */

    yyssp = yyss + yysize - 1;
    yyvsp = yyvs + yysize - 1;


    YYDPRINTF ((stderr, "Stack size increased to %lu\n",
            (unsigned long int) yystacksize));

    if (yyss + yystacksize - 1 <= yyssp)
      YYABORT;
  }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
  yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY) {
    YYDPRINTF ((stderr, "Reading a token: "));
    yychar = YYLEX;
  }

  if (yychar <= YYEOF) {
    yychar = yytoken = YYEOF;
    YYDPRINTF ((stderr, "Now at end of input.\n"));
  } else {
    yytoken = YYTRANSLATE (yychar);
    YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
  }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0) {
    if (yyn == 0 || yyn == YYTABLE_NINF)
      goto yyerrlab;
    yyn = -yyn;
    goto yyreduce;
  }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
  yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
  yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1 - yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn) {
    case 2:
#line 35 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (2)].syntax_node);
      MinisqlParserSetRoot((yyval.syntax_node));
    }
      break;

    case 3:
#line 42 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 4:
#line 43 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 5:
#line 44 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 6:
#line 45 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 7:
#line 46 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 8:
#line 47 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 9:
#line 48 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 10:
#line 49 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 11:
#line 50 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 12:
#line 51 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 13:
#line 52 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 14:
#line 53 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 15:
#line 54 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 16:
#line 55 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 17:
#line 56 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 18:
#line 57 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 19:
#line 58 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 20:
#line 59 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 21:
#line 60 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 22:
#line 64 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCreateDB, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 23:
#line 71 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeDropDB, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 24:
#line 78 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeShowDB, NULL);
    }
      break;

    case 25:
#line 84 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeUseDB, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(2) - (2)].syntax_node));
    }
      break;

    case 26:
#line 91 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeShowTables, NULL);
    }
      break;

    case 27:
#line 97 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCreateTable, NULL);
      pSyntaxNode list_node = CreateSyntaxNode(kNodeColumnDefinitionList, NULL);
      SyntaxNodeAddChildren(list_node, (yyvsp[(5) - (6)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (6)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), list_node);
    }
      break;

    case 28:
#line 107 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (3)].syntax_node);
      SyntaxNodeAddSibling((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 29:
#line 111 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 30:
#line 117 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (3)].syntax_node);
      SyntaxNodeAddSibling((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 31:
#line 121 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 32:
#line 124 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeColumnList, "primary keys");
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(4) - (5)].syntax_node));
    }
      break;

    case 33:
#line 131 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeColumnDefinition, "unique");
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(1) - (3)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(2) - (3)].syntax_node));
    }
      break;

    case 34:
#line 136 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeColumnDefinition, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(1) - (2)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(2) - (2)].syntax_node));
    }
      break;

    case 35:
#line 144 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeColumnType, "int");
    }
      break;

    case 36:
#line 147 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeColumnType, "float");
    }
      break;

    case 37:
#line 150 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeColumnType, "char");
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (4)].syntax_node));
    }
      break;

    case 38:
#line 157 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeDropTable, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 39:
#line 164 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCreateIndex, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (8)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(5) - (8)].syntax_node));
      pSyntaxNode index_keys_node = CreateSyntaxNode(kNodeColumnList, "index keys");
      SyntaxNodeAddChildren(index_keys_node, (yyvsp[(7) - (8)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), index_keys_node);
    }
      break;

    case 40:
#line 172 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCreateIndex, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (10)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(5) - (10)].syntax_node));
      pSyntaxNode index_keys_node = CreateSyntaxNode(kNodeColumnList, "index keys");
      SyntaxNodeAddChildren(index_keys_node, (yyvsp[(7) - (10)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), index_keys_node);
      pSyntaxNode index_type_node = CreateSyntaxNode(kNodeIndexType, "index type");
      SyntaxNodeAddChildren(index_type_node, (yyvsp[(10) - (10)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), index_type_node);
    }
      break;

    case 41:
#line 186 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeDropIndex, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 42:
#line 193 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeShowIndexes, NULL);
    }
      break;

    case 43:
#line 199 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeSelect, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(2) - (4)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(4) - (4)].syntax_node));
    }
      break;

    case 44:
#line 204 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeSelect, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(2) - (6)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(4) - (6)].syntax_node));
      pSyntaxNode condition_node = CreateSyntaxNode(kNodeConditions, NULL);
      SyntaxNodeAddChildren(condition_node, (yyvsp[(6) - (6)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), condition_node);
    }
      break;

    case 45:
#line 215 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeAllColumns, NULL);
    }
      break;

    case 46:
#line 218 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeColumnList, "select columns");
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(1) - (1)].syntax_node));
    }
      break;

    case 47:
#line 225 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(2) - (3)].syntax_node);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(1) - (3)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 48:
#line 230 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 49:
#line 236 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeConnector, "and");
    }
      break;

    case 50:
#line 239 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeConnector, "or");
    }
      break;

    case 51:
#line 245 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(2) - (3)].syntax_node);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(1) - (3)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 52:
#line 253 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 53:
#line 256 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 54:
#line 259 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeNull, NULL);
    }
      break;

    case 55:
#line 265 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCompareOperator, "=");
    }
      break;

    case 56:
#line 268 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCompareOperator, "<>");
    }
      break;

    case 57:
#line 271 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCompareOperator, "<=");
    }
      break;

    case 58:
#line 274 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCompareOperator, ">=");
    }
      break;

    case 59:
#line 277 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCompareOperator, "<");
    }
      break;

    case 60:
#line 280 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCompareOperator, ">");
    }
      break;

    case 61:
#line 283 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCompareOperator, "is");
    }
      break;

    case 62:
#line 286 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeCompareOperator, "not");
    }
      break;

    case 63:
#line 292 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeInsert, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (7)].syntax_node));
      pSyntaxNode col_val_node = CreateSyntaxNode(kNodeColumnValues, NULL);
      SyntaxNodeAddChildren(col_val_node, (yyvsp[(6) - (7)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), col_val_node);
    }
      break;

    case 64:
#line 302 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (3)].syntax_node);
      SyntaxNodeAddSibling((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 65:
#line 306 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 66:
#line 312 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeDelete, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 67:
#line 316 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeDelete, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (5)].syntax_node));
      pSyntaxNode condition_node = CreateSyntaxNode(kNodeConditions, NULL);
      SyntaxNodeAddChildren(condition_node, (yyvsp[(5) - (5)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), condition_node);
    }
      break;

    case 68:
#line 326 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeUpdate, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(2) - (4)].syntax_node));
      pSyntaxNode upd_values_node = CreateSyntaxNode(kNodeUpdateValues, NULL);
      SyntaxNodeAddChildren(upd_values_node, (yyvsp[(4) - (4)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), upd_values_node);
    }
      break;

    case 69:
#line 333 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeUpdate, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(2) - (6)].syntax_node));
      // update values
      pSyntaxNode upd_values_node = CreateSyntaxNode(kNodeUpdateValues, NULL);
      SyntaxNodeAddChildren(upd_values_node, (yyvsp[(4) - (6)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), upd_values_node);
      // where conditions
      pSyntaxNode condition_node = CreateSyntaxNode(kNodeConditions, NULL);
      SyntaxNodeAddChildren(condition_node, (yyvsp[(6) - (6)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), condition_node);
    }
      break;

    case 70:
#line 348 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (3)].syntax_node);
      SyntaxNodeAddSibling((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 71:
#line 352 "minisql.y"
    {
      (yyval.syntax_node) = (yyvsp[(1) - (1)].syntax_node);
    }
      break;

    case 72:
#line 358 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeUpdateValue, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(1) - (3)].syntax_node));
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(3) - (3)].syntax_node));
    }
      break;

    case 73:
#line 366 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeTrxBegin, NULL);
    }
      break;

    case 74:
#line 372 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeTrxCommit, NULL);
    }
      break;

    case 75:
#line 378 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeTrxRollback, NULL);
    }
      break;

    case 76:
#line 384 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeQuit, NULL);
    }
      break;

    case 77:
#line 390 "minisql.y"
    {
      (yyval.syntax_node) = CreateSyntaxNode(kNodeExecFile, NULL);
      SyntaxNodeAddChildren((yyval.syntax_node), (yyvsp[(2) - (2)].syntax_node));
    }
      break;


/* Line 1267 of yacc.c.  */
#line 2098 "./minisql_yacc.c"
    default:
      break;
  }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
  yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus) {
    ++yynerrs;
#if !YYERROR_VERBOSE
    yyerror(YY_("syntax error"));
#else
    {
YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
  {
    YYSIZE_T yyalloc = 2 * yysize;
    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
      yyalloc = YYSTACK_ALLOC_MAXIMUM;
    if (yymsg != yymsgbuf)
      YYSTACK_FREE (yymsg);
    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
    if (yymsg)
      yymsg_alloc = yyalloc;
    else
      {
  yymsg = yymsgbuf;
  yymsg_alloc = sizeof yymsgbuf;
      }
  }

if (0 < yysize && yysize <= yymsg_alloc)
  {
    (void) yysyntax_error (yymsg, yystate, yychar);
    yyerror (yymsg);
  }
else
  {
    yyerror (YY_("syntax error"));
    if (yysize != 0)
      goto yyexhaustedlab;
  }
    }
#endif
  }


  if (yyerrstatus == 3) {
    /* If just tried and failed to reuse look-ahead token after an
 error, discard it.  */

    if (yychar <= YYEOF) {
      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        YYABORT;
    } else {
      yydestruct("Error: discarding",
                 yytoken, &yylval);
      yychar = YYEMPTY;
    }
  }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
  yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
    goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
  yyerrlab1:
  yyerrstatus = 3;  /* Each real token shifted decrements this.  */

  for (;;) {
    yyn = yypact[yystate];
    if (yyn != YYPACT_NINF) {
      yyn += YYTERROR;
      if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR) {
        yyn = yytable[yyn];
        if (0 < yyn)
          break;
      }
    }

    /* Pop the current state because it cannot handle the error token.  */
    if (yyssp == yyss)
      YYABORT;


    yydestruct("Error: popping",
               yystos[yystate], yyvsp);
    YYPOPSTACK (1);
    yystate = *yyssp;
    YY_STACK_PRINT (yyss, yyssp);
  }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
  yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
  yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
  yyexhaustedlab:
  yyerror(YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

  yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
    yydestruct("Cleanup: discarding lookahead",
               yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss) {
    yydestruct("Cleanup: popping",
               yystos[*yyssp], yyvsp);
    YYPOPSTACK (1);
  }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 396 "minisql.y"

int yyerror(char *error) {
  MinisqlParserSetError(error);
  return 0;
}
