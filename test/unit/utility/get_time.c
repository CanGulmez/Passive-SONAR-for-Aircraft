/**
 ******************************************************************************
 * @file 	get_time.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	Unit test for get_time() function.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2026 Ahmet Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 ******************************************************************************
 */

#include "../../../src/main.h"

START_TEST(get_time_default_format)
{
	printf("\n[TEST] Testing get_time() with default format...\n");

	char *res = get_time(NULL);

	ck_assert_ptr_nonnull(res);
	ck_assert_int_gt(strlen(res), 0);

	for (size_t i = 0; i < strlen(res); i++)
	{
		ck_assert(res[i] != '\0');
	}

	printf("Passed.\n");
}
END_TEST

START_TEST(get_time_custom_format)
{
	printf("\n[TEST] Testing get_time() with custom format...\n");	

	char *res = get_time("%Y-%m-%d");

	ck_assert_ptr_nonnull(res);
	ck_assert_int_gt(strlen(res), 10);

	for (size_t i = 0; i < strlen(res); i++)
	{
		ck_assert(res[i] != '\0');
	}

	printf("Passed.\n");
}

Suite *get_time_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("Time");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, get_time_default_format);
	tcase_add_test(tc_core, get_time_custom_format);

	suite_add_tcase(s, tc_core);

	return s;
}

int main(int argc, char *argv[])
{
	int numFailed = 0;
	Suite *s;
	SRunner *sr;

	s = get_time_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);

	numFailed = srunner_ntests_failed(sr);

	srunner_free(sr);

	printf("numFailed = %d\n", numFailed);

	return EXIT_SUCCESS;
}
