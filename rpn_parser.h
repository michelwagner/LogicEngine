#pragma once
#include <stdint.h>
#include <stack>
#include "logic.h"


class CRPNLogicParser: public ILogicBlock
{
protected:
    CLogicInputData& mr_LogicInputData;
    std::stack<ILogicBlock*> m_ParserStack;
    std::stack<ILogicBlock*> m_ManagedLogicBlocks;
    ILogicBlock* p_RootBlock;

public:
    CRPNLogicParser(CLogicInputData& r_LogicInputData);
    ~CRPNLogicParser();

    void Parse(const char* pu8_Expression);
    virtual bool Evaluate() const override;

protected:
    void ClearManagedLogicBlocks();
    ILogicBlock* GetTopBlock();
    void StoreBlock(ILogicBlock* p_LogicBlock);
    void CreateLogicOrOperator();
    void CreateLogicAndOperator();
    void CreateLogicInput(char c_Symbol);
    uint32_t ConvertSymbolToChannel(char c_Symbol) const;
};