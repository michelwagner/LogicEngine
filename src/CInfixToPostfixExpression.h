#pragma once
#include <stdint.h>


class CInfixToPostfixExpression
{
public:
    CInfixToPostfixExpression() = default;
    void Parse(const char *pc_InfixExpression, char *pc_PostfixExpression);

protected:
    void ParseRecursive(char const *&rpc_InfixExpression, char *&rpc_PostfixExpression);
    bool IsOperator(char c_Symbol);
    void Append(char c_Symbol, char *&rpc_Expression);
};