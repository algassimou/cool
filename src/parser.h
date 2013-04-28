
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TYPE = 258,
     ID = 259,
     FDN = 260,
     PLUS = 261,
     MOINS = 262,
     MULT = 263,
     DIV = 264,
     COLON = 265,
     PERIOD = 266,
     LPAREN = 267,
     RPAREN = 268,
     COMMA = 269,
     LBRACKET = 270,
     RBRACKET = 271,
     EQUAL = 272,
     SEMI = 273,
     LBRACE = 274,
     RBRACE = 275,
     CLASS = 276,
     CASE = 277,
     ELSE = 278,
     ESAC = 279,
     FALSE = 280,
     FI = 281,
     IF = 282,
     IN = 283,
     INHERITS = 284,
     ISVOID = 285,
     LET = 286,
     LOOP = 287,
     NEW = 288,
     NOT = 289,
     OF = 290,
     POOL = 291,
     THEN = 292,
     TRUE = 293,
     WHILE = 294
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 7 "/home/algassimou/Programmation/compilation/cool/src/parser.y"

  char *id ;
  int ival;



/* Line 1676 of yacc.c  */
#line 98 "/home/algassimou/Programmation/compilation/cool/src/parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


