#pragma once

#include <AK/NonnullRefPtr.h>
#include <AK/NonnullRefPtrVector.h>

#include "Command.h"

class Program : public RefCounted<Program> {
public:
    static NonnullRefPtr<Program> create(NonnullRefPtrVector<Command>&& commands)
    {
        return adopt(*new Program(move(commands)));
    }

    ~Program();

    const NonnullRefPtrVector<Command>& commands() const { return m_commands; }

    String to_string() const;

private:
    explicit Program(NonnullRefPtrVector<Command>&&);

    NonnullRefPtrVector<Command> m_commands;
};
