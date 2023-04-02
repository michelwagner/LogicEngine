#include "LogicBlocks.h"


CLogicInputData::CLogicInputData()
    : mu32_Data{0u}
{
};


void CLogicInputData::Set(uint32_t u32_Data)
{
    mu32_Data = u32_Data;
};


bool CLogicInputData::Get(uint32_t u32_Channel)
{
    const uint32_t u32_ChannelMask = 1u << u32_Channel;
    return ((mu32_Data & u32_ChannelMask) != 0u);
};



CLogicInput::CLogicInput(ILogicInputData &r_LogicInputData, uint32_t u32_Channel)
    : mr_LogicInputData{r_LogicInputData},
      mu32_Channel{u32_Channel}
{    
};


bool CLogicInput::Evaluate() const
{
    return mr_LogicInputData.Get(mu32_Channel);
};



CLogicNotOperator::CLogicNotOperator(ILogicBlock &r_A)
    : mr_A{r_A}
{
};


bool CLogicNotOperator::Evaluate() const
{
    return (mr_A.Evaluate() == false);
};



CLogicAndOperator::CLogicAndOperator(ILogicBlock &r_A, ILogicBlock &r_B)
    : mr_A{r_A},
      mr_B{r_B}
{
};


bool CLogicAndOperator::Evaluate() const
{
    return (mr_A.Evaluate() && mr_B.Evaluate());
};


CLogicOrOperator::CLogicOrOperator(ILogicBlock &r_A, ILogicBlock &r_B)
    : mr_A{r_A},
      mr_B{r_B}
{
};


bool CLogicOrOperator::Evaluate() const
{
    return (mr_A.Evaluate() || mr_B.Evaluate());
};
