#include "mainwindow.h"

// These are used for testing and debugging
#include <iostream>
#include "gate.h"
#include "wire.h"

#include <QApplication>

// Testing function for debugging purposes
bool testGatesAndWires() {
    return true;
}


int main(int argc, char *argv[])
{
    std::cout << testGatesAndWires() << std::endl;
    srand(time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
