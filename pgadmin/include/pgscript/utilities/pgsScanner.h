//////////////////////////////////////////////////////////////////////////
//
// pgScript - PostgreSQL Tools
// RCS-ID:      $Id$
// Copyright (C) 2002 - 2010, The pgAdmin Development Team
// This software is released under the PostgreSQL Licence
//
//////////////////////////////////////////////////////////////////////////


#ifndef PGSSCANNER_H_
#define PGSSCANNER_H_

// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.

#ifndef YY_DECL

#define	YY_DECL						\
	pgscript::pgsParser::token_type				\
	pgscript::pgsScanner::lex(				\
	pgscript::pgsParser::semantic_type* yylval,		\
	pgscript::pgsParser::location_type* yylloc		\
	)
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer pgsFlexLexer
#include "pgscript/FlexLexer.h"
#undef  yyFlexLexer
#endif

#include "pgscript/parser.tab.hh"
#include <iostream>
#include <fstream>
#include <sstream>

namespace pgscript {

/** pgsScanner is a derived class to add some extra function to the scanner
 * class. Flex itself creates a class named yyFlexLexer, which is renamed using
 * macros to pgsFlexLexer. However we change the context of the generated
 * yylex() function to be contained within the pgsScanner class. This is required
 * because the yylex() defined in pgsFlexLexer has no parameters. */
class pgsScanner : public pgsFlexLexer
{
public:
	/** Create a new scanner object. The streams arg_yyin and arg_yyout default
	 * to cin and cout, but that assignment is only made when initializing in
	 * yylex(). */
	pgsScanner(wxMBConv & conv, std::istream * arg_yyin = 0,
			std::ostream * arg_yyout = 0);
	
	/** Required for virtual functions */
	virtual ~pgsScanner();
	
	/** This is the main lexing function. It is generated by flex according to
	 * the macro declaration YY_DECL above. The generated bison parser then
	 * calls this virtual function to fetch new tokens. */
	virtual pgsParser::token_type lex(pgsParser::semantic_type * yylval,
		pgsParser::location_type* yylloc);
	
	/** Enable debug output (via arg_yyout) if compiled into the scanner. */
	void set_debug(bool b);
	
private:
	
	/** Corrects column count because of multi-byte UTF8 characters. */
	int columns(const char & c);
	
	/** To count parenthesis. */
	int m_parent;
	
	std::string query, dollar, str;
	
	int comment_caller, string_caller;
	
	pgscript::pgsParser::token_type query_token;
	
	wxMBConv & m_conv;
	
};

} // namespace pgscript

#endif // PGSSCANNER_H_
