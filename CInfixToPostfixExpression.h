#pragma once

class CInfixToPostfixExpression
{
public:
    CInfixToPostfixExpression() = default;

    void Parse(char const *&src, char *&rpn)
    {
        char ops[8u];
        uint8_t n = 0u;

        while (*src != 0)
        {
            const char s = *src;
            src++;

            if (s == '(')
            {
                Parse(src, rpn);
            }
            else if (s == ')')
            {
                break;
            }
            else if (IsOperator(s))
            {
                ops[n] = s;
                n++;
            }
            else
            {
                Append(s, rpn);
            }
        }

        while (n > 0u)
        {
            n--;
            Append(ops[n], rpn);
        }
    }

protected:
    bool IsOperator(char Symbol)
    {
        return ((Symbol == '*') || (Symbol == '+') || (Symbol == '!'));
    }

    void Append(char Symbol, char *&rpn)
    {
        *rpn = Symbol;
        rpn++;
    }
};