#pragma once
#include <stdint.h>
#include <stack>
#include <vector>
#include "logic.h"


class CRPNLogicParser: public ILogicBlock
{
protected:
    CLogicInputData& mr_LogicInputData;
    std::stack<ILogicBlock*> m_ParserStack;
    std::vector<ILogicBlock*> m_LogicBlocks;
    ILogicBlock* p_RootBlock;
    uint32_t mu32_NumberOfLogicBlocks;

public:
    CRPNLogicParser(CLogicInputData& r_LogicInputData);
    ~CRPNLogicParser();

    void Parse(const char* pu8_Expression);
    virtual bool Evaluate() const override;

protected:
    void ClearLogicBlocks();
    ILogicBlock* GetTopBlock();
    void StoreBlock(ILogicBlock* p_LogicBlock);
    void CreateLogicOrOperator();
    void CreateLogicAndOperator();
    void CreateLogicInput(char c_Symbol);
};