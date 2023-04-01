#include <gtest/gtest.h>
#include "logic.h"

TEST(logic_test, LogicInput)
{
    CLogicInputData LogicInputData;
    CLogicInput LogicInput0(LogicInputData, 0u);
    CLogicInput LogicInput1(LogicInputData, 1u);
    CLogicInput LogicInput31(LogicInputData, 31u);

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
    CLogicInput LogicInput0_H(LogicInputData, 0u);
    CLogicInput LogicInput1_H(LogicInputData, 1u);
    CLogicInput LogicInput2_L(LogicInputData, 2u);
    LogicInputData.Set(0x00000003u);

    CLogicAndOperator LogicAndOperator01_HH(LogicInput0_H, LogicInput1_H);
    CLogicAndOperator LogicAndOperator02_HL(LogicInput0_H, LogicInput2_L);

    EXPECT_EQ(LogicAndOperator01_HH.Evaluate(), true);
    EXPECT_EQ(LogicAndOperator02_HL.Evaluate(), false);
}


TEST(logic_test, CLogicOROperator)
{
    CLogicInputData LogicInputData;
    CLogicInput LogicInput0_L(LogicInputData, 0u);
    CLogicInput LogicInput1_H(LogicInputData, 1u);
    CLogicInput LogicInput2_L(LogicInputData, 2u);
    LogicInputData.Set(0x00000002u);

    CLogicOROperator LogicAndOperator01_LH(LogicInput0_L, LogicInput1_H);
    CLogicOROperator LogicAndOperator02_LL(LogicInput0_L, LogicInput2_L);

    EXPECT_EQ(LogicAndOperator01_LH.Evaluate(), true);
    EXPECT_EQ(LogicAndOperator02_LL.Evaluate(), false);
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
