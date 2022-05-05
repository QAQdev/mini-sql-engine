/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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


#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 10 "minisql.y"
{
  pSyntaxNode syntax_node;
}
/* Line 1529 of yacc.c.  */
#line 145 "./minisql_yacc.h"
        YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

