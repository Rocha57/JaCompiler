/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CBRACE = 258,
    CCURV = 259,
    CSQUARE = 260,
    OBRACE = 261,
    OCURV = 262,
    OSQUARE = 263,
    AND = 264,
    ASSIGN = 265,
    BOOL = 266,
    CLASS = 267,
    COMMA = 268,
    DIV = 269,
    DO = 270,
    DOTLENGTH = 271,
    DOUBLE = 272,
    ELSE = 273,
    EQ = 274,
    GEQ = 275,
    GT = 276,
    IF = 277,
    INT = 278,
    LEQ = 279,
    LT = 280,
    MINUS = 281,
    MOD = 282,
    NEQ = 283,
    NOT = 284,
    OR = 285,
    RETURN = 286,
    PARSEINT = 287,
    PLUS = 288,
    PRINT = 289,
    PUBLIC = 290,
    SEMI = 291,
    STAR = 292,
    STATIC = 293,
    STRING = 294,
    VOID = 295,
    WHILE = 296,
    RESERVED = 297,
    ID = 298,
    BOOLLIT = 299,
    REALLIT = 300,
    DECLIT = 301,
    STRLIT = 302,
    IFX = 303
  };
#endif
/* Tokens.  */
#define CBRACE 258
#define CCURV 259
#define CSQUARE 260
#define OBRACE 261
#define OCURV 262
#define OSQUARE 263
#define AND 264
#define ASSIGN 265
#define BOOL 266
#define CLASS 267
#define COMMA 268
#define DIV 269
#define DO 270
#define DOTLENGTH 271
#define DOUBLE 272
#define ELSE 273
#define EQ 274
#define GEQ 275
#define GT 276
#define IF 277
#define INT 278
#define LEQ 279
#define LT 280
#define MINUS 281
#define MOD 282
#define NEQ 283
#define NOT 284
#define OR 285
#define RETURN 286
#define PARSEINT 287
#define PLUS 288
#define PRINT 289
#define PUBLIC 290
#define SEMI 291
#define STAR 292
#define STATIC 293
#define STRING 294
#define VOID 295
#define WHILE 296
#define RESERVED 297
#define ID 298
#define BOOLLIT 299
#define REALLIT 300
#define DECLIT 301
#define STRLIT 302
#define IFX 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 19 "jac.y" /* yacc.c:1915  */

    struct _info *yylval; /*token*/
    struct _Node* node;


#line 156 "y.tab.h" /* yacc.c:1915  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
