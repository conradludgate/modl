#pragma once

#include <string>
#include "token.hpp"

class Atom
{
public:
	static std::vector<Token> Atomise(std::istream& is);

	inline static std::vector<std::string> keywords = {
		"module", "include", "namespace", "interface", "struct",
		"return", "break", "if", "else", "for", "while", "switch", "case",
		"int", "int8", "int16", "int32", "int64",
		"uint", "uint8", "uint16", "uint32", "uint64",
		"float32", "float64", "byte", "bool",
		"auto", "void", "string", "public"
	};
	inline static std::string operators = "!#&()*+,-./:;<=>?@[\\]^_{|}~%";

	enum AtomType
	{
		Module,    // 0
		Include,   // 1
		Namespace, // 2
		Interface, // 3
		Struct,    // 4
		Return,    // 5
		Break,     // 6
		If,        // 7
		Else,      // 8
		For,       // 9
		While,     // 10
		Switch,    // 11
		Case,      // 12
		Int,       // 13
		Int8,      // 14
		Int16,     // 15
		Int32,     // 16
		Int64,     // 17
		UInt,      // 18
		UInt_8,    // 19
		UInt_16,   // 20
		UInt_32,   // 21
		UInt_64,   // 22
		Bool,      // 23
		Byte_8,    // 24
		Float_32,  // 25
		Float_64,  // 26
		Auto,      // 27
		Void,      // 28
		String,    // 29
		Public,    // 30
		
		Exclamation,  // 31 !
		Hash,         // 32 #
		Ampersand,    // 33 &
		LParen,       // 34 (
		RParen,       // 35 )
		Star,         // 36 *
		Plus,         // 37 +
		Comma,        // 38 ,
		Minus,        // 39 -
		Dot,          // 40 .
		ForwardSlash, // 41 /
		Colon,        // 42 :
		Semicolon,    // 43 ;
		Less,         // 44 <
		Equal,        // 45 =
		Greater,      // 46 >
		Question,     // 47 ?
		At,           // 48 @
		LBracket,     // 49 [
		Backslash,    // 50 \
		RBracket,     // 51 ]
		Carat,        // 52 ^
		Underscore,   // 53 _
		LBrace,       // 54 {
		Vertical,     // 55 |
		RBrace,       // 56 }
		Tild,         // 57 ~
		Percent       // 58 %
	};
};

