#pragma once

#include <AK/AKString.h>
#include <AK/NonnullRefPtr.h>

#include "Word.h"

class RedirectionOperand : public RefCounted<RedirectionOperand> {
public:
    virtual ~RedirectionOperand();

    enum class Type {
        Invalid,
        Word,
    };

    Type type() const { return m_type; }

    virtual String to_string() const = 0;

protected:
    explicit RedirectionOperand(Type);

private:
    Type m_type { Type::Invalid };
};

class WordRedirectionOperand : public RedirectionOperand {
public:
    static NonnullRefPtr<WordRedirectionOperand> create(NonnullRefPtr<Word>& word)
    {
        return adopt(*new WordRedirectionOperand(word));
    }
    virtual ~WordRedirectionOperand() override {}

    String to_string() const override { return m_word->to_string(); }

private:
    explicit WordRedirectionOperand(NonnullRefPtr<Word>& word)
        : RedirectionOperand(Type::Word)
        , m_word(word)
    {
    }

    NonnullRefPtr<Word>& m_word;
};
