#include "CPostfixLogicParser.h"
#include <cctype>

CPostfixLogicParser::CPostfixLogicParser(CLogicInputData &r_LogicInputData)
    : mr_LogicInputData{r_LogicInputData},
      m_ParserStack{},
      m_ManagedLogicBlocks{},
      p_RootBlock{nullptr}
{
}


CPostfixLogicParser::~CPostfixLogicParser()
{
    ClearManagedLogicBlocks();
    p_RootBlock = nullptr;
}


void CPostfixLogicParser::Parse(const char *pu8_Expression)
{
    ClearManagedLogicBlocks();

    uint32_t i = 0;
    while (pu8_Expression[i] != 0u)
    {
        const uint8_t u8_Symbol = std::toupper(pu8_Expression[i]);

        switch (u8_Symbol)
        {
        case '!':
            CreateLogicNotOperator();
            break;

        case '+':
        case '|':
            CreateLogicOrOperator();
            break;

        case '*':
        case '&':
            CreateLogicAndOperator();
            break;

        case ' ':
        case '\t':
        break;

        default:
            if (IsInputSymbolValid(u8_Symbol))
            {
                CreateLogicInput(u8_Symbol);
            }
            break;
        }

        i++;
    }

    if (m_ParserStack.empty() == false)
    {
        p_RootBlock = m_ParserStack.top();
    }
}


bool CPostfixLogicParser::IsInputSymbolValid(uint8_t u8_InputSymbol)
{
    return ((u8_InputSymbol >= 'A') && (u8_InputSymbol <= 'Z'));
}


bool CPostfixLogicParser::Evaluate() const
{
    return p_RootBlock != nullptr ? p_RootBlock->Evaluate() : false;
};


void CPostfixLogicParser::ClearManagedLogicBlocks()
{
    while (m_ManagedLogicBlocks.empty() == false)
    {
        delete m_ManagedLogicBlocks.top();
        m_ManagedLogicBlocks.pop();
    }
}


ILogicBlock *CPostfixLogicParser::GetTopBlock()
{
    ILogicBlock *p_Block = m_ParserStack.top();
    m_ParserStack.pop();
    return p_Block;
}


void CPostfixLogicParser::StoreBlock(ILogicBlock *p_LogicBlock)
{
    m_ManagedLogicBlocks.push(p_LogicBlock);
    m_ParserStack.push(p_LogicBlock);
}


void CPostfixLogicParser::CreateLogicNotOperator()
{
    StoreBlock(new CLogicNotOperator(*GetTopBlock()));
}


void CPostfixLogicParser::CreateLogicOrOperator()
{
    StoreBlock(new CLogicOrOperator(*GetTopBlock(), *GetTopBlock()));
}


void CPostfixLogicParser::CreateLogicAndOperator()
{
    StoreBlock(new CLogicAndOperator(*GetTopBlock(), *GetTopBlock()));
}


void CPostfixLogicParser::CreateLogicInput(char c_Symbol)
{
    const uint32_t u32_Channel = ConvertSymbolToChannel(c_Symbol);
    StoreBlock(new CLogicInput(mr_LogicInputData, u32_Channel));
}


uint32_t CPostfixLogicParser::ConvertSymbolToChannel(char c_Symbol) const
{
    const uint32_t u32_Channel = (static_cast<uint8_t>(c_Symbol) - static_cast<uint8_t>('A'));
    return u32_Channel;
}