#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

#include "chip8.hpp"

// This class will test all math related instructions
class TestMath : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestMath);
    CPPUNIT_TEST(testOR);
    CPPUNIT_TEST(testAND);
    CPPUNIT_TEST(testXOR);
    CPPUNIT_TEST(testADD_noOverflow);
    CPPUNIT_TEST(testADD_overflow);
    CPPUNIT_TEST(testSUB_noBorrow);
    CPPUNIT_TEST(testSUB_borrow);
    CPPUNIT_TEST(testSHR_LSB_on);
    CPPUNIT_TEST(testSHR_LSB_off);
    CPPUNIT_TEST(testSUBN_noBorrow);
    CPPUNIT_TEST(testSUBN_borrow);
    CPPUNIT_TEST(testSHL_MSB_on);
    CPPUNIT_TEST(testSHL_MSB_off);
    CPPUNIT_TEST_SUITE_END();

public:
    void testOR(void);
    void testAND(void);
    void testXOR(void);
    void testADD_noOverflow(void);
    void testADD_overflow(void);
    void testSUB_noBorrow(void);
    void testSUB_borrow(void);
    void testSHR_LSB_on(void);
    void testSHR_LSB_off(void);
    void testSUBN_noBorrow(void);
    void testSUBN_borrow(void);
    void testSHL_MSB_on(void);
    void testSHL_MSB_off(void);

private:
    void testADD(unsigned char xRegisterValue, unsigned char yRegisterValue);
    void testSUB(unsigned char xRegisterValue, unsigned char yRegisterValue);
    void testSHR(unsigned char xRegisterValue);
    void testSUBN(unsigned char xRegisterValue, unsigned char yRegisterValue);
    void testSHL(unsigned char xRegisterValue);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestMath);

void TestMath::testOR(void)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values for the test
    unsigned short instruction = 0x8121;
    unsigned short initialPc = 2;
    unsigned short finalPc = 4;
    unsigned char xRegister = 0x1;
    unsigned char yRegister = 0x2;
    unsigned char xRegisterValue = 0x01;
    unsigned char yRegisterValue = 0x10;
    unsigned char result = xRegisterValue | yRegisterValue;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(xRegister, xRegisterValue);
    chip8.setRegister(yRegister, yRegisterValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(chip8.executeCycle(), Ok);

    // Check the program counter has advanced two bytes
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);

    // Check result
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(xRegister), result);

    // Check the y register has been left untouched
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(yRegister), yRegisterValue);
}

void TestMath::testAND(void)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values for the test
    unsigned short instruction = 0x8122;
    unsigned short initialPc = 2;
    unsigned short finalPc = 4;
    unsigned char xRegister = 0x1;
    unsigned char yRegister = 0x2;
    unsigned char xRegisterValue = 0x01;
    unsigned char yRegisterValue = 0x10;
    unsigned char result = xRegisterValue & yRegisterValue;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(xRegister, xRegisterValue);
    chip8.setRegister(yRegister, yRegisterValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(chip8.executeCycle(), Ok);

    // Check the program counter has advanced two bytes
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);

    // Check the result
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(xRegister), result);

    // Check the y register has been left untouched
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(yRegister), yRegisterValue);
}

void TestMath::testXOR(void)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide some values for the test
    unsigned short instruction = 0x8123;
    unsigned short initialPc = 2;
    unsigned short finalPc = 4;
    unsigned char xRegister = 0x1;
    unsigned char yRegister = 0x2;
    unsigned char xRegisterValue = 0x01;
    unsigned char yRegisterValue = 0x11;
    unsigned char result = xRegisterValue ^ yRegisterValue;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(xRegister, xRegisterValue);
    chip8.setRegister(yRegister, yRegisterValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(chip8.executeCycle(), Ok);

    // Check the program counter has advanced two bytes
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);

    // Check the result
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(xRegister), result);

    // Check the y register has been left untouched
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(yRegister), yRegisterValue);
}

void TestMath::testADD(unsigned char xRegisterValue, unsigned char yRegisterValue)
{
    Chip8 chip8;
    chip8.initialize();

    // Decide values for the test
    unsigned short instruction = 0x8124;
    unsigned short initialPc = 2;
    unsigned short finalPc = 4;
    unsigned char xRegister = 0x1;
    unsigned char yRegister = 0x2;
    unsigned short completeResult = xRegisterValue + yRegisterValue;
    unsigned char storedResult = completeResult & 0xff;
    unsigned char carry = completeResult >> 8;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(xRegister, xRegisterValue);
    chip8.setRegister(yRegister, yRegisterValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(chip8.executeCycle(), Ok);

    // Check the program counter has advanced two bytes
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);

    // Check the result has been stored in the x output register
    // and that the F register has the correct flag
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(xRegister), storedResult);
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(0xf), carry);

    // Check the y register has been left untouched
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(yRegister), yRegisterValue);
}

void TestMath::testADD_noOverflow(void)
{
    testADD(0x01, 0x10);
    testADD(0x00, 0x78);
}

void TestMath::testADD_overflow(void)
{
    testADD(0xff, 0x01);
    testADD(0x01, 0xff);
}

void TestMath::testSUB(unsigned char xRegisterValue, unsigned char yRegisterValue)
{
    Chip8 chip8;
    chip8.initialize();

    // Values involved in the test
    unsigned short instruction = 0x8125;
    unsigned short initialPc = 2;
    unsigned short finalPc = 4;
    unsigned char xRegister = 0x1;
    unsigned char yRegister = 0x2;
    unsigned char result = xRegisterValue - yRegisterValue;
    unsigned char FRegValue = xRegisterValue > yRegisterValue ? 0x1 : 0x0;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(xRegister, xRegisterValue);
    chip8.setRegister(yRegister, yRegisterValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(chip8.executeCycle(), Ok);

    // Check the program counter has advanced two bytes
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);

    // Check the result
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(xRegister), result);
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(0xf), FRegValue);

    // Check the y register has been left untouched
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(yRegister), yRegisterValue);
}

void TestMath::testSUB_noBorrow()
{
    testSUB(0xff, 0x01);
    testSUB(0x10, 0x01);
}

void TestMath::testSUB_borrow()
{
    testSUB(0x01, 0xff);
    testSUB(0x01, 0x10);
}

void TestMath::testSHR(unsigned char xRegisterValue)
{
    Chip8 chip8;
    chip8.initialize();

    // Values involved in the test
    unsigned short instruction = 0x8106;
    unsigned short initialPc = 2;
    unsigned short finalPc = 4;
    unsigned char xRegister = 0x1;
    unsigned char result = xRegisterValue >> 1;
    unsigned char FRegValue = (xRegisterValue & 0x1) == 0x1 ? 1 : 0;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(xRegister, xRegisterValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(chip8.executeCycle(), Ok);

    // Check the program counter has advanced two bytes
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);

    // Check the result
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(xRegister), result);
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(0xf), FRegValue);
}

void TestMath::testSHR_LSB_on(void)
{
    testSHR(0x01);
    testSHR(0xff);
}

void TestMath::testSHR_LSB_off(void)
{
    testSHR(0x00);
    testSHR(0xf2);
}

void TestMath::testSUBN(unsigned char xRegisterValue, unsigned char yRegisterValue)
{
    Chip8 chip8;
    chip8.initialize();

    // Values involved in the test
    unsigned short instruction = 0x8127;
    unsigned short initialPc = 2;
    unsigned short finalPc = 4;
    unsigned char xRegister = 0x1;
    unsigned char yRegister = 0x2;
    unsigned char result = yRegisterValue - xRegisterValue;
    unsigned char FRegValue = yRegisterValue > xRegisterValue ? 1 : 0;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(xRegister, xRegisterValue);
    chip8.setRegister(yRegister, yRegisterValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(chip8.executeCycle(), Ok);

    // Check the program counter has advanced two bytes
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);

    // Check the result
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(xRegister), result);
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(0xf), FRegValue);

    // Check the y register has been left untouched
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(yRegister), yRegisterValue);
}

void TestMath::testSUBN_noBorrow(void)
{
    testSUBN(0x01, 0xff);
    testSUBN(0x01, 0x10);
}

void TestMath::testSUBN_borrow(void)
{
    testSUBN(0xff, 0x01);
    testSUBN(0x10, 0x01);
}

void TestMath::testSHL(unsigned char xRegisterValue)
{
    Chip8 chip8;
    chip8.initialize();

    // Values involved in the test
    unsigned short instruction = 0x810e;
    unsigned short initialPc = 2;
    unsigned short finalPc = 4;
    unsigned char xRegister = 0x1;
    unsigned char result = xRegisterValue << 1;
    unsigned char FRegValue = xRegisterValue >> 7 == 0x1 ? 1 : 0;

    // Initialize all the relevant chip8 values
    chip8.setInstructionInMemory(initialPc, instruction);
    chip8.setPc(initialPc);
    chip8.setRegister(xRegister, xRegisterValue);

    // Execute a cycle
    CPPUNIT_ASSERT_EQUAL(chip8.executeCycle(), Ok);

    // Check the program counter has advanced two bytes
    CPPUNIT_ASSERT_EQUAL(chip8.getPc(), finalPc);

    // Check the result and the flag
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(xRegister), result);
    CPPUNIT_ASSERT_EQUAL(chip8.getRegister(0xf), FRegValue);
}

void TestMath::testSHL_MSB_on(void)
{
    testSHL(0xff);
    testSHL(0xf0);
}

void TestMath::testSHL_MSB_off(void)
{
    testSHL(0x00);
    testSHL(0xef);
}