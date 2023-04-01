#include <gtest/gtest.h>
#include "CPostfixLogicParser.h"
#include "CInfixToPostfixExpression.h"


TEST(logic_test, LogicInput)
{
    CLogicInputData LogicInputData;
    CLogicInput LogicInput0(LogicInputData, 0u);
    CLogicInput LogicInput1(LogicInputData, 1u);
    CLogicInput LogicInput31(LogicInputData, 31u);

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(LogicInput0.Evaluate(), false);
    EXPECT_EQ(LogicInput1.Evaluate(), false);
    EXPECT_EQ(LogicInput31.Evaluate(), false);

    LogicInputData.Set(0x00000001u);
    EXPECT_EQ(LogicInput0.Evaluate(), true);
    EXPECT_EQ(LogicInput1.Evaluate(), false);
    EXPECT_EQ(LogicInput31.Evaluate(), false);

    LogicInputData.Set(0x80000000u);
    EXPECT_EQ(LogicInput0.Evaluate(), false);
    EXPECT_EQ(LogicInput1.Evaluate(), false);
    EXPECT_EQ(LogicInput31.Evaluate(), true);

    LogicInputData.Set(0x80000001u);
    EXPECT_EQ(LogicInput0.Evaluate(), true);
    EXPECT_EQ(LogicInput1.Evaluate(), false);
    EXPECT_EQ(LogicInput31.Evaluate(), true);
}


TEST(logic_test, CLogicNotOperator)
{
    CLogicInputData LogicInputData;
    LogicInputData.Set(0x00000002u);
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
    LogicInputData.Set(0x00000003u);
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
    LogicInputData.Set(0x00000002u);
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

    LogicInputData.Set(0xffffffffu);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);
}


TEST(logic_test, PostfixLogicParserAnd)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("AB*");

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000001u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000003u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);
}


TEST(logic_test, PostfixLogicParserOr)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("AB+");

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000001u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(0x00000003u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);
}


TEST(logic_test, PostfixLogicParserOrAndOr)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("AB+CD+*");

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000003u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000005u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);
}


TEST(logic_test, PostfixLogicParserNotOrAndOr)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("AB+CD+*!");

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(0x00000003u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(0x00000005u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);
}


TEST(logic_test, PostfixLogicParserWhitespacesLowerCase)
{
    CLogicInputData LogicInputData;
    CPostfixLogicParser PostfixLogicParser(LogicInputData);

    PostfixLogicParser.Parse("Ab+\tCd+ * !!!");

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(0x00000003u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), true);

    LogicInputData.Set(0x00000005u);
    EXPECT_EQ(PostfixLogicParser.Evaluate(), false);
}


TEST(logic_test, ExpressionParser01)
{
    CInfixToPostfixExpression InfixToPostfixExpression;
    const char expression[] = "A+B+C+D";
    char const* p_src = &expression[0u];
    char rpn[32] = { 0u };
    char *p_rpn = &rpn[0u];

    InfixToPostfixExpression.Parse(p_src, p_rpn);

    EXPECT_EQ(strncmp(&rpn[0u], "ABCD+++", sizeof(rpn)), 0);
}


TEST(logic_test, ExpressionParser01a)
{
    CInfixToPostfixExpression InfixToPostfixExpression;
    const char expression[] = "A*B*C*D";
    char const* p_src = &expression[0u];
    char rpn[32] = { 0u };
    char *p_rpn = &rpn[0u];

    InfixToPostfixExpression.Parse(p_src, p_rpn);

    EXPECT_EQ(strncmp(&rpn[0u], "ABCD***", sizeof(rpn)), 0);
}


TEST(logic_test, ExpressionParser02)
{
    CInfixToPostfixExpression InfixToPostfixExpression;
    const char expression[] = "(A+B)*(C+D)";
    char const* p_src = &expression[0u];
    char rpn[32] = { 0u };
    char *p_rpn = &rpn[0u];

    InfixToPostfixExpression.Parse(p_src, p_rpn);

    EXPECT_EQ(strncmp(&rpn[0u], "AB+CD+*", sizeof(rpn)), 0);
}


TEST(logic_test, ExpressionParser03)
{
    CInfixToPostfixExpression InfixToPostfixExpression;
    const char expression[] = "(A*B)+(C*D)+(A*(B*(C*D)))";
    char const* p_src = &expression[0u];
    char rpn[32] = { 0u };
    char *p_rpn = &rpn[0u];

    InfixToPostfixExpression.Parse(p_src, p_rpn);

    EXPECT_EQ(strncmp(&rpn[0u], "AB*CD*ABCD***++", sizeof(rpn)), 0);
}


TEST(logic_test, ExpressionParser04)
{
    CInfixToPostfixExpression InfixToPostfixExpression;
    const char expression[] = "((A+B)*(C+D))+((A*B)+(C*D))";
    char const* p_src = &expression[0u];
    char rpn[32] = { 0u };
    char *p_rpn = &rpn[0u];

    InfixToPostfixExpression.Parse(p_src, p_rpn);

    EXPECT_EQ(strncmp(&rpn[0u], "AB+CD+*AB*CD*++", sizeof(rpn)), 0);
}


TEST(logic_test, ExpressionParser05)
{
    CInfixToPostfixExpression InfixToPostfixExpression;
    const char expression[] = "A+(B*(!(C+D+E)))+(!F)";
    char const* p_src = &expression[0u];
    char rpn[32] = { 0u };
    char *p_rpn = &rpn[0u];

    InfixToPostfixExpression.Parse(p_src, p_rpn);

    EXPECT_EQ(strncmp(&rpn[0u], "ABCDE++!*F!++", sizeof(rpn)), 0);
}


TEST(logic_test, ExpressionParser06)
{
    CInfixToPostfixExpression InfixToPostfixExpression;
    const char expression[] = "(!A)*(!B)";
    char const* p_src = &expression[0u];
    char rpn[32] = { 0u };
    char *p_rpn = &rpn[0u];

    InfixToPostfixExpression.Parse(p_src, p_rpn);

    EXPECT_EQ(strncmp(&rpn[0u], "A!B!*", sizeof(rpn)), 0);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
