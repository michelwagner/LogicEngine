#include "CInfixToPostfixExpression.h"


void CInfixToPostfixExpression::Parse(const char *pc_InfixExpression, char *pc_PostfixExpression)
{
    ParseRecursive(pc_InfixExpression, pc_PostfixExpression);
}


void CInfixToPostfixExpression::ParseRecursive(char const *&rpc_InfixExpression, char *&rpc_PostfixExpression)
{
    char ca_OperatorStack[8u];
    uint8_t u8_OperatorStackIndex = 0u;

    while (*rpc_InfixExpression != 0)
    {
        const char c_Symbol = *rpc_InfixExpression;
        rpc_InfixExpression++;

        if (c_Symbol == '(')
        {
            ParseRecursive(rpc_InfixExpression, rpc_PostfixExpression);
        }
        else if (c_Symbol == ')')
        {
            break;
        }
        else if (IsOperator(c_Symbol))
        {
            ca_OperatorStack[u8_OperatorStackIndex] = c_Symbol;
            u8_OperatorStackIndex++;
        }
        else
        {
            Append(c_Symbol, rpc_PostfixExpression);
        }
    }

    while (u8_OperatorStackIndex > 0u)
    {
        u8_OperatorStackIndex--;
        Append(ca_OperatorStack[u8_OperatorStackIndex], rpc_PostfixExpression);
    }
}


bool CInfixToPostfixExpression::IsOperator(char c_Symbol)
{
    return ((c_Symbol == '!') ||
        (c_Symbol == '*') || (c_Symbol == '+') || 
        (c_Symbol == '&') || (c_Symbol == '|'));
}


void CInfixToPostfixExpression::Append(char c_Symbol, char *&rpc_Expression)
{
    *rpc_Expression = c_Symbol;
    rpc_Expression++;
}
