#include "mainwindow.h"

// These are used for testing and debugging
#include <iostream>
#include "gate.h"
#include "wire.h"

#include <QApplication>

// Testing function for debugging purposes
QVector<bool> testGatesAndWires() {
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


int main(int argc, char *argv[])
{
    QVector<bool> outputs = testGatesAndWires();
    std::cout << "0 | 0 | " << int(outputs[0]) << std::endl;
    std::cout << "0 | 1 | " << int(outputs[1]) << std::endl;
    std::cout << "1 | 0 | " << int(outputs[2]) << std::endl;
    std::cout << "1 | 1 | " << int(outputs[3]) << std::endl;
    srand(time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
