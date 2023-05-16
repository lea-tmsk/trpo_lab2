#ifndef UNIT_H
#define UNIT_H
#include <iostream>
#include <memory>
#include <vector>

class Unit {
public:
    using Flags = unsigned int;
    virtual ~Unit() = default;

    virtual void add(const std::shared_ptr<Unit>&) {
        throw std::runtime_error("Not supported");
    }
    virtual std::string compile(unsigned int level = 0) const = 0;
    virtual Flags getFlags() const {
        return 0;
    }

protected:
    virtual std::string generateShift(unsigned int level) const {
            static const auto DEFAULT_SHIFT = "    ";
            std::string result;
            for (unsigned int i = 0; i < level; ++i) {
            result += DEFAULT_SHIFT;
        }
        return result;
    }
};

class MethodUnit : public Unit {
public:
    enum Modifier {
        STATIC = 1,
        CONST = 1 << 1,
        VIRTUAL = 1 << 2,
        FINAL = 1 << 3,
        ABSTRACT = 1 << 4,
        PUBLIC = 1 << 5,
        PROTECTED = 1 << 6,
        PRIVATE = 1 << 7,
        PRIVATE_PROTECTED = 1 << 8,
        PROTECTED_INTERNAL = 1 << 9,
        INTERNAL = 1 << 10
    };
    MethodUnit(const std::string& name, const std::string& returnType, Flags flags) :
    m_name(name), m_returnType(returnType), m_flags(flags) {};

    void add(const std::shared_ptr<Unit>& unit) {
        m_body.push_back(unit);
    }

protected:
    std::string getName() const {
        return m_name;
    }
    std::string getReturnType() const {
        return m_returnType;
    }
    Flags getFlags() const {
        return m_flags;
    }
    std::vector<std::shared_ptr<Unit>> getBody() const {
        return m_body;
    }

private:
    std::string m_name;
    std::string m_returnType;
    Flags m_flags;
    std::vector<std::shared_ptr<Unit>> m_body;
};

class ClassUnit : public Unit
{
public:
    enum AccessModifier {
            PUBLIC,
            PROTECTED,
            PROTECTED_INTERNAL,
            INTERNAL,
            PRIVATE,
            PRIVATE_PROTECTED
        };

    static const std::vector<std::string> ACCESS_MODIFIERS;
    using Fields = std::vector<std::shared_ptr<Unit>>;

    explicit ClassUnit(const std::string& name) : m_name(name) {
        m_fields.resize(ACCESS_MODIFIERS.size());
    }
    void add(const std::shared_ptr<Unit>& unit) {
        int accessModifier = PRIVATE;
        Flags flags = unit->getFlags();
        if (flags & MethodUnit::PUBLIC) {
            accessModifier = PUBLIC;
        } else if (flags & MethodUnit::PROTECTED) {
            accessModifier = PRIVATE;
        } else if (flags & MethodUnit::PRIVATE_PROTECTED) {
            accessModifier = PRIVATE_PROTECTED;
        } else if (flags & MethodUnit::INTERNAL) {
            accessModifier = INTERNAL;
        } else if (flags & MethodUnit::PROTECTED_INTERNAL) {
            accessModifier = PROTECTED_INTERNAL;
        }

        m_fields[accessModifier].push_back(unit);
    }

protected:
    std::string getName() const {
        return m_name;
    }

    const Fields& getFields(unsigned int accessGroup) const {
        if (ACCESS_MODIFIERS.size() <= accessGroup) {
            throw std::out_of_range("Invalid access group index");
        }
        return m_fields[accessGroup];
    }

private:
    std::string m_name;
    std::vector<Fields> m_fields;
};

const std::vector<std::string> ClassUnit::ACCESS_MODIFIERS = {"public", "protected", "protected internal", "internal", "private", "private protected"};

class PrintOperatorUnit : public Unit {
public:    
    explicit PrintOperatorUnit(const std::string& text) : m_text(text) {}

protected:
    std::string getText() const {
        return m_text;
    }
private:
    std::string m_text;
};

class ClassFactory {
public:
    virtual ~ClassFactory() = default;
    using Flags = unsigned int;

    virtual std::unique_ptr<MethodUnit> createMethodUnit(const std::string& name, const std::string& returnType, Flags flags) const = 0;
    virtual std::unique_ptr<ClassUnit> createClassUnit(const std::string& name) const = 0;
    virtual std::unique_ptr<PrintOperatorUnit> createPrintOperatorUnit(const std::string& text) const = 0;
};

#endif // UNIT_H
