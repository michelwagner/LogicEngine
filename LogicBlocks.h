#pragma once
#include <stdint.h>


class ILogicBlock
{
public:
    virtual ~ILogicBlock() = default;
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
    CLogicInputData();
    void Set(uint32_t u32_Data);
    virtual bool Get(uint32_t u32_Channel) override;
};



class CLogicInput: public ILogicBlock
{
protected:
    ILogicInputData& mr_LogicInputData;
    uint32_t mu32_Channel;

public:
    virtual ~CLogicInput() = default;
    CLogicInput(ILogicInputData& r_LogicInputData, uint32_t u32_Channel);
    virtual bool Evaluate() const override;
};



class CLogicNotOperator: public ILogicBlock
{
protected:
    ILogicBlock& mr_A;

public:
    virtual ~CLogicNotOperator() = default;
    CLogicNotOperator(ILogicBlock& r_A);
    virtual bool Evaluate() const override;
};



class CLogicAndOperator: public ILogicBlock
{
protected:
    ILogicBlock& mr_A;
    ILogicBlock& mr_B;

public:
    virtual ~CLogicAndOperator() = default;
    CLogicAndOperator(ILogicBlock& r_A, ILogicBlock& r_B);
    virtual bool Evaluate() const override;
};



class CLogicOrOperator: public ILogicBlock
{
protected:
    ILogicBlock& mr_A;
    ILogicBlock& mr_B;

public:
    virtual ~CLogicOrOperator() = default;
    CLogicOrOperator(ILogicBlock& r_A, ILogicBlock& r_B);
    virtual bool Evaluate() const override;
};