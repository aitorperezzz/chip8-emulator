#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

#include "chip8.hpp"

// This class will test all LD or load instructions, where values are moved
// from one place to another
class TestLoad : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestLoad);
    CPPUNIT_TEST(testLD_inmediate);
    CPPUNIT_TEST(testLD_register);
    CPPUNIT_TEST(testLD_setIRegister);
    CPPUNIT_TEST(testLD_setRegWithDelayTimer);
    CPPUNIT_TEST(testLD_setDelayTimer);
    CPPUNIT_TEST(testLD_setSoundTimer);
    CPPUNIT_TEST(testLD_storeRegisters);
    CPPUNIT_TEST(testLD_readRegisters);
    CPPUNIT_TEST_SUITE_END();

public:
    void testLD_inmediate(void);
    void testLD_register(void);
    void testLD_setIRegister(void);
    void testLD_setRegWithDelayTimer(void);
    void testLD_setDelayTimer(void);
    void testLD_setSoundTimer(void);
    void testLD_storeRegisters(void);
    void testLD_readRegisters(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLoad);

void TestLoad::testLD_inmediate(void)
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

void TestLoad::testLD_register(void)
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

void TestLoad::testLD_setIRegister()
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values
    const unsigned short instruction = 0xafe9;
    const unsigned short initialPc = 0x2;
    const unsigned short finalPc = 0x4;
    const unsigned short value = 0xfe9;

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the proper value is stored at the I register
    CPPUNIT_ASSERT_EQUAL(value, chip8.getI());

    // Check the pc has incremented
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestLoad::testLD_setRegWithDelayTimer()
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values
    const unsigned short instruction = 0xf507;
    const unsigned char dtrValue = 0x4;
    const unsigned char regIndex = 0x5;
    const unsigned short initialPc = 0x2;
    const unsigned short finalPc = 0x4;

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setDelayTimer(dtrValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the register now holds the proper value
    CPPUNIT_ASSERT_EQUAL(dtrValue, chip8.getRegister(regIndex));

    // Check the pc has incremented
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestLoad::testLD_setDelayTimer()
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values
    const unsigned short instruction = 0xf515;
    const unsigned char regValue = 0x4;
    const unsigned char regIndex = 0x5;
    const unsigned short initialPc = 0x2;
    const unsigned short finalPc = 0x4;

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(regIndex, regValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the delay timer has the proper value
    CPPUNIT_ASSERT_EQUAL(regValue, chip8.getDelayTimer());

    // Check the pc has incremented
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestLoad::testLD_setSoundTimer()
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values
    const unsigned short instruction = 0xf518;
    const unsigned char regValue = 0x4;
    const unsigned char regIndex = 0x5;
    const unsigned short initialPc = 0x2;
    const unsigned short finalPc = 0x4;

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(regIndex, regValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the sound timer has the proper value
    CPPUNIT_ASSERT_EQUAL(regValue, chip8.getSoundTimer());

    // Check the pc has incremented
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestLoad::testLD_storeRegisters()
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values
    const unsigned short instruction = 0xf455;
    const size_t numRegs = 5;
    const std::array<unsigned char, numRegs> regValues = {0x0, 0x1, 0x2, 0x3,
                                                          0x4};
    const unsigned short initialPc = 0x2;
    const unsigned short finalPc = 0x4;
    unsigned short iValue = 0x589;

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    for (size_t i = 0; i < numRegs; i++)
    {
        chip8.setRegister(i, regValues[i]);
    }
    chip8.setI(iValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the value stored at i register has not changed
    CPPUNIT_ASSERT_EQUAL(iValue, chip8.getI());

    // Check the values have been properly copied to memory, starting at
    // location iValue
    for (size_t i = 0; i < numRegs; i++)
    {
        CPPUNIT_ASSERT_EQUAL(regValues[i], chip8.getMemory(iValue + i));
    }

    // Check the pc has incremented
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}

void TestLoad::testLD_readRegisters()
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values
    const unsigned short instruction = 0xf465;
    const size_t numRegs = 5;
    const std::array<unsigned char, numRegs> regValues = {0x0, 0x1, 0x2, 0x3,
                                                          0x4};
    const unsigned short initialPc = 0x2;
    const unsigned short finalPc = 0x4;
    unsigned short iValue = 0x589;

    // Initialize internal variables
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    for (size_t i = 0; i < numRegs; i++)
    {
        chip8.setMemory(iValue + i, regValues[i]);
    }
    chip8.setI(iValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(Ok, chip8.executeCycle());

    // Check the value stored at i register has not changed
    CPPUNIT_ASSERT_EQUAL(iValue, chip8.getI());

    // Check the values have been properly copied to the registers
    for (size_t i = 0; i < numRegs; i++)
    {
        CPPUNIT_ASSERT_EQUAL(regValues[i], chip8.getRegister(i));
    }

    // Check the pc has incremented
    CPPUNIT_ASSERT_EQUAL(finalPc, chip8.getPc());
}