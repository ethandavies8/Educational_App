#include "mainwindow.h"

// These are used for testing and debugging
#include <iostream>
#include "gate.h"
#include "wire.h"

#include <QApplication>

QVector<bool> testNOT();
QVector<bool> testAND();
QVector<bool> testNAND();
QVector<bool> testOR();
QVector<bool> testNOR();
QVector<bool> testXOR();
QVector<bool> testXNOR();
QVector<bool> testBiggerGate();
QVector<std::tuple<bool, bool>> testAdderCircuit();
void testCases();
void testGatesAndWires();
void clearFlags(QVector<Gate *> gates);

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //testGatesAndWires();
    //srand(time(0));
    return a.exec();
}

// TESTING FUNCTIONS
// Initial function that will print and call subsequent tests
void testGatesAndWires()
{

    // Test base cases / error checking
    testCases();

    QVector<bool> outputs;
    outputs = testNOT();
    std::cout << "NOT GATE" << std::endl;
    std::cout << "0 | " << int(outputs[0]) << std::endl;
    std::cout << "1 | " << int(outputs[1]) << std::endl;
    outputs = testAND();
    std::cout << "AND GATE" << std::endl;
    std::cout << "0 | 0 | " << int(outputs[0]) << std::endl;
    std::cout << "0 | 1 | " << int(outputs[1]) << std::endl;
    std::cout << "1 | 0 | " << int(outputs[2]) << std::endl;
    std::cout << "1 | 1 | " << int(outputs[3]) << std::endl;
    outputs = testNAND();
    std::cout << "NAND GATE" << std::endl;
    std::cout << "0 | 0 | " << int(outputs[0]) << std::endl;
    std::cout << "0 | 1 | " << int(outputs[1]) << std::endl;
    std::cout << "1 | 0 | " << int(outputs[2]) << std::endl;
    std::cout << "1 | 1 | " << int(outputs[3]) << std::endl;
    outputs = testOR();
    std::cout << "OR GATE" << std::endl;
    std::cout << "0 | 0 | " << int(outputs[0]) << std::endl;
    std::cout << "0 | 1 | " << int(outputs[1]) << std::endl;
    std::cout << "1 | 0 | " << int(outputs[2]) << std::endl;
    std::cout << "1 | 1 | " << int(outputs[3]) << std::endl;
    outputs = testNOR();
    std::cout << "NOR GATE" << std::endl;
    std::cout << "0 | 0 | " << int(outputs[0]) << std::endl;
    std::cout << "0 | 1 | " << int(outputs[1]) << std::endl;
    std::cout << "1 | 0 | " << int(outputs[2]) << std::endl;
    std::cout << "1 | 1 | " << int(outputs[3]) << std::endl;
    outputs = testXOR();
    std::cout << "XOR GATE" << std::endl;
    std::cout << "0 | 0 | " << int(outputs[0]) << std::endl;
    std::cout << "0 | 1 | " << int(outputs[1]) << std::endl;
    std::cout << "1 | 0 | " << int(outputs[2]) << std::endl;
    std::cout << "1 | 1 | " << int(outputs[3]) << std::endl;
    outputs = testXNOR();
    std::cout << "XNOR GATE" << std::endl;
    std::cout << "0 | 0 | " << int(outputs[0]) << std::endl;
    std::cout << "0 | 1 | " << int(outputs[1]) << std::endl;
    std::cout << "1 | 0 | " << int(outputs[2]) << std::endl;
    std::cout << "1 | 1 | " << int(outputs[3]) << std::endl;
    outputs = testBiggerGate();
    std::cout << "XOR GATE (4 INPUTS)" << std::endl;
    for (int i = 0; i < outputs.length(); i++)
    {
        bool a = i & 8;
        bool b = i & 4;
        bool c = i & 2;
        bool d = i & 1;
        std::string separator = " | ";
        std::string outputSeparator = " || ";
        std::cout << int(a) << separator << int(b) << separator << int(c) << separator << int(d) << outputSeparator << outputs[i] << std::endl;
    }

    QVector<std::tuple<bool, bool>> adderOutputs = testAdderCircuit();
    std::cout << "Adder Circuit" << std::endl;
    std::cout << "A | B | Cin || S | Cout" << std::endl;
    for (int i = 0; i < adderOutputs.length(); i++)
    {
        bool A = i & 4;
        bool B = i & 2;
        bool Cin = i & 1;
        bool S, Cout;
        std::tie(S, Cout) = adderOutputs[i];
        std::cout << int(A) << " | " << int(B) << " | " << int(Cin) << "   || " << int(S) << " | " << int(Cout) << std::endl;
    }
}

/* Test the NOT gate with a simple circuit

   SRC1 - - |>o - - OUT
*/
QVector<bool> testNOT()
{
    SourceGate input1;
    NOTGate notGate;
    OutputGate output;
    Wire input1ToNot(&input1, &notGate);
    Wire notToOutput(&notGate, &output);

    QVector<bool> result;

    for (int i = 0; i < 2; i++)
    {
        bool x = i & 1;
        input1.setOutput(x);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test the AND gate with a simple circuit

   SRC1 - - - |--)
              |   )---OUT
   SRC2 - - - |--)
*/
QVector<bool> testAND()
{
    SourceGate input1, input2;
    ANDGate andGate;
    OutputGate output;
    Wire input1ToAnd(&input1, &andGate);
    Wire input2ToAnd(&input2, &andGate);
    Wire andToOutput(&andGate, &output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++)
    {
        bool x = i & 1;
        bool y = i & 2;
        input1.setOutput(x);
        input2.setOutput(y);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test the NAND gate with a simple circuit

   SRC1 - - - |--)
              |   )o---OUT
   SRC2 - - - |--)
*/
QVector<bool> testNAND()
{
    SourceGate input1, input2;
    NANDGate nandGate;
    OutputGate output;
    Wire input1ToNand(&input1, &nandGate);
    Wire input2ToNand(&input2, &nandGate);
    Wire nandToOutput(&nandGate, &output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++)
    {
        bool x = i & 1;
        bool y = i & 2;
        input1.setOutput(x);
        input2.setOutput(y);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test the OR gate with a simple circuit

   SRC1 - - - |--\
               |  >---OUT
   SRC2 - - - |--/
*/
QVector<bool> testOR()
{
    SourceGate input1, input2;
    ORGate orGate;
    OutputGate output;
    Wire input1ToOr(&input1, &orGate);
    Wire input2ToOr(&input2, &orGate);
    Wire orToOutput(&orGate, &output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++)
    {
        bool x = i & 1;
        bool y = i & 2;
        input1.setOutput(x);
        input2.setOutput(y);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test the NOR gate with a simple circuit

   SRC1 - - - |--\
               |  >o---OUT
   SRC2 - - - |--/
*/
QVector<bool> testNOR()
{
    SourceGate input1, input2;
    NORGate norGate;
    OutputGate output;
    Wire input1ToNor(&input1, &norGate);
    Wire input2ToNor(&input2, &norGate);
    Wire norToOutput(&norGate, &output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++)
    {
        bool x = i & 1;
        bool y = i & 2;
        input1.setOutput(x);
        input2.setOutput(y);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test the XOR gate with a simple circuit

   SRC1 - - -| |--\
              | |  >---OUT
   SRC2 - - -| |--/
*/
QVector<bool> testXOR()
{
    SourceGate input1, input2;
    XORGate xorGate;
    OutputGate output;
    Wire input1ToXor(&input1, &xorGate);
    Wire input2ToXor(&input2, &xorGate);
    Wire xorToOutput(&xorGate, &output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++)
    {
        bool x = i & 1;
        bool y = i & 2;
        input1.setOutput(x);
        input2.setOutput(y);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test the XNOR gate with a simple circuit

   SRC1 - - -| |--\
              | |  >o---OUT
   SRC2 - - -| |--/
*/
QVector<bool> testXNOR()
{
    SourceGate input1, input2;
    XNORGate xnorGate;
    OutputGate output;
    Wire input1ToXnor(&input1, &xnorGate);
    Wire input2ToXnor(&input2, &xnorGate);
    Wire xnorToOutput(&xnorGate, &output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++)
    {
        bool x = i & 1;
        bool y = i & 2;
        input1.setOutput(x);
        input2.setOutput(y);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test a gate with more than 2 inputs
   We'll use an XOR gate for this test

   SRC1 - - -| |-----\
              | |      \
   SRC2 - - - -| |       \
                | |        >---OUT
   SRC3 - - - -| |       /
              | |      /
   SRC4 - - -| |-----/
*/
QVector<bool> testBiggerGate()
{
    SourceGate input1, input2, input3, input4;
    XORGate xorGate;
    OutputGate output;
    Wire input1Out(&input1, &xorGate);
    Wire input2Out(&input2, &xorGate);
    Wire input3Out(&input3, &xorGate);
    Wire input4Out(&input4, &xorGate);
    Wire xorOut(&xorGate, &output);

    QVector<bool> result;

    for (int i = 0; i < 16; i++)
    {
        bool a = i & 8;
        bool b = i & 4;
        bool c = i & 2;
        bool d = i & 1;

        input1.setOutput(a);
        input2.setOutput(b);
        input3.setOutput(c);
        input4.setOutput(d);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test an adder circuit
 * An example of an adder circuit can be seen online
 */
QVector<std::tuple<bool, bool>> testAdderCircuit()
{
    SourceGate inputA, inputB, inputCin;
    XORGate a_XOR_b, aXORb_XOR_cin;
    ANDGate a_AND_b, aXORb_AND_cin;
    ORGate aXORbANDcin_OR_aANDb;
    OutputGate outputS, outputCout;
    Wire inputAOut(&inputA, &a_XOR_b);
    inputAOut.connect(&a_AND_b);
    Wire inputBOut(&inputB, &a_XOR_b);
    inputBOut.connect(&a_AND_b);
    Wire inputCinOut(&inputCin, &aXORb_XOR_cin);
    inputCinOut.connect(&aXORb_AND_cin);
    Wire a_XOR_bOut(&a_XOR_b, &aXORb_XOR_cin);
    a_XOR_bOut.connect(&aXORb_AND_cin);
    Wire aXORb_XOR_cinOut(&aXORb_XOR_cin, &outputS);
    Wire aXORb_AND_cinOut(&aXORb_AND_cin, &aXORbANDcin_OR_aANDb);
    Wire a_AND_bOut(&a_AND_b, &aXORbANDcin_OR_aANDb);
    Wire aXORbANDcin_OR_aANDbOut(&aXORbANDcin_OR_aANDb, &outputCout);

    QVector<std::tuple<bool, bool>> result;

    for (int i = 0; i < 8; i++)
    {
        bool x = i & 1;
        bool y = i & 2;
        bool z = i & 4;
        inputCin.setOutput(x);
        inputB.setOutput(y);
        inputA.setOutput(z);
        result.push_back(std::tuple<bool, bool>{outputS.getOutput(), outputCout.getOutput()});
    }

    return result;
}

/* Test base cases with circuits

  TEST1 (input gate isn't connected):
   SRC1 - - - |--)
          \   |   ) - - OUT
            - |--)
   SRC2

  TEST2 (gate's output isn't connected):
   SRC1 - - - |--\
               |  >---OUT
   SRC2 - - - |--/
          \
            -|>o

  TEST3 (output isn't connected):
   SRC1 - - - |>o -| |--\
                    | |  >  OUT
   SRC2 - - - |>o -| |--/

  TEST4 (incomplete gate):
   SRC1 - - - |--)
              |   )--OUT
              |--)

  TEST5 (circular circuit):
   SRC1 - - - |--)
              |   )-\
           - -|--)  /
       - -| - - - -    OUT
      |   \- - - - -
       \- - - |--)   \
              |   )-/
   SRC2 - - - |--)
*/
void testCases()
{
    // TEST 1
    {
        SourceGate input1, input2;
        ANDGate andGate;
        OutputGate output;
        Wire input1ToAnd(&input1, &andGate);
        input1ToAnd.connect(&andGate);
        Wire andToOutput(&andGate, &output);

        QVector<bool> result;

        for (int i = 0; i < 4; i++)
        {
            bool x = i & 1;
            bool y = i & 2;
            input1.setOutput(x);
            input2.setOutput(y);
            result.push_back(output.getOutput());
        }

        // Expected:
        // 0 | 0 | 0
        // 0 | 1 | 1
        // 1 | 0 | 0
        // 1 | 1 | 1
        std::cout << "INPUTDISCONNECTEDTEST" << std::endl;
        if (!result[0] && result[1] && !result[2] && result[3])
            std::cout << "  TEST PASSED" << std::endl;
    }

    // TEST 2
    {
        SourceGate input1, input2;
        ORGate orGate;
        NOTGate notGate;
        OutputGate output;
        Wire input1Out(&input1, &orGate);
        Wire input2Out(&input2, &orGate);
        input2Out.connect(&notGate);
        Wire orToOutput(&orGate, &output);

        QVector<bool> result;

        for (int i = 0; i < 4; i++)
        {
            bool x = i & 1;
            bool y = i & 2;
            input1.setOutput(x);
            input2.setOutput(y);
            result.push_back(output.getOutput());
        }

        // Expected:
        // 0 | 0 | 0
        // 0 | 1 | 1
        // 1 | 0 | 1
        // 1 | 1 | 1
        std::cout << "GATEDISCONNECTEDTEST" << std::endl;
        if (!result[0] && result[1] && result[2] && result[3])
            std::cout << "  TEST PASSED" << std::endl;
    }

    // TEST 3
    {
        SourceGate input1, input2;
        XORGate xorGate;
        NOTGate notGate1, notGate2;
        OutputGate output;
        Wire input1Out(&input1, &notGate1);
        Wire input2Out(&input2, &notGate2);
        Wire not1ToXor(&notGate1, &xorGate);
        Wire not2ToXor(&notGate2, &xorGate);

        QVector<bool> result;

        for (int i = 0; i < 4; i++)
        {
            bool x = i & 1;
            bool y = i & 2;
            input1.setOutput(x);
            input2.setOutput(y);
            result.push_back(output.getOutput());
        }

        // Expected:
        // 0 | 0 | 0
        // 0 | 1 | 0
        // 1 | 0 | 0
        // 1 | 1 | 0
        // In this case, the user won't be able to compute a truth table
        //  if the output is not connected
        std::cout << "OUTPUTDISCONNECTEDTEST" << std::endl;
        if (!result[0] && !result[1] && !result[2] && !result[3])
            std::cout << "  TEST PASSED" << std::endl;
    }

    // TEST 4
    {
        SourceGate input1;
        ANDGate andGate;
        OutputGate output;
        Wire input1Out(&input1, &andGate);
        Wire andToOutput(&andGate, &output);

        QVector<bool> result;

        for (int i = 0; i < 2; i++)
        {
            bool x = i & 1;
            input1.setOutput(x);
            result.push_back(output.getOutput());
        }

        // Expected:
        // 0 | 0
        // 1 | 0
        // In this case, the user won't be able to compute a truth table
        //  if a gate doesn't have the minimum number of inputs
        std::cout << "INCOMPLETEGATETEST" << std::endl;
        if (!result[0] && !result[1])
            std::cout << "  TEST PASSED" << std::endl;
    }

    // TEST 5
    {
        // This program must avoid circular dependencies
        std::cout << "CIRCULARTEST" << std::endl;

        SourceGate input1, input2;
        ANDGate andGate1, andGate2;
        OutputGate output;
        Wire input1ToAnd1(&input1, &andGate1);
        Wire input2ToAnd2(&input2, &andGate2);
        Wire and1ToAnd2(&andGate1, &andGate2);
        Wire and2ToAnd1(&andGate2, &andGate1);

        // This circuit is circular, so it will crash if the test fails

        input1.setOutput(true);
        std::cout << "  TEST PASSED" << std::endl;
    }
}
