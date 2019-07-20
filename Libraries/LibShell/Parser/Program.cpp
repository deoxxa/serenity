#include <AK/StringBuilder.h>

#include "Command.h"
#include "Program.h"

Program::Program(NonnullRefPtrVector<Command>&& commands)
    : m_commands(commands)
{
}

Program::~Program()
{
}

String Program::to_string() const
{
    StringBuilder sb;

    bool first = true;
    for (auto& command : m_commands) {
        if (!first)
            sb.append("; ");

        sb.append(command.to_string());

        first = false;
    }

    return sb.to_string();
}
