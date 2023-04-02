#pragma once

class CInfixToPostfixExpression
{
public:
    CInfixToPostfixExpression() = default;

    void Parse(char const *&rpc_InfixExpression, char *&rpc_PostfixExpression)
    {
        char ca_OperatorStack[8u];
        uint8_t u8_OperatorStackIndex = 0u;

        while (*rpc_InfixExpression != 0)
        {
            const char c_Symbol = *rpc_InfixExpression;
            rpc_InfixExpression++;

            if (c_Symbol == '(')
            {
                Parse(rpc_InfixExpression, rpc_PostfixExpression);
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

protected:
    bool IsOperator(char c_Symbol)
    {
        return ((c_Symbol == '*') || (c_Symbol == '+') || (c_Symbol == '!'));
    }

    void Append(char c_Symbol, char *&rpc_Expression)
    {
        *rpc_Expression = c_Symbol;
        rpc_Expression++;
    }
};