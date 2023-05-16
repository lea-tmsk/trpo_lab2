#include <QCoreApplication>
#include "unit.h"
#include "Java.h"
#include "CPP.h"
#include "CSharp.h"

std::string generateProgram(const std::shared_ptr<ClassFactory> &factory) {
    std::unique_ptr<ClassUnit> myClass = factory->createClassUnit("MyClass");
    myClass->add(factory->createMethodUnit("testFunc1", "void", MethodUnit::PUBLIC));

    myClass->add(factory->createMethodUnit("testFunc2", "void", MethodUnit::STATIC | MethodUnit::PRIVATE));

    myClass->add(factory->createMethodUnit("testFunc3", "void", MethodUnit::VIRTUAL | MethodUnit::CONST | MethodUnit::PROTECTED));

    std::shared_ptr<MethodUnit> method = factory->createMethodUnit("testFunc4", "void", MethodUnit::STATIC | MethodUnit::PROTECTED);
    method->add(factory->createPrintOperatorUnit(R"(Hello, world!\n)"));

    myClass->add(method);

    myClass->add(factory->createMethodUnit("testFunc5", "void", MethodUnit::VIRTUAL | MethodUnit::CONST | MethodUnit::PROTECTED_INTERNAL));

    return myClass->compile();
}

int main(int argc, char *argv[])
{
    CppClassFactory f1;
    QCoreApplication a(argc, argv);
    std::cout << "C++: \n\n" << generateProgram(std::make_shared<CppClassFactory>()) << std::endl;
    std::cout << "C#: \n\n" << generateProgram(std::make_shared<CSharpClassFactory>()) << std::endl;
    std::cout << "Java: \n\n" << generateProgram(std::make_shared<JavaClassFactory>()) << std::endl;
    return a.exec();
}
