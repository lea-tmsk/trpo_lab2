#ifndef CPP_H
#define CPP_H
#include "unit.h"
#include <algorithm>

class CppMethodUnit : public MethodUnit {
public:
    enum Modifier {
        STATIC = 1,
        CONST = 1 << 1,
        VIRTUAL = 1 << 2
    };

    CppMethodUnit(const std::string& name, const std::string& returnType, Flags flags) :
        MethodUnit(name, returnType, flags) {}

    std::string compile(unsigned int level = 0) const override {
        std::string result = generateShift(level);

        if (getFlags() & STATIC) {
            result += "static ";
        } else if (getFlags() & VIRTUAL) {
            result += "virtual ";
        }

        result += getReturnType() + " ";
        result += getName() + "()";

        if (getFlags() & CONST) {
            result += " const";
        }

        result += " {\n\n";
std::vector<std::shared_ptr<Unit>> _body = getBody();

        for (const auto& b : getBody()) {
            result += b->compile(level + 1);
        }

        result += generateShift(level) + "}\n";
        return result;
    }
};

class CppClassUnit : public ClassUnit
{
public:
    CppClassUnit(const std::string& name) : ClassUnit(name) {}

    std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level) + "class " + getName() + " {\n";
        std::vector<std::string> CPPModifiers = {"public", "protected", "private"};
        for (size_t i = 0; i < ACCESS_MODIFIERS.size(); ++i) {
            if (getFields(i).empty()) {
                continue;
            }

            if (std::find(CPPModifiers.begin(), CPPModifiers.end(), ACCESS_MODIFIERS[i])!= CPPModifiers.end()) {
                result += ACCESS_MODIFIERS[i] + ":\n";
            }

            for(const auto& f : getFields(i)) {
                result += f->compile(level + 1);
            }
                result += "\n";
        }

        result += generateShift(level) + "};\n";
        return result;
    }
};

//const std::vector<std::string> CppClassUnit::ACCESS_MODIFIERS = {"public", "protected", "private"};

class CppPrintOperatorUnit : public PrintOperatorUnit {
public:
    explicit CppPrintOperatorUnit(const std::string& text) : PrintOperatorUnit(text) {}

    std::string compile(unsigned int level = 0) const {
        return generateShift(level) + "printf(\"" + getText() + "\");\n";
    }
};

class CppClassFactory : public ClassFactory {
public:
    std::unique_ptr<MethodUnit> createMethodUnit(const std::string& name, const std::string& returnType, Flags flags) const override {
        return std::unique_ptr<MethodUnit> (new CppMethodUnit(name, returnType, flags));
    }
    std::unique_ptr<ClassUnit> createClassUnit(const std::string& name) const override {
        return std::unique_ptr<ClassUnit> (new CppClassUnit(name));
    }
    std::unique_ptr<PrintOperatorUnit> createPrintOperatorUnit(const std::string& text) const override {
        return std::unique_ptr<PrintOperatorUnit> (new CppPrintOperatorUnit(text));
    }
};

#endif // CPP_H
