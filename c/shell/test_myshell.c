#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "myshell.h"

// Test case for basic command parsing
START_TEST(test_parse_input_basic) {
    char input[MAX_INPUT] = "ls -l";
    char *args[MAX_ARGS];
    char *input_file, *output_file;
    int append;

    int argc = parse_input(input, args, &input_file, &output_file, &append);

    ck_assert_int_eq(argc, 2);
    ck_assert_str_eq(args[0], "ls");
    ck_assert_str_eq(args[1], "-l");
    ck_assert_ptr_null(args[2]);
    ck_assert_ptr_null(input_file);
    ck_assert_ptr_null(output_file);
    ck_assert_int_eq(append, 0);
}
END_TEST

// Test case for input redirection
START_TEST(test_parse_input_input_redirect) {
    char input[MAX_INPUT] = "cat < input.txt";
    char *args[MAX_ARGS];
    char *input_file, *output_file;
    int append;

    int argc = parse_input(input, args, &input_file, &output_file, &append);

    ck_assert_int_eq(argc, 1);
    ck_assert_str_eq(args[0], "cat");
    ck_assert_ptr_null(args[1]);
    ck_assert_str_eq(input_file, "input.txt");
    ck_assert_ptr_null(output_file);
    ck_assert_int_eq(append, 0);
}
END_TEST

// Test case for output redirection (overwrite)
START_TEST(test_parse_input_output_redirect) {
    char input[MAX_INPUT] = "ls -l > output.txt";
    char *args[MAX_ARGS];
    char *input_file, *output_file;
    int append;

    int argc = parse_input(input, args, &input_file, &output_file, &append);

    ck_assert_int_eq(argc, 2);
    ck_assert_str_eq(args[0], "ls");
    ck_assert_str_eq(args[1], "-l");
    ck_assert_ptr_null(args[2]);
    ck_assert_ptr_null(input_file);
    ck_assert_str_eq(output_file, "output.txt");
    ck_assert_int_eq(append, 0);
}
END_TEST

// Test case for output redirection (append)
START_TEST(test_parse_input_append_redirect) {
    char input[MAX_INPUT] = "echo hello >> log.txt";
    char *args[MAX_ARGS];
    char *input_file, *output_file;
    int append;

    int argc = parse_input(input, args, &input_file, &output_file, &append);

    ck_assert_int_eq(argc, 2);
    ck_assert_str_eq(args[0], "echo");
    ck_assert_str_eq(args[1], "hello");
    ck_assert_ptr_null(args[2]);
    ck_assert_ptr_null(input_file);
    ck_assert_str_eq(output_file, "log.txt");
    ck_assert_int_eq(append, 1);
}
END_TEST

// Test case for empty input
START_TEST(test_parse_input_empty) {
    char input[MAX_INPUT] = "";
    char *args[MAX_ARGS];
    char *input_file, *output_file;
    int append;

    int argc = parse_input(input, args, &input_file, &output_file, &append);

    ck_assert_int_eq(argc, 0);
    ck_assert_ptr_null(args[0]);
    ck_assert_ptr_null(input_file);
    ck_assert_ptr_null(output_file);
    ck_assert_int_eq(append, 0);
}
END_TEST

// Test case for missing redirection filename
START_TEST(test_parse_input_missing_redirect_file) {
    char input[MAX_INPUT] = "ls >";
    char *args[MAX_ARGS];
    char *input_file, *output_file;
    int append;

    int argc = parse_input(input, args, &input_file, &output_file, &append);

    ck_assert_int_eq(argc, 1);
    ck_assert_str_eq(args[0], "ls");
    ck_assert_ptr_null(args[1]);
    ck_assert_ptr_null(input_file);
    ck_assert_ptr_null(output_file);
    ck_assert_int_eq(append, 0);
}
END_TEST

// Test suite setup
Suite *parse_input_suite(void) {
    Suite *s = suite_create("Parse Input");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_parse_input_basic);
    tcase_add_test(tc_core, test_parse_input_input_redirect);
    tcase_add_test(tc_core, test_parse_input_output_redirect);
    tcase_add_test(tc_core, test_parse_input_append_redirect);
    tcase_add_test(tc_core, test_parse_input_empty);
    tcase_add_test(tc_core, test_parse_input_missing_redirect_file);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    Suite *s = parse_input_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);  // Run all tests, normal verbosity
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
