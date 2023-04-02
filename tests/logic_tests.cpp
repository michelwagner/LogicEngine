#include <CPostfixLogicParser.h>
#include <CInfixToPostfixExpression.h>
#include <gtest/gtest.h>

uint32_t ExpressionToInput(const char* pc_Expression)
{
    uint32_t u32_Value = 0u;

    for (uint32_t i = 0u; i < strlen(pc_Expression); i++)
    {
        const uint8_t u8_Symbol = std::toupper(pc_Expression[i]);
        const uint32_t u32_Channel = (static_cast<uint8_t>(u8_Symbol) - static_cast<uint8_t>('A'));
        const uint32_t u32_ChannelMask = 1u << u32_Channel;

        u32_Value |= u32_ChannelMask;
    }

    return u32_Value;
}


TEST(logic_test, LogicInput)
{
    CLogicInputData LogicInputData;
    CLogicInput LogicInput0(LogicInputData, 0u);
    CLogicInput LogicInput1(LogicInputData, 1u);
    CLogicInput LogicInput31(LogicInputData, 31u);

    LogicInputData.Set(0b00000000'00000000'00000000'00000000u);
    EXPECT_EQ(LogicInput0.Evaluate(), false);
    EXPECT_EQ(LogicInput1.Evaluate(), false);
    EXPECT_EQ(LogicInput31.Evaluate(), false);

    LogicInputData.Set(0b00000000'00000000'00000000'00000001u);
    EXPECT_EQ(LogicInput0.Evaluate(), true);
    EXPECT_EQ(LogicInput1.Evaluate(), false);
    EXPECT_EQ(LogicInput31.Evaluate(), false);

    LogicInputData.Set(0b10000000'00000000'00000000'00000000u);
    EXPECT_EQ(LogicInput0.Evaluate(), false);
    EXPECT_EQ(LogicInput1.Evaluate(), false);
    EXPECT_EQ(LogicInput31.Evaluate(), true);

    LogicInputData.Set(0b10000000'00000000'00000000'00000001u);
    EXPECT_EQ(LogicInput0.Evaluate(), true);
    EXPECT_EQ(LogicInput1.Evaluate(), false);
    EXPECT_EQ(LogicInput31.Evaluate(), true);
}


TEST(logic_test, CLogicNotOperator)
{
    CLogicInputData LogicInputData;
    LogicInputData.Set(0b00000000'00000000'00000000'00000010u);
    CLogicInput LogicInput0_L(LogicInputData, 0u);
    CLogicInput LogicInput1_H(LogicInputData, 1u);

    CLogicNotOperator LogicNotOperator1_L(LogicInput0_L);
    CLogicNotOperator LogicNotOperator2_H(LogicInput1_H);

    EXPECT_EQ(LogicNotOperator1_L.Evaluate(), true);
    EXPECT_EQ(LogicNotOperator2_H.Evaluate(), false);
}


TEST(logic_test, CLogicAndOperator)
{
    CLogicInputData LogicInputData;
    LogicInputData.Set(0b00000000'00000000'00000000'00000011u);
    CLogicInput LogicInput0_H(LogicInputData, 0u);
    CLogicInput LogicInput1_H(LogicInputData, 1u);
    CLogicInput LogicInput2_L(LogicInputData, 2u);

    CLogicAndOperator LogicAndOperator01_HH(LogicInput0_H, LogicInput1_H);
    CLogicAndOperator LogicAndOperator02_HL(LogicInput0_H, LogicInput2_L);

    EXPECT_EQ(LogicAndOperator01_HH.Evaluate(), true);
    EXPECT_EQ(LogicAndOperator02_HL.Evaluate(), false);
}


TEST(logic_test, CLogicOROperator)
{
    CLogicInputData LogicInputData;
    LogicInputData.Set(0b00000000'00000000'00000000'00000010u);
    CLogicInput LogicInput0_L(LogicInputData, 0u);
    CLogicInput LogicInput1_H(LogicInputData, 1u);
    CLogicInput LogicInput2_L(LogicInputData, 2u);

    CLogicOrOperator LogicAndOperator01_LH(LogicInput0_L, LogicInput1_H);
    CLogicOrOperator LogicAndOperator02_LL(LogicInput0_L, LogicInput2_L);

    EXPECT_EQ(LogicAndOperator01_LH.Evaluate(), true);
    EXPECT_EQ(LogicAndOperator02_LL.Evaluate(), false);
}


TEST(logic_test, PostfixLogicParser_NoExpression)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("");

    LogicInputData.Set(0b11111111'11111111'11111111'11111111u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);
}


TEST(logic_test, PostfixLogicParserAnd)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("AB&");

    LogicInputData.Set(ExpressionToInput(""));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(ExpressionToInput("A"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(ExpressionToInput("AB"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);
}


TEST(logic_test, PostfixLogicParserOr)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("AB|");

    LogicInputData.Set(ExpressionToInput(""));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(ExpressionToInput("A"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(ExpressionToInput("AB"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);
}


TEST(logic_test, PostfixLogicParserOrAndOr)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("AB|CD|&");

    LogicInputData.Set(ExpressionToInput(""));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(ExpressionToInput("AB"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(ExpressionToInput("AD"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);
}


TEST(logic_test, PostfixLogicParserNotOrAndOr)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("AB|CD|&!");

    LogicInputData.Set(ExpressionToInput(""));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(ExpressionToInput("AB"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(ExpressionToInput("AD"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);
}


TEST(logic_test, PostfixLogicParserWhitespacesLowerCase)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("Ab|\tCd| & !! !");

    LogicInputData.Set(ExpressionToInput(""));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(ExpressionToInput("AB"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(ExpressionToInput("AD"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);
}


TEST(logic_test, ExpressionParser01)
{
    CInfixToPostfixExpression InfixToPostfixExpression;

    char ac_PostfixExpression[32] = { 0u };
    const char ac_InfixExpression[] = "A|B|C|D";
    InfixToPostfixExpression.Parse(&ac_InfixExpression[0u], &ac_PostfixExpression[0u]);

    EXPECT_EQ(strncmp(&ac_PostfixExpression[0u], "ABCD|||", sizeof(ac_PostfixExpression)), 0);
}


TEST(logic_test, ExpressionParser01a)
{
    CInfixToPostfixExpression InfixToPostfixExpression;

    char ac_PostfixExpression[32u] = { 0u };
    const char ac_InfixExpression[] = "A&B&C&D";
    InfixToPostfixExpression.Parse(&ac_InfixExpression[0u], &ac_PostfixExpression[0u]);

    EXPECT_EQ(strncmp(&ac_PostfixExpression[0u], "ABCD&&&", sizeof(ac_PostfixExpression)), 0);
}


TEST(logic_test, ExpressionParser02)
{
    CInfixToPostfixExpression InfixToPostfixExpression;

    char ac_PostfixExpression[32u] = { 0u };
    const char ac_InfixExpression[] = "(A|B)&(C|D)";
    InfixToPostfixExpression.Parse(&ac_InfixExpression[0u], &ac_PostfixExpression[0u]);

    EXPECT_EQ(strncmp(&ac_PostfixExpression[0u], "AB|CD|&", sizeof(ac_PostfixExpression)), 0);
}


TEST(logic_test, ExpressionParser03)
{
    CInfixToPostfixExpression InfixToPostfixExpression;

    char ac_PostfixExpression[32u] = { 0u };
    const char ac_InfixExpression[] = "(A&B)|(C&D)|(A&(B&(C&D)))";
    InfixToPostfixExpression.Parse(&ac_InfixExpression[0u], &ac_PostfixExpression[0u]);

    EXPECT_EQ(strncmp(&ac_PostfixExpression[0u], "AB&CD&ABCD&&&||", sizeof(ac_PostfixExpression)), 0);
}


TEST(logic_test, ExpressionParser04)
{
    CInfixToPostfixExpression InfixToPostfixExpression;

    char ac_PostfixExpression[32u] = { 0u };
    const char ac_InfixExpression[] = "((A|B)&(C|D))|((A&B)|(C&D))";
    InfixToPostfixExpression.Parse(&ac_InfixExpression[0u], &ac_PostfixExpression[0u]);

    EXPECT_EQ(strncmp(&ac_PostfixExpression[0u], "AB|CD|&AB&CD&||", sizeof(ac_PostfixExpression)), 0);
}


TEST(logic_test, ExpressionParser05)
{
    CInfixToPostfixExpression InfixToPostfixExpression;

    char ac_PostfixExpression[32u] = { 0u };
    const char ac_InfixExpression[] = "A|(B&(!(C|D|E)))|(!F)";
    InfixToPostfixExpression.Parse(&ac_InfixExpression[0u], &ac_PostfixExpression[0u]);

    EXPECT_EQ(strncmp(&ac_PostfixExpression[0u], "ABCDE||!&F!||", sizeof(ac_PostfixExpression)), 0);
}


TEST(logic_test, ExpressionParser06)
{
    CInfixToPostfixExpression InfixToPostfixExpression;

    char ac_PostfixExpression[32u] = { 0u };
    const char ac_InfixExpression[] = "(!A)&(!B)";
    InfixToPostfixExpression.Parse(&ac_InfixExpression[0u], &ac_PostfixExpression[0u]);

    EXPECT_EQ(strncmp(&ac_PostfixExpression[0u], "A!B!&", sizeof(ac_PostfixExpression)), 0);
}


TEST(logic_test, Integration01)
{
    CInfixToPostfixExpression InfixToPostfixExpression;
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    char ac_PostfixExpression[32u] = { 0u };
    const char ac_InfixExpression[] = "!((A|B) & (C|D))";
    InfixToPostfixExpression.Parse(&ac_InfixExpression[0u], &ac_PostfixExpression[0u]);
    PostfixLogicParser.Parse(&ac_PostfixExpression[0u]);

    LogicInputData.Set(ExpressionToInput(""));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(ExpressionToInput("AB"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(ExpressionToInput("BC"));
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
