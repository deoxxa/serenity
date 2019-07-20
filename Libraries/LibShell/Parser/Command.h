#pragma once

#include <AK/NonnullRefPtr.h>
#include <AK/NonnullRefPtrVector.h>

#include "Redirection.h"
#include "Word.h"

class Command : public RefCounted<Command> {
public:
    static NonnullRefPtr<Command> create(NonnullRefPtrVector<Word>&& words, NonnullRefPtrVector<Redirection>&& redirections)
    {
        return adopt(*new Command(move(words), move(redirections)));
    }

    ~Command();

    const NonnullRefPtrVector<Word>& words() const { return m_words; }
    const NonnullRefPtrVector<Redirection>& redirections() const { return m_redirections; }

    String to_string() const;

private:
    explicit Command(NonnullRefPtrVector<Word>&&, NonnullRefPtrVector<Redirection>&&);

    NonnullRefPtrVector<Word> m_words;
    NonnullRefPtrVector<Redirection> m_redirections;
};
