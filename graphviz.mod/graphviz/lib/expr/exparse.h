/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_EX_EXPARSE_H_INCLUDED
# define YY_EX_EXPARSE_H_INCLUDED
/* Debug traces.  */
#ifndef EX_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define EX_DEBUG 1
#  else
#   define EX_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define EX_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined EX_DEBUG */
#if EX_DEBUG
extern int ex_debug;
#endif

/* Token type.  */
#ifndef EX_TOKENTYPE
# define EX_TOKENTYPE
  enum ex_tokentype
  {
    MINTOKEN = 258,
    INTEGER = 259,
    UNSIGNED = 260,
    CHARACTER = 261,
    FLOATING = 262,
    STRING = 263,
    VOIDTYPE = 264,
    ADDRESS = 265,
    ARRAY = 266,
    BREAK = 267,
    CALL = 268,
    CASE = 269,
    CONSTANT = 270,
    CONTINUE = 271,
    DECLARE = 272,
    DEFAULT = 273,
    DYNAMIC = 274,
    ELSE = 275,
    EXIT = 276,
    FOR = 277,
    FUNCTION = 278,
    GSUB = 279,
    ITERATE = 280,
    ITERATOR = 281,
    ID = 282,
    IF = 283,
    LABEL = 284,
    MEMBER = 285,
    NAME = 286,
    POS = 287,
    PRAGMA = 288,
    PRE = 289,
    PRINT = 290,
    PRINTF = 291,
    PROCEDURE = 292,
    QUERY = 293,
    RAND = 294,
    RETURN = 295,
    SCANF = 296,
    SPLIT = 297,
    SPRINTF = 298,
    SRAND = 299,
    SSCANF = 300,
    SUB = 301,
    SUBSTR = 302,
    SWITCH = 303,
    TOKENS = 304,
    UNSET = 305,
    WHILE = 306,
    F2I = 307,
    F2S = 308,
    I2F = 309,
    I2S = 310,
    S2B = 311,
    S2F = 312,
    S2I = 313,
    F2X = 314,
    I2X = 315,
    S2X = 316,
    X2F = 317,
    X2I = 318,
    X2S = 319,
    X2X = 320,
    XPRINT = 321,
    OR = 322,
    AND = 323,
    EQ = 324,
    NE = 325,
    LE = 326,
    GE = 327,
    LSH = 328,
    RSH = 329,
    IN_OP = 330,
    UNARY = 331,
    INC = 332,
    DEC = 333,
    CAST = 334,
    MAXTOKEN = 335
  };
#endif
/* Tokens.  */
#define MINTOKEN 258
#define INTEGER 259
#define UNSIGNED 260
#define CHARACTER 261
#define FLOATING 262
#define STRING 263
#define VOIDTYPE 264
#define ADDRESS 265
#define ARRAY 266
#define BREAK 267
#define CALL 268
#define CASE 269
#define CONSTANT 270
#define CONTINUE 271
#define DECLARE 272
#define DEFAULT 273
#define DYNAMIC 274
#define ELSE 275
#define EXIT 276
#define FOR 277
#define FUNCTION 278
#define GSUB 279
#define ITERATE 280
#define ITERATOR 281
#define ID 282
#define IF 283
#define LABEL 284
#define MEMBER 285
#define NAME 286
#define POS 287
#define PRAGMA 288
#define PRE 289
#define PRINT 290
#define PRINTF 291
#define PROCEDURE 292
#define QUERY 293
#define RAND 294
#define RETURN 295
#define SCANF 296
#define SPLIT 297
#define SPRINTF 298
#define SRAND 299
#define SSCANF 300
#define SUB 301
#define SUBSTR 302
#define SWITCH 303
#define TOKENS 304
#define UNSET 305
#define WHILE 306
#define F2I 307
#define F2S 308
#define I2F 309
#define I2S 310
#define S2B 311
#define S2F 312
#define S2I 313
#define F2X 314
#define I2X 315
#define S2X 316
#define X2F 317
#define X2I 318
#define X2S 319
#define X2X 320
#define XPRINT 321
#define OR 322
#define AND 323
#define EQ 324
#define NE 325
#define LE 326
#define GE 327
#define LSH 328
#define RSH 329
#define IN_OP 330
#define UNARY 331
#define INC 332
#define DEC 333
#define CAST 334
#define MAXTOKEN 335

/* Value type.  */
#if ! defined EX_STYPE && ! defined EX_STYPE_IS_DECLARED
union EX_STYPE
{
#line 41 "../../lib/expr/exparse.y"

	struct Exnode_s*expr;
	double		floating;
	struct Exref_s*	reference;
	struct Exid_s*	id;
	long long integer;
	int		op;
	char*		string;

#line 235 "exparse.h"

};
typedef union EX_STYPE EX_STYPE;
# define EX_STYPE_IS_TRIVIAL 1
# define EX_STYPE_IS_DECLARED 1
#endif


extern EX_STYPE ex_lval;

int ex_parse (void);

#endif /* !YY_EX_EXPARSE_H_INCLUDED  */
