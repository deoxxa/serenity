#include <AK/StringBuilder.h>
#include <AK/NonnullRefPtr.h>

#include "Redirection.h"
#include "RedirectionOperand.h"

Redirection::Redirection(int fd, Type type, NonnullRefPtr<RedirectionOperand>& operand)
    : m_fd(fd)
    , m_type(type)
    , m_operand(operand)
{
}

Redirection::~Redirection()
{
}

String Redirection::to_string() const
{
    StringBuilder sb;

    if (m_fd != -1)
        sb.append("%d", m_fd);

    switch (m_type) {
    case Type::Input:
        sb.append("<");
        break;
    case Type::Output:
        sb.append(">");
        break;
    case Type::OutputAppend:
        sb.append(">>");
        break;
    case Type::Invalid:
        ASSERT_NOT_REACHED();
    }

    sb.append(m_operand->to_string());

    return sb.to_string();
}
