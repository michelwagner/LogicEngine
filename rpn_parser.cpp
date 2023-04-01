#include "rpn_parser.h"

CRPNLogicParser::CRPNLogicParser(CLogicInputData &r_LogicInputData)
    : mr_LogicInputData{r_LogicInputData},
      m_ParserStack{},
      m_ManagedLogicBlocks{},
      p_RootBlock{nullptr}
{
}


CRPNLogicParser::~CRPNLogicParser()
{
    ClearManagedLogicBlocks();
    p_RootBlock = nullptr;
}


void CRPNLogicParser::Parse(const char *pu8_Expression)
{
    ClearManagedLogicBlocks();

    uint32_t i = 0;
    while (pu8_Expression[i] != 0u)
    {
        const char c_Symbol = pu8_Expression[i];

        switch (c_Symbol)
        {
        case '!':
            CreateLogicNotOperator();
            break;

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

    if (m_ParserStack.empty() == false)
    {
        p_RootBlock = m_ParserStack.top();
    }
}


bool CRPNLogicParser::Evaluate() const
{
    return p_RootBlock != nullptr ? p_RootBlock->Evaluate() : false;
};


void CRPNLogicParser::ClearManagedLogicBlocks()
{
    while (m_ManagedLogicBlocks.empty() == false)
    {
        delete m_ManagedLogicBlocks.top();
        m_ManagedLogicBlocks.pop();
    }
}


ILogicBlock *CRPNLogicParser::GetTopBlock()
{
    ILogicBlock *p_Block = m_ParserStack.top();
    m_ParserStack.pop();
    return p_Block;
}


void CRPNLogicParser::StoreBlock(ILogicBlock *p_LogicBlock)
{
    m_ManagedLogicBlocks.push(p_LogicBlock);
    m_ParserStack.push(p_LogicBlock);
}


void CRPNLogicParser::CreateLogicNotOperator()
{
    StoreBlock(new CLogicNotOperator(*GetTopBlock()));
}


void CRPNLogicParser::CreateLogicOrOperator()
{
    StoreBlock(new CLogicOrOperator(*GetTopBlock(), *GetTopBlock()));
}


void CRPNLogicParser::CreateLogicAndOperator()
{
    StoreBlock(new CLogicAndOperator(*GetTopBlock(), *GetTopBlock()));
}


void CRPNLogicParser::CreateLogicInput(char c_Symbol)
{
    const uint32_t u32_Channel = ConvertSymbolToChannel(c_Symbol);
    StoreBlock(new CLogicInput(mr_LogicInputData, u32_Channel));
}


uint32_t CRPNLogicParser::ConvertSymbolToChannel(char c_Symbol) const
{
    const uint8_t u8_LowerCase = static_cast<uint8_t>(c_Symbol) | 0x20u;
    const uint32_t u32_Channel = (u8_LowerCase - static_cast<uint8_t>('a'));

    return u32_Channel;
}