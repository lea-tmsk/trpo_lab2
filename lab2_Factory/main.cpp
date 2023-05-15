#include <QCoreApplication>
#include "unit.h"
//#include "Java.h"
#include "CPP.h"
//#include "CSharp.h"

std::string generateProgram(const std::shared_ptr<ClassFactory> &factory) {
    auto myClass = factory->createClassUnit("TestClass");
    myClass->add(factory->createMethodUnit("testFunc1", "string", 1 << 2), ClassUnit::PUBLIC);
    return myClass->compile();
//    ClassUnit myClass( "MyClass" );
//    factory.createMethodUnit("testFunc1", "void", 0);
    /*myClass.add(
        std::make_shared<MethodUnit>("testFunc1", "void", 0),
        ClassUnit::PUBLIC
    );
    myClass.add(
        std::make_shared<MethodUnit>("testFunc2", "void", MethodUnit::STATIC),
        ClassUnit::PRIVATE
    );
    myClass.add(
        std::make_shared<MethodUnit>("testFunc3", "void", MethodUnit::VIRTUAL | MethodUnit::CONST),
        ClassUnit::PUBLIC
    );
    auto method = std::make_shared<MethodUnit>("testFunc4", "void", MethodUnit::STATIC);
    method->add(std::make_shared<PrintOperatorUnit>(R"(Hello, world!\n)"));
    myClass.add(method, ClassUnit::PROTECTED);
    return myClass.compile();*/
}

int main(int argc, char *argv[])
{
    CppClassFactory f1;
    QCoreApplication a(argc, argv);
    std::cout << generateProgram(std::make_shared<CppClassFactory>()) << std::endl;
    return a.exec();
}
