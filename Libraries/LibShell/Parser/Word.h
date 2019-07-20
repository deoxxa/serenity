#pragma once

#include <AK/AKString.h>
#include <AK/NonnullRefPtr.h>
#include <AK/NonnullRefPtrVector.h>

class Word : public RefCounted<Word> {
public:
    virtual ~Word();

    enum class Type {
        Invalid,
        Bare,
        SingleQuotedString,
        DoubleQuotedString,
    };

    Type type() const { return m_type; }

    virtual String to_string() const = 0;

protected:
    explicit Word(Type);

private:
    Type m_type { Type::Invalid };
};

class BareWord : public Word {
public:
    static NonnullRefPtr<BareWord> create(const String& string)
    {
        return adopt(*new BareWord(string));
    }
    virtual ~BareWord() override {}

    String to_string() const override { return m_string; }

private:
    explicit BareWord(const String& string)
        : Word(Type::Bare)
        , m_string(string)
    {
    }

    String m_string;
};

class SingleQuotedStringWord : public Word {
public:
    static NonnullRefPtr<SingleQuotedStringWord> create(const String& string)
    {
        return adopt(*new SingleQuotedStringWord(string));
    }
    virtual ~SingleQuotedStringWord() override {}

    String to_string() const override { return String::format("'%s'", m_string.characters()); }

private:
    explicit SingleQuotedStringWord(const String& string)
        : Word(Type::SingleQuotedString)
        , m_string(string)
    {
    }

    String m_string;
};

class DoubleQuotedStringWord : public Word {
public:
    static NonnullRefPtr<DoubleQuotedStringWord> create(const String& string)
    {
        return adopt(*new DoubleQuotedStringWord(string));
    }
    virtual ~DoubleQuotedStringWord() override {}

    String to_string() const override { return String::format("\"%s\"", m_string.characters()); }

private:
    explicit DoubleQuotedStringWord(const String& string)
        : Word(Type::DoubleQuotedString)
        , m_string(string)
    {
    }

    String m_string;
};

