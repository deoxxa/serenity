#pragma once

#include <AK/AKString.h>
#include <AK/NonnullRefPtr.h>
#include <AK/NonnullRefPtrVector.h>

#include "RedirectionOperand.h"

class Redirection : public RefCounted<Redirection> {
public:
    enum class Type {
        Invalid,
        Input,
        Output,
        OutputAppend,
    };

    static NonnullRefPtr<Redirection> create(int fd, Type type, NonnullRefPtr<RedirectionOperand>& operand)
    {
        return adopt(*new Redirection(fd, type, operand));
    }

    ~Redirection();

    int fd() const { return m_fd; }
    Type type() const { return m_type; }
    NonnullRefPtr<RedirectionOperand>& operand() const { return m_operand; }

    String to_string() const;

private:
    explicit Redirection(int, Type, NonnullRefPtr<RedirectionOperand>&);

    int m_fd { -1 };
    Type m_type { Type::Invalid };
    NonnullRefPtr<RedirectionOperand>& m_operand;
};
