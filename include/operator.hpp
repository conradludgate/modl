#pragma once

class Op
{
public:
	enum OpType {
	// Left Associated
	InNamespace,   // Left::Right
	PostIncrement, // Left++
	PostDecrement, // Left--
	FunctionCall,  // Left(Right)
	Parenthesis,   // (Left)
	ArrayIndex,    // Left[Right]
	InDot,         // Left.Right
	// Right Associated
	PreIncrement,   // ++Right
	PreDecrement,   // --Right
	PrePlus,        // +Right
	PreSubtract,    // -Right
	PreNot,         // !Right
	PreBitNot,      // ~Right
	TypeCast,       // (Left)Right
	PreDereference, // #Right
	PreReference,   // @Right
	// Left Associated
	InMultiply,      // Left*Right
	InDivide,        // Left/Right
	InModulo,        // Left%Right
	InAdd,           // Left+Right
	InSubtract,      // Left-Right
	InShiftLeft,     // Left<<Right
	InShiftRight,    // Left>>Right
	InLessThan,      // Left<Right
	InLessThanEq,    // Left<=Right
	InGreaterThan,   // Left>Right
	InGreaterThanEq, // Left>=Right
	InEqualTo,       // Left==Right
	InNotEqualTo,    // Left!=Right
	InBitAnd,        // Left&Right
	InBitXor,        // Left^Right
	InBitOr,         // Left|Right
	InAnd,           // Left&&Right
	InOr,            // Left||Right
	// Right Associated
	InAssign,           // Left=Right
	InAddAssign,        // Left+=Right
	InSubtractAssign,   // Left-=Right
	InMultiplyAssign,   // Left*=Right
	InDivideAssign,     // Left/=Right
	InModuloAssign,     // Left%=Right
	InShiftLeftAssign,  // Left<<=Right
	InShiftRightAssign, // Left>>=Right
	InBitAndAssign,     // Left&=Right
	InBitXorAssign,     // Left^=Right
	InBitOrAssign,      // Left|=Right
	InComma             // Left,Right
	};
};