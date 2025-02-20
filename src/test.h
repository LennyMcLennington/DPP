/************************************************************************************
 *
 * D++, A Lightweight C++ library for Discord
 *
 * Copyright 2021 Craig Edwards and D++ contributors 
 * (https://github.com/brainboxdotcc/DPP/graphs/contributors)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ************************************************************************************/
#undef DPP_BUILD
#ifdef _WIN32
_Pragma("warning( disable : 4251 )"); // 4251 warns when we export classes or structures with stl member variables
#endif
#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <dpp/fmt/format.h>

using json = nlohmann::json;

/* Represents a test case */
struct test_t {
	/* Description of test */
	std::string description;
	/* Has been executed */
	bool executed = false;
	/* Was successfully tested */
	bool success = false;
};

/* How long the unit tests can run for */
const int64_t TEST_TIMEOUT = 60;

/* IDs of various channels and guilds used to test */
const dpp::snowflake TEST_GUILD_ID = 907951970017480704;
const dpp::snowflake TEST_TEXT_CHANNEL_ID = 907951970017480707;
const dpp::snowflake TEST_VC_ID = 907951970017480708;
const dpp::snowflake TEST_USER_ID = 826535422381391913;

/**
 * @brief Perform a test of a REST base API call with one parameter
 */
#define singleparam_api_test(func_name, param, return_type, testname) \
	set_test(testname, false); \
	bot.func_name (param, [&](const dpp::confirmation_callback_t &cc) { \
		if (!cc.is_error()) { \
			return_type g = std::get<return_type>(cc.value); \
			if (g.id == param) { \
				set_test(testname, true); \
			} else { \
				bot.log(dpp::ll_debug, cc.http_info.body); \
				set_test(testname, false); \
			} \
		} else { \
			bot.log(dpp::ll_debug, cc.http_info.body); \
			set_test(testname, false); \
		} \
	});

/**
 * @brief Perform a test of a REST base API call with one parameter that returns a list
 */
#define singleparam_api_test_list(func_name, param, return_type, testname) \
	set_test(testname, false); \
	bot.func_name (param, [&](const dpp::confirmation_callback_t &cc) { \
		if (!cc.is_error()) { \
			return_type g = std::get<return_type>(cc.value); \
			if (g.size() > 0) { \
				set_test(testname, true); \
			} else { \
				bot.log(dpp::ll_debug, cc.http_info.body); \
				set_test(testname, false); \
			} \
		} else { \
			bot.log(dpp::ll_debug, cc.http_info.body); \
			set_test(testname, false); \
		} \
	});

/**
 * @brief Perform a test of a REST base API call with no parameters
 */
#define noparam_api_test(func_name, return_type, testname) \
	set_test(testname, false); \
	bot.func_name ([&](const dpp::confirmation_callback_t &cc) { \
		if (!cc.is_error()) { \
			return_type g = std::get<return_type>(cc.value); \
			set_test(testname, true); \
		} else { \
			bot.log(dpp::ll_debug, cc.http_info.body); \
			set_test(testname, false); \
		} \
	});

/**
 * @brief Sets a test's status
 * 
 * @param testname test name (key) to set the status of
 * @param success If set to true, sets success to true, if set to false and called
 * once, sets executed to true, if called twice, also sets success to false.
 * This means that before you run the test you should call this function once
 * with success set to false, then if/wen the test completes call it again with true.
 * If the test fails, call it a second time with false, or not at all.
 */
void set_test(const std::string &testname, bool success = false);

/**
 * @brief Prints a summary of all tests executed
 * @param tests List of tests executed
 * 
 * @return int Returns number of failed tests, for use as a return value from the main() function
 */
int test_summary();


/**
 * @brief Load test audio for the voice channel tests
 * 
 * @return std::vector<uint8_t> data and size for test audio
 */
std::vector<uint8_t> load_test_audio();

/**
 * @brief Get the token from the environment variable DPP_UNIT_TEST_TOKEN
 * 
 * @return std::string token
 * @note If the environment variable does not exist, this will exit the program.
 */
std::string get_token();

/**
 * @brief Wait for all tests to be completed or test program to time out
 */
void wait_for_tests();

/**
 * @brief Get the start time of tests
 * 
 * @return double start time in fractional seconds
 */
double get_start_time();

/**
 * @brief Get the current execution time in seconds
 * 
 * @return double fractional seconds
 */
double get_time();