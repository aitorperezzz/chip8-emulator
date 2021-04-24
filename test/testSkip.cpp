#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

#include "chip8.hpp"

// This class will test all stepping and skipping related instructions
class TestSkip : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestSkip);
    CPPUNIT_TEST(testSE_skip);
    CPPUNIT_TEST(testSE_noSkip);
    CPPUNIT_TEST(testSNE_skip);
    CPPUNIT_TEST(testSNE_noSkip);
    CPPUNIT_TEST(testSE_registers_skip);
    CPPUNIT_TEST(testSE_registers_noSkip);
    CPPUNIT_TEST_SUITE_END();

public:
    void testSE(void);
    void testSE_skip(void);
    void testSE_noSkip(void);
    void testSNE_skip(void);
    void testSNE_noSkip(void);
    void testSE_registers_skip(void);
    void testSE_registers_noSkip(void);

private:
    void testSE(const unsigned short initialPc,
                const unsigned char registerIndex,
                const unsigned char registerValue,
                const unsigned char instructionValue, const bool skip);
    void testSNE(const unsigned short initialPc,
                 const unsigned char registerIndex,
                 const unsigned char registerValue,
                 const unsigned char instructionValue, const bool skip);
    void testSE_registers(const unsigned short initialPc,
                          const unsigned char registerIndex1,
                          const unsigned char registerValue1,
                          const unsigned char registerIndex2,
                          const unsigned char registerValue2, const bool skip);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSkip);

void TestSkip::testSE(const unsigned short initialPc,
                      const unsigned char registerIndex,
                      const unsigned char registerValue,
                      const unsigned char instructionValue, const bool skip)
{
    Chip8 chip8;
    chip8.initialize();

    // Initialize internal variables
    const unsigned short instruction =
        (0x3 << 12) + (registerIndex << 8) + instructionValue;
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(registerIndex, registerValue);

    // Decide the final value of the program counter.
    unsigned short finalPc = skip ? initialPc + 4 : initialPc + 2;

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the final value of the program counter
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestSkip::testSE_skip(void)
{
    // Decide some values
    unsigned char instructionValue = 0x5e;
    unsigned short initialPc = 0xff1;
    unsigned char registerIndex = 0x2;
    unsigned char registerValue = 0x5e;

    testSE(initialPc, registerIndex, registerValue, instructionValue, true);
}

void TestSkip::testSE_noSkip(void)
{
    // Decide some values
    unsigned short instructionValue = 0x5e;
    unsigned short initialPc = 0xff1;
    unsigned char registerIndex = 0x2;
    unsigned char registerValue = 0x5f;

    testSE(initialPc, registerIndex, registerValue, instructionValue, false);
}

void TestSkip::testSNE(const unsigned short initialPc,
                       const unsigned char registerIndex,
                       const unsigned char registerValue,
                       const unsigned char instructionValue, const bool skip)
{
    Chip8 chip8;
    chip8.initialize();

    // Initialize internal variables
    const unsigned short instruction =
        (0x4 << 12) + (registerIndex << 8) + instructionValue;
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(registerIndex, registerValue);

    // Decide the final value of the program counter.
    unsigned short finalPc = skip ? initialPc + 4 : initialPc + 2;

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the final value of the program counter
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestSkip::testSNE_skip(void)
{
    // Decide some values
    unsigned short instructionValue = 0x5e;
    unsigned short initialPc = 0xff1;
    unsigned char registerIndex = 0x2;
    unsigned char registerValue = 0x5f;

    testSNE(initialPc, registerIndex, registerValue, instructionValue, true);
}

void TestSkip::testSNE_noSkip(void)
{
    // Decide some values
    unsigned short instructionValue = 0x5e;
    unsigned short initialPc = 0xff1;
    unsigned char registerIndex = 0x2;
    unsigned char registerValue = 0x5e;

    testSNE(initialPc, registerIndex, registerValue, instructionValue, false);
}

void TestSkip::testSE_registers(const unsigned short initialPc,
                                const unsigned char registerIndex1,
                                const unsigned char registerValue1,
                                const unsigned char registerIndex2,
                                const unsigned char registerValue2,
                                const bool skip)
{
    Chip8 chip8;
    chip8.initialize();

    // Initialize internal variables
    const unsigned short instruction =
        (0x5 << 12) + (registerIndex1 << 8) + (registerIndex2 << 4);
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(registerIndex1, registerValue1);
    chip8.setRegister(registerIndex2, registerValue2);

    // Decide the final value of the program counter.
    unsigned short finalPc = skip ? initialPc + 4 : initialPc + 2;

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the final value of the program counter
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestSkip::testSE_registers_skip()
{
    // Decide some values
    unsigned short initialPc = 0xff1;
    unsigned char registerIndex1 = 0x2;
    unsigned char registerValue1 = 0x5e;
    unsigned char registerIndex2 = 0x3;
    unsigned char registerValue2 = 0x5e;

    testSE_registers(initialPc, registerIndex1, registerValue1, registerIndex2,
                     registerValue2, true);
}

void TestSkip::testSE_registers_noSkip()
{
    // Decide some values
    unsigned short initialPc = 0xff1;
    unsigned char registerIndex1 = 0x2;
    unsigned char registerValue1 = 0x5e;
    unsigned char registerIndex2 = 0x3;
    unsigned char registerValue2 = 0x5f;

    testSE_registers(initialPc, registerIndex1, registerValue1, registerIndex2,
                     registerValue2, false);
}