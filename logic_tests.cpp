#include <gtest/gtest.h>
#include "logic.h"
#include "rpn_parser.h"


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


TEST(logic_test, CRPNLogicParser_NoExpression)
{
    CLogicInputData LogicInputData;
    CRPNLogicParser RPNLogicParser(LogicInputData);

    const char c_Expression[] = "";
    RPNLogicParser.Parse(&c_Expression[0u]);

    LogicInputData.Set(0xffffffffu);
    EXPECT_EQ(RPNLogicParser.Evaluate(), false);
}


TEST(logic_test, CRPNLogicParserAnd)
{
    CLogicInputData LogicInputData;
    CRPNLogicParser RPNLogicParser(LogicInputData);

    const char c_Expression[] = "ab*";
    RPNLogicParser.Parse(&c_Expression[0u]);

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000001u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000003u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), true);
}


TEST(logic_test, CRPNLogicParserOr)
{
    CLogicInputData LogicInputData;
    CRPNLogicParser RPNLogicParser(LogicInputData);

    const char c_Expression[] = "ab+";
    RPNLogicParser.Parse(&c_Expression[0u]);

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000001u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), true);

    LogicInputData.Set(0x00000003u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), true);
}


TEST(logic_test, CRPNLogicParserOrAndOr)
{
    CLogicInputData LogicInputData;
    CRPNLogicParser RPNLogicParser(LogicInputData);

    const char c_Expression[] = "ab+cd+*";
    RPNLogicParser.Parse(&c_Expression[0u]);

    LogicInputData.Set(0x00000000u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000003u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), false);

    LogicInputData.Set(0x00000005u);
    EXPECT_EQ(RPNLogicParser.Evaluate(), true);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}