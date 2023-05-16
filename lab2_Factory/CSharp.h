#ifndef CSHARP_H
#define CSHARP_H
#include "unit.h"

class CSharpMethodUnit : public MethodUnit {
public:

    CSharpMethodUnit(const std::string& name, const std::string& returnType, Flags flags) :
        MethodUnit(name, returnType, flags) {}

    std::string compile(unsigned int level = 0) const override {
        std::string result = generateShift(level);
        Flags flags = getFlags();

        if (flags & PUBLIC) {
            result += "public ";
        } else if (flags & PROTECTED) {
            result += "protected ";
        } else if (flags & PRIVATE_PROTECTED) {
            result += "private protected ";
        } else if (flags & INTERNAL) {
            result += "internal ";
        } else if (flags & PROTECTED_INTERNAL) {
            result += "protected internal ";
        } else {
            result += "private ";
        }

        if (flags & STATIC) {
            result += "static ";
        } else if (flags & VIRTUAL) {
            result += "virtual ";
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

class CSharpClassUnit : public ClassUnit
{
public:
    CSharpClassUnit(const std::string& name) : ClassUnit(name) {}

    std::string compile(unsigned int level = 0) const {
        std::string result = generateShift(level) + "class " + getName() + " {\n";

        for (size_t i = 0; i < ACCESS_MODIFIERS.size(); ++i) {
            if (getFields(i).empty()) {
                continue;
            }

            for(const auto& f : getFields(i)) {
                result += f->compile(level + 1);
            }
        }

        result += generateShift(level) + "};\n";
        return result;
    }
};

class CSharpPrintOperatorUnit : public PrintOperatorUnit {
public:
    explicit CSharpPrintOperatorUnit(const std::string& text) : PrintOperatorUnit(text) {}

    std::string compile(unsigned int level = 0) const {
        return generateShift(level) + "printf(\"" + getText() + "\");\n";
    }
};

class CSharpClassFactory : public ClassFactory {
public:
    std::unique_ptr<MethodUnit> createMethodUnit(const std::string& name, const std::string& returnType, Flags flags) const override {
        return std::unique_ptr<MethodUnit> (new CSharpMethodUnit(name, returnType, flags));
    }
    std::unique_ptr<ClassUnit> createClassUnit(const std::string& name) const override {
        return std::unique_ptr<ClassUnit> (new CSharpClassUnit(name));
    }
    std::unique_ptr<PrintOperatorUnit> createPrintOperatorUnit(const std::string& text) const override {
        return std::unique_ptr<PrintOperatorUnit> (new CSharpPrintOperatorUnit(text));
    }
};

#endif // CSHARP_H
