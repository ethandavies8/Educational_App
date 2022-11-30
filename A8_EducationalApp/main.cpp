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
void testGatesAndWires();

int main(int argc, char *argv[])
{
    testGatesAndWires();
    srand(time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// TESTING FUNCTIONS
// Initial function that will print and call subsequent tests
void testGatesAndWires() {
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
}

/* Test the NOT gate with a simple circuit

   SRC1 - - |>o - - OUT
*/
QVector<bool> testNOT() {
    SourceGate input1;
    NOTGate notGate;
    OutputGate output;
    Wire input1ToNot(input1, notGate);
    Wire notToOutput(notGate, output);

    QVector<bool> result;

    for (int i = 0; i < 2; i++) {
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
QVector<bool> testAND() {
    SourceGate input1, input2;
    ANDGate andGate;
    OutputGate output;
    Wire input1ToAnd(input1, andGate);
    Wire input2ToAnd(input2, andGate);
    Wire andToOutput(andGate, output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++) {
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
QVector<bool> testNAND() {
    SourceGate input1, input2;
    NANDGate nandGate;
    OutputGate output;
    Wire input1ToNand(input1, nandGate);
    Wire input2ToNand(input2, nandGate);
    Wire nandToOutput(nandGate, output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++) {
        bool x = i & 1;
        bool y = i & 2;
        input1.setOutput(x);
        input2.setOutput(y);
        result.push_back(output.getOutput());
    }

    return result;
}

/* Test the OR gate with a simple circuit

   SRC1 - - - |--
               |  >---OUT
   SRC2 - - - |--/
*/
QVector<bool> testOR() {
    SourceGate input1, input2;
    ORGate orGate;
    OutputGate output;
    Wire input1ToOr(input1, orGate);
    Wire input2ToOr(input2, orGate);
    Wire orToOutput(orGate, output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++) {
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
QVector<bool> testNOR() {
    SourceGate input1, input2;
    NORGate norGate;
    OutputGate output;
    Wire input1ToNor(input1, norGate);
    Wire input2ToNor(input2, norGate);
    Wire norToOutput(norGate, output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++) {
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
QVector<bool> testXOR() {
    SourceGate input1, input2;
    XORGate xorGate;
    OutputGate output;
    Wire input1ToXor(input1, xorGate);
    Wire input2ToXor(input2, xorGate);
    Wire xorToOutput(xorGate, output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++) {
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
QVector<bool> testXNOR() {
    SourceGate input1, input2;
    XNORGate xnorGate;
    OutputGate output;
    Wire input1ToXnor(input1, xnorGate);
    Wire input2ToXnor(input2, xnorGate);
    Wire xnorToOutput(xnorGate, output);

    QVector<bool> result;

    for (int i = 0; i < 4; i++) {
        bool x = i & 1;
        bool y = i & 2;
        input1.setOutput(x);
        input2.setOutput(y);
        result.push_back(output.getOutput());
    }

    return result;
}
