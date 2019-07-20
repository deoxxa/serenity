#include <ctype.h>
#include <stdio.h>

#include <AK/AKString.h>
#include <AK/RefPtr.h>
#include <AK/Vector.h>

#include "Command.h"
#include "Program.h"
#include "Redirection.h"
#include "RedirectionOperand.h"
#include "Word.h"

#include "Parser.h"

RefPtr<Program> parse(const String& line)
{
    Vector<char> buffer;

    int index = 0;

    auto eof = [&]() -> bool {
        return index == line.length();
    };

    auto peek = [&]() -> char {
        if (!eof())
            return line[index];
        return 0;
    };

    auto consume_one = [&]() -> char {
        if (!eof())
            return line[index++];
        return 0;
    };

    auto consume_specific = [&](char ch) {
        ASSERT(!eof());
        ASSERT(peek() == ch);
        ++index;
        return ch;
    };

    auto consume_whitespace = [&] {
        while (isspace(peek()))
            ++index;
    };

    auto parse_single_quoted_string = [&]() -> RefPtr<SingleQuotedStringWord> {
        consume_specific('\'');

        buffer.clear();

        while (peek() != '\'' && !eof())
            buffer.append(consume_one());

        if (eof())
            return nullptr;

        consume_specific('\'');

        RefPtr<SingleQuotedStringWord> word = SingleQuotedStringWord::create(String::copy(buffer));

        buffer.clear();

        return word;
    };

    auto parse_double_quoted_string = [&]() -> RefPtr<DoubleQuotedStringWord> {
        consume_specific('"');

        buffer.clear();

        bool escape = false;
        while (!eof()) {
            if (escape) {
                auto ch = consume_one();

                if (ch != '$' && ch != '`' && ch != '"' && ch != '\\' && ch != '\n')
                    buffer.append('\\');

                buffer.append(ch);

                escape = false;
            } else if (peek() == '\\') {
                consume_specific('\\');

                escape = true;
            } else if (peek() == '"') {
                break;
            } else {
                buffer.append(consume_one());
            }
        }

        if (eof())
            return nullptr;

        consume_specific('"');

        RefPtr<DoubleQuotedStringWord> word = DoubleQuotedStringWord::create(String::copy(buffer));

        buffer.clear();

        return word;
    };

    auto is_valid_operator_char = [&](char ch) -> bool {
        return ch == '|' || ch == '&' || ch == ';' || ch == '<' || ch == '>';
    };

    auto is_valid_word_char = [&](char ch) {
        return !isspace(ch) && !is_valid_operator_char(ch);
    };

    auto parse_bare_word = [&]() -> NonnullRefPtr<Word> {
        ASSERT(is_valid_word_char(peek()));

        buffer.clear();

        bool escape = false;
        while (!eof()) {
            if (escape) {
                auto ch = consume_one();

                if (ch != '\n')
                    buffer.append(ch);

                escape = false;
            } else if (peek() == '\\') {
                consume_one();

                escape = true;
            } else if (is_valid_word_char(peek())) {
                buffer.append(consume_one());
            } else {
                break;
            }
        }

        auto word = BareWord::create(String::copy(buffer));

        buffer.clear();

        return word;
    };

    auto parse_redirection_operand = [&]() -> RefPtr<RedirectionOperand> {
        consume_whitespace();

        if (!is_valid_word_char(peek()))
            return nullptr;

        auto word = parse_bare_word();

        return WordRedirectionOperand::create(word);
    };

    auto parse_lt = [&]() -> RefPtr<Redirection> {
        consume_specific('<');

        auto redirection_operand = parse_redirection_operand();
        if (!redirection_operand)
            return nullptr;

        NonnullRefPtr<RedirectionOperand> oper = *redirection_operand;

        return Redirection::create(-1, Redirection::Type::Input, oper);
    };

    auto parse_gt = [&]() -> RefPtr<Redirection> {
        consume_specific('>');
        bool append = false;

        if (peek() == '>') {
            consume_specific('>');
            append = true;
        }

        auto redirection_operand = parse_redirection_operand();
        if (!redirection_operand)
            return nullptr;

        NonnullRefPtr<RedirectionOperand> oper = *redirection_operand;

        return Redirection::create(
            -1,
            append ? Redirection::Type::Output : Redirection::Type::OutputAppend,
            oper
        );
    };

    auto parse_command = [&]() -> RefPtr<Command> {
        Vector<NonnullRefPtr<Word>> words;
        Vector<NonnullRefPtr<Redirection>> redirections;

        while (!eof()) {
            consume_whitespace();

            if (peek() == '\'') {
                auto word = parse_single_quoted_string();
                if (!word)
                    return nullptr;

                words.append(*word);
            } else if (peek() == '"') {
                auto word = parse_double_quoted_string();
                if (!word)
                    return nullptr;

                words.append(*word);
            } else if (peek() == '<') {
                auto redirection = parse_lt();
                if (!redirection)
                    return nullptr;

                redirections.append(*redirection);
            } else if (peek() == '>') {
                auto redirection = parse_gt();
                if (!redirection)
                    return nullptr;

                redirections.append(*redirection);
            } else if (is_valid_word_char(peek())) {
                auto word = parse_bare_word();
                if (!word)
                    return nullptr;

                words.append(word);
            } else {
                return nullptr;
            }
        }

        return Command::create(words, redirections);
    };

    Vector<NonnullRefPtr<Command>> commands;

    while (!eof()) {
        auto command = parse_command();
        if (!command)
            return nullptr;

        commands.append(*command);
    }

    return Program::create(commands);
}
