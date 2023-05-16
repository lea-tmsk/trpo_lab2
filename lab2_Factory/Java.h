#ifndef JAVA_H
#define JAVA_H
#include "unit.h"

class JavaMethodUnit : public MethodUnit {
public:
    JavaMethodUnit(const std::string& name, const std::string& returnType, Flags flags) :
        MethodUnit(name, returnType, flags) {}

    std::string compile(unsigned int level = 0) const override {
        std::string result = generateShift(level);
        Flags _flags = getFlags();

        //модификаторы доступа
        if (_flags & PUBLIC) {
            result += "public ";
        } else if (_flags & PROTECTED) {
            result += "protected ";
        } else {
            result += "private ";
        }

        //модификаторы метода
        if (_flags & STATIC) {
            result += "static ";
        }
        if (_flags & FINAL) {
            result += "final ";
        }
        if (_flags & ABSTRACT) {
            result += "abstract ";
        }

        result += getReturnType() + " ";
        result += getName() + "()";

        result += " {\n";

        for (const auto& b : getBody()) {
            result += b->compile(level + 1);
        }

        result += generateShift(level) + "}\n\n";
        return result;
    }
};

class JavaClassUnit : public ClassUnit
{
public:
    JavaClassUnit(const std::string& name) : ClassUnit(name) {}

    std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level) + "class " + getName() + " {\n";

        for (size_t i = 0; i < ACCESS_MODIFIERS.size(); ++i) {
            if (getFields(i).empty()) {
                continue;
            }

            for (const auto& f : getFields(i)) {
                result += f->compile(level + 1);
            }
        }

        result += generateShift(level) + "};\n";
        return result;
    }
};

class JavaPrintOperatorUnit : public PrintOperatorUnit {
public:
    explicit JavaPrintOperatorUnit(const std::string& text) : PrintOperatorUnit(text) {}

    std::string compile(unsigned int level = 0) const {
        return generateShift(level) + "printf(\"" + getText() + "\");\n";
    }
};

class JavaClassFactory : public ClassFactory {
public:
    std::unique_ptr<MethodUnit> createMethodUnit(const std::string& name, const std::string& returnType, Flags flags) const override {
        return std::unique_ptr<MethodUnit> (new JavaMethodUnit(name, returnType, flags));
    }
    std::unique_ptr<ClassUnit> createClassUnit(const std::string& name) const override {
        return std::unique_ptr<ClassUnit> (new JavaClassUnit(name));
    }
    std::unique_ptr<PrintOperatorUnit> createPrintOperatorUnit(const std::string& text) const override {
        return std::unique_ptr<PrintOperatorUnit> (new JavaPrintOperatorUnit(text));
    }
};

#endif // JAVA_H
