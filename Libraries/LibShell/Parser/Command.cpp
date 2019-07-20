#include <AK/StringBuilder.h>

#include "Command.h"
#include "Word.h"

Command::Command(NonnullRefPtrVector<Word>&& words, NonnullRefPtrVector<Redirection>&& redirections)
    : m_words(words)
    , m_redirections(redirections)
{
}

Command::~Command()
{
}

String Command::to_string() const
{
    StringBuilder sb;

    bool first = true;
    for (auto& word : m_words) {
        if (!first)
            sb.append(" ");

        sb.append(word.to_string());

        first = false;
    }

    for (auto& redirection : m_redirections) {
        sb.append(" ");

        sb.append(redirection.to_string());
    }

    return sb.to_string();
}
