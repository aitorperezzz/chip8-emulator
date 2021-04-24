#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

#include "chip8.hpp"

// This class will test a mix of instructions
class TestMisc : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestMisc);
    CPPUNIT_TEST(testRET);
    CPPUNIT_TEST(testJP);
    CPPUNIT_TEST(testCALL);
    CPPUNIT_TEST(testLD_inmediate);
    CPPUNIT_TEST(testLD_register);
    CPPUNIT_TEST_SUITE_END();

public:
    void testRET(void);
    void testJP(void);
    void testCALL(void);
    void testLD_inmediate(void);
    void testLD_register(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestMisc);

void TestMisc::testRET(void)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values for the test
    unsigned short instruction = 0x00ee;
    std::array<unsigned short, SIZE_STACK> initialStack = {0x0, 0x1};
    unsigned char initialStackPointer = 0x1;
    unsigned char finalStackPointer = 0x0;
    unsigned short initialPc = 0x2;
    unsigned short finalPc = 0x1;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setStack(initialStack);
    chip8.setStackPointer(initialStackPointer);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the stack pointer is now pointing to one unit less
    // than was provided
    CPPUNIT_ASSERT_EQUAL(finalStackPointer, chip8.getStackPointer());

    // Check the program counter now points to the address that was provided
    // by the last element in the stack
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestMisc::testJP(void)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values for the test
    unsigned short instruction = 0x1e48;
    unsigned short initialPc = 0x2;
    unsigned short finalPc = 0x0e48;

    // Initialize instruction and program counter
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the program counter is pointing to the address provided by the
    // instruction
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestMisc::testCALL(void)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values for the test
    unsigned short instruction = 0x2e48;
    unsigned short initialPc = 0xff5;
    unsigned short finalPc = 0x0e48;
    unsigned char initialStackPointer = 0x1;
    unsigned char finalStackPointer = 0x2;
    std::array<unsigned short, SIZE_STACK> initialStack = {0x0, 0xe};
    std::array<unsigned short, SIZE_STACK> finalStack = {0x0, 0xe, 0xff5};

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setStackPointer(initialStackPointer);
    chip8.setStack(initialStack);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the stack pointer has incremented
    CPPUNIT_ASSERT_EQUAL(finalStackPointer, chip8.getStackPointer());

    // Check the pc has been placed at the top of the stack
    for (size_t i = 0; i < SIZE_STACK; i++)
    {
        CPPUNIT_ASSERT_EQUAL(chip8.getStack()[i], finalStack[i]);
    }

    // Check the pc has been now set to the address provided in the instruction
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);
}

void TestMisc::testLD_inmediate(void)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values for the test
    unsigned short instruction = 0x62f5;
    unsigned short initialPc = 0x2;
    unsigned short finalPc = 0x4;
    unsigned char regIndex = 0x2;
    unsigned char value = 0xf5;

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the register holds the proper value
    CPPUNIT_ASSERT_EQUAL(value, chip8.getRegister(regIndex));

    // Check the pc has incremented
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestMisc::testLD_register(void)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values for the test
    unsigned short instruction = 0x8230;
    unsigned short initialPc = 0x2;
    unsigned short finalPc = 0x4;
    unsigned char xregIndex = 0x2;
    unsigned char yregIndex = 0x3;
    unsigned char regValue = 0x5b;

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(yregIndex, regValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the x register now holds the value that was insde y. Check
    // the y register still holds the same value
    CPPUNIT_ASSERT_EQUAL(regValue, chip8.getRegister(xregIndex));
    CPPUNIT_ASSERT_EQUAL(regValue, chip8.getRegister(yregIndex));

    // Check the pc has incremented
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}