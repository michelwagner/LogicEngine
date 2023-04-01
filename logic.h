#pragma once

#include <stdint.h>


class ILogicExpression
{
public:
    virtual ~ILogicExpression() = default;
    virtual bool Evaluate() const = 0;
};


class ILogicInputData
{
public:
    virtual ~ILogicInputData() = default;
    virtual bool Get(uint32_t u32_Channel) = 0;
};


class CLogicInputData: public ILogicInputData
{
protected:
    uint32_t mu32_Data;

public:
    virtual ~CLogicInputData() = default;
    CLogicInputData()
    :mu32_Data{0u}
    {
    };
    
    void Set(uint32_t u32_Data)
    {
        mu32_Data = u32_Data;
    };

    virtual bool Get(uint32_t u32_Channel) override
    {
        const uint32_t u32_ChannelMask = 1u << u32_Channel;
        return ((mu32_Data & u32_ChannelMask) != 0u);
    };
};


class CLogicInput: public ILogicExpression
{
protected:
    ILogicInputData& mr_LogicInputData;
    uint32_t mu32_Channel;

public:
    virtual ~CLogicInput() = default;

    CLogicInput(ILogicInputData& r_LogicInputData, uint32_t u32_Channel)
    :mr_LogicInputData{r_LogicInputData},
    mu32_Channel{u32_Channel}
    {
    };

    virtual bool Evaluate() const override
    {
        return mr_LogicInputData.Get(mu32_Channel);
    };
};


class CLogicAndOperator: public ILogicExpression
{
protected:
    ILogicExpression& mr_A;
    ILogicExpression& mr_B;

public:
    virtual ~CLogicAndOperator() = default;

    CLogicAndOperator(ILogicExpression& r_A, ILogicExpression& r_B)
    :mr_A{r_A},
    mr_B{r_B}
    {
    };

    virtual bool Evaluate() const override
    {
        return mr_A.Evaluate() && mr_B.Evaluate();
    };
};


class CLogicOrOperator: public ILogicExpression
{
protected:
    ILogicExpression& mr_A;
    ILogicExpression& mr_B;

public:
    virtual ~CLogicOrOperator() = default;

    CLogicOrOperator(ILogicExpression& r_A, ILogicExpression& r_B)
    :mr_A{r_A},
    mr_B{r_B}
    {
    };

    virtual bool Evaluate() const override
    {
        return mr_A.Evaluate() || mr_B.Evaluate();
    };
};