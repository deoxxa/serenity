#include <stdio.h>
#include <stdlib.h>

#include <AK/AKString.h>
#include <AK/Vector.h>

#include <LibShell/Parser/Parser.h>

int main(int argc, char **argv)
{
  if (argc >= 2) {
    String str(argv[1]);
    printf("going to parse: %s\n", str.characters());
    auto program = parse(str);
    if (!program)
      return 1;
    printf("result: %s\n", program->to_string().characters());
    return 0;
  }

  struct TestCase {
    String name;
    String input;
    String output;
    bool fail;
  };

  Vector<TestCase> test_cases;

  test_cases.append(TestCase{"one word", "one", "one", false});
  test_cases.append(TestCase{"two words", "two words", "two words", false});
  test_cases.append(TestCase{"single quoted string", "'single quoted'", "'single quoted'", false});
  test_cases.append(TestCase{"unclosed single quoted string", "'single quoted", "", true});
  test_cases.append(TestCase{"double quoted string", "\"double quoted\"", "\"double quoted\"", false});
  test_cases.append(TestCase{"unclosed double quoted string", "\"double quoted", "", true});
  test_cases.append(TestCase{"collapse spaces", "a     b", "a b", false});
  test_cases.append(TestCase{"escaped space", "a\\ b", "a b", false});
  test_cases.append(TestCase{"line continuation", "a\\\nb\\\nc", "abc", false});
  test_cases.append(TestCase{"redirect to file", "command > file", "command > file", false});
  test_cases.append(TestCase{"append to file", "command >> file", "command >> file", false});
  test_cases.append(TestCase{"redirect from file", "command < file", "command < file", false});
  test_cases.append(TestCase{"redirect to and from files", "command < file1 > file2", "command < file1 > file2", false});

  bool failed = false;

  printf("1..%d\n", test_cases.size());

  int i = 0;
  for (auto test_case : test_cases) {
    i++;

    auto program = parse(test_case.input);
    if (test_case.fail) {
      if (!program) {
        printf("ok %d - %s\n", i, test_case.name.characters());
      } else {
        printf("not ok %d - %s\n", i, test_case.name.characters());
        printf("  parse should have failed\n");
        failed = true;
      }
    } else {
      if (program) {
        auto str = program->to_string();

        if (str == test_case.output) {
          printf("ok %d - %s\n", i, test_case.name.characters());
        } else {
          printf("not ok %d - %s\n", i, test_case.name.characters());
          printf("  expected: %s\n  actual:  %s\n", test_case.output.characters(), str.characters());
          failed = true;
        }
      } else {
        printf("not ok %d - %s\n", i, test_case.name.characters());
        printf("  parse failed\n");
        failed = true;
      }
    }
  }

  if (failed)
    return 1;

  return 0;
}
