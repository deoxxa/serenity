#include <AK/ByteBuffer.h>
#include <AK/String.h>
#include <AK/StringBuilder.h>
#include <LibCore/CFile.h>
#include <LibGUI/GClipboard.h>
#include <LibGUI/GEventLoop.h>
#include <getopt.h>

struct Options {
    String data;
    String type { "text" };
};

void print_usage(FILE* stream, const char* argv0)
{
    fprintf(
        stream,
        "Usage:\n"
        "\t%s [--type type] text\n"
        "\t%s [--type type] < file\n"
        "\n"
        "\t-t type, --type type\tPick a type.\n"
        "\t-h, --help\t\tPrint this help message.\n",
        argv0,
        argv0);
}

Options parse_options(int argc, char* argv[])
{
    Options options;

    static struct option long_options[] = {
        { "type", required_argument, 0, 't' },
        { "help", no_argument, 0, 'h' },
        { 0, 0, 0, 0 }
    };
    while (true) {
        int option_index;
        int c = getopt_long(argc, argv, "t:h", long_options, &option_index);
        if (c == -1)
            break;
        if (c == 0)
            c = long_options[option_index].val;

        switch (c) {
        case 't':
            options.type = optarg;
            break;
        case 'h':
            print_usage(stdout, argv[0]);
            exit(0);
        default:
            print_usage(stderr, argv[0]);
            exit(1);
        }
    }

    if (optind < argc) {
        // Copy the rest of our command-line args.
        StringBuilder builder;
        bool first = true;
        for (int i = optind; i < argc; i++) {
            if (!first)
                builder.append(' ');
            first = false;
            builder.append(argv[i]);
        }
        options.data = builder.to_string();
    } else {
        // Copy our stdin.
        CFile c_stdin;
        bool success = c_stdin.open(
            STDIN_FILENO,
            CIODevice::OpenMode::ReadOnly,
            CFile::ShouldCloseFileDescription::No);
        ASSERT(success);
        auto buffer = c_stdin.read_all();
        dbg() << "Read size " << buffer.size();
        options.data = String((char*)buffer.data(), buffer.size());
    }

    return options;
}

int main(int argc, char* argv[])
{
    Options options = parse_options(argc, argv);

    new GEventLoop;

    GClipboard& clipboard = GClipboard::the();
    clipboard.set_data(options.data, options.type);
}
