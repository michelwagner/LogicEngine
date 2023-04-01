#pragma once

#include <stdint.h>
#include <stack>
#include "logic.h"


class CRPNLogicParser: public ILogicExpression
{
protected:
    CLogicInputData& mr_LogicInputData;
    std::stack<ILogicExpression*> m_Stack;

public:
    CRPNLogicParser(CLogicInputData& r_LogicInputData)
    :mr_LogicInputData{r_LogicInputData},
    m_Stack{}
    {}

    void Parse(const char* pu8_Expression)
    {
        uint32_t i = 0;
        m_Stack.push(nullptr);
        
        while (pu8_Expression[i] != 0u)
        {
            const char c_Symbol = pu8_Expression[i];

            switch (c_Symbol)
            {
            case '+':
                CreateLogicOrOperator();
                break;

            case '*':
                CreateLogicAndOperator();
                break;

            default:
                CreateLogicInput(c_Symbol);
                break;
            }

            i++;
        }
    }

    virtual bool Evaluate() const override
    {
        ILogicExpression const* p_Expression = m_Stack.top();
        return p_Expression != nullptr ? p_Expression->Evaluate() : false;
    };

protected:
    void CreateLogicOrOperator()
    {
        ILogicExpression* p_B = m_Stack.top();
        m_Stack.pop();
        ILogicExpression* p_A = m_Stack.top();
        m_Stack.pop();

        CLogicOrOperator* p_LogicOperator = new CLogicOrOperator(*p_A, *p_B);
        m_Stack.push(p_LogicOperator);
    }

    void CreateLogicAndOperator()
    {
        ILogicExpression* p_B = m_Stack.top();
        m_Stack.pop();
        ILogicExpression* p_A = m_Stack.top();
        m_Stack.pop();

        CLogicAndOperator* p_LogicOperator = new CLogicAndOperator(*p_A, *p_B);
        m_Stack.push(p_LogicOperator);
    }

    void CreateLogicInput(char c_Symbol)
    {
        const uint8_t u8_LowerCase = static_cast<uint8_t>(c_Symbol) | 0x20u;
        const uint32_t u32_Channel = (u8_LowerCase  - static_cast<uint8_t>('a'));

        CLogicInput* p_LogicInput = new CLogicInput(mr_LogicInputData, u32_Channel);
        m_Stack.push(p_LogicInput);
    }
};