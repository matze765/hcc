
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
     identifier = 258,
     IF = 259,
     ELSE = 260,
     RETURN = 261,
     WHILE = 262,
     DO = 263,
     CONST = 264,
     VOID = 265,
     PLUSPLUS = 266,
     MINUSMINUS = 267,
     NOTEQUAL = 268,
     LESSEQUAL = 269,
     GREATEREQUAL = 270,
     GREATER = 271,
     LESS = 272,
     SHIFTL = 273,
     SHIFTR = 274,
     ORL = 275,
     ANDL = 276,
     EQUALL = 277,
     NOTL = 278,
     PLUS = 279,
     MINUS = 280,
     TIMES = 281,
     DIV = 282,
     MOD = 283,
     OPENBR = 284,
     CLOSEBR = 285,
     COPENBR = 286,
     CCLOSEBR = 287,
     COMMA = 288,
     EQUAL = 289,
     SEMICOLON = 290,
     INT = 291,
     FLOAT = 292,
     NAME = 293,
     COMMENT = 294,
     MCOMMENT = 295,
     INTT = 296,
     FLOATT = 297
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 42 "hcc.y"

	int ival; 
	float fval;
	char* sval;
	symtabEntryType type;



/* Line 1676 of yacc.c  */
#line 103 "hcc.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


