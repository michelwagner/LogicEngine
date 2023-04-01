#include "rpn_parser.h"

CRPNLogicParser::CRPNLogicParser(CLogicInputData &r_LogicInputData)
    : mr_LogicInputData{r_LogicInputData},
      m_ParserStack{},
      m_LogicBlocks{},
      p_RootBlock{nullptr},
      mu32_NumberOfLogicBlocks{0u}
{
}

CRPNLogicParser::~CRPNLogicParser()
{
    ClearLogicBlocks();
}

void CRPNLogicParser::Parse(const char *pu8_Expression)
{
    ClearLogicBlocks();
    m_ParserStack.push(nullptr);

    uint32_t i = 0;
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

    p_RootBlock = m_ParserStack.top();
}

bool CRPNLogicParser::Evaluate() const
{
    return p_RootBlock != nullptr ? p_RootBlock->Evaluate() : false;
};

void CRPNLogicParser::ClearLogicBlocks()
{
    for (uint32_t i = 0u; i < mu32_NumberOfLogicBlocks; i++)
    {
        delete m_LogicBlocks[i];
    }
    mu32_NumberOfLogicBlocks = 0;
}

ILogicBlock *CRPNLogicParser::GetTopBlock()
{
    ILogicBlock *p_Block = m_ParserStack.top();
    m_ParserStack.pop();
    return p_Block;
}

void CRPNLogicParser::StoreBlock(ILogicBlock *p_LogicBlock)
{
    m_LogicBlocks[mu32_NumberOfLogicBlocks++] = p_LogicBlock;
    m_ParserStack.push(p_LogicBlock);
}

void CRPNLogicParser::CreateLogicOrOperator()
{
    ILogicBlock *p_LogicBlock = new CLogicOrOperator(*GetTopBlock(), *GetTopBlock());
    StoreBlock(p_LogicBlock);
}

void CRPNLogicParser::CreateLogicAndOperator()
{
    ILogicBlock *p_LogicBlock = new CLogicAndOperator(*GetTopBlock(), *GetTopBlock());
    StoreBlock(p_LogicBlock);
}

void CRPNLogicParser::CreateLogicInput(char c_Symbol)
{
    const uint8_t u8_LowerCase = static_cast<uint8_t>(c_Symbol) | 0x20u;
    const uint32_t u32_Channel = (u8_LowerCase - static_cast<uint8_t>('a'));

    ILogicBlock *p_LogicBlock = new CLogicInput(mr_LogicInputData, u32_Channel);
    StoreBlock(p_LogicBlock);
}
