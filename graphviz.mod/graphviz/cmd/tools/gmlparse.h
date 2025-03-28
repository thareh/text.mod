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

#ifndef YY_GML_GMLPARSE_H_INCLUDED
# define YY_GML_GMLPARSE_H_INCLUDED
/* Debug traces.  */
#ifndef GMLDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define GMLDEBUG 1
#  else
#   define GMLDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define GMLDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined GMLDEBUG */
#if GMLDEBUG
extern int gmldebug;
#endif

/* Token type.  */
#ifndef GMLTOKENTYPE
# define GMLTOKENTYPE
  enum gmltokentype
  {
    GRAPH = 258,
    NODE = 259,
    EDGE = 260,
    DIRECTED = 261,
    SOURCE = 262,
    TARGET = 263,
    XVAL = 264,
    YVAL = 265,
    WVAL = 266,
    HVAL = 267,
    LABEL = 268,
    GRAPHICS = 269,
    LABELGRAPHICS = 270,
    TYPE = 271,
    FILL = 272,
    OUTLINE = 273,
    OUTLINESTYLE = 274,
    OUTLINEWIDTH = 275,
    WIDTH = 276,
    STYLE = 277,
    LINE = 278,
    POINT = 279,
    TEXT = 280,
    FONTSIZE = 281,
    FONTNAME = 282,
    COLOR = 283,
    INTEGER = 284,
    REAL = 285,
    STRING = 286,
    ID = 287,
    NAME = 288,
    LIST = 289
  };
#endif
/* Tokens.  */
#define GRAPH 258
#define NODE 259
#define EDGE 260
#define DIRECTED 261
#define SOURCE 262
#define TARGET 263
#define XVAL 264
#define YVAL 265
#define WVAL 266
#define HVAL 267
#define LABEL 268
#define GRAPHICS 269
#define LABELGRAPHICS 270
#define TYPE 271
#define FILL 272
#define OUTLINE 273
#define OUTLINESTYLE 274
#define OUTLINEWIDTH 275
#define WIDTH 276
#define STYLE 277
#define LINE 278
#define POINT 279
#define TEXT 280
#define FONTSIZE 281
#define FONTNAME 282
#define COLOR 283
#define INTEGER 284
#define REAL 285
#define STRING 286
#define ID 287
#define NAME 288
#define LIST 289

/* Value type.  */
#if ! defined GMLSTYPE && ! defined GMLSTYPE_IS_DECLARED
union GMLSTYPE
{
#line 237 "../../cmd/tools/gmlparse.y"

    int i;
    char *str;
    gmlnode* np;
    gmledge* ep;
    gmlattr* ap;
    Dt_t*    list;

#line 142 "gmlparse.h"

};
typedef union GMLSTYPE GMLSTYPE;
# define GMLSTYPE_IS_TRIVIAL 1
# define GMLSTYPE_IS_DECLARED 1
#endif


extern GMLSTYPE gmllval;

int gmlparse (void);

#endif /* !YY_GML_GMLPARSE_H_INCLUDED  */
