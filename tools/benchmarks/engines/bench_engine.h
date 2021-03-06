// Copyright (C) 2013 Alexandre Rames <alexandre@coreperf.com>

// rejit is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef BENCH_ENGINE_H_
#define BENCH_ENGINE_H_

#include <iostream>
#include <vector>

#include <argp.h>


#ifdef BENCH_ENGINE_REJIT
#include "rejit.h"
#include "checks.h"
#include "flags.h"
using namespace rejit;
#endif


using namespace std;


// Start the enum from the latest argp key used.
enum rejit_flags_option_keys {
  // Hope it does not collide with other keys.
  base_rejit_flag_key = 0x7BAD,
#ifdef BENCH_ENGINE_REJIT
#define ENUM_KEYS(flag_name, r, d) flag_name##_key,
  REJIT_FLAGS_LIST(ENUM_KEYS)
#undef ENUM_KEYS
#endif
  after_last_rejit_key,
  first_rejit_flag_key = base_rejit_flag_key + 1
};
#define REJIT_FLAG_OFFSET(flag_name) (flag_name##_key - first_rejit_flag_key)


struct arguments {
  char      *args[1];  // The regexp.
  char      *file;
  vector<size_t> sizes;
  unsigned  iterations;
  char      low_char;
  char      high_char;
  int       run_worst_case;
#ifdef BENCH_ENGINE_REJIT
  int  rejit_flags;
#endif
};

extern struct argp_option options[];
extern char args_doc[];
extern char doc[];
extern struct argp argp;
extern const char *argp_program_bug_address;

error_t parse_opt(int key, char *arg, struct argp_state *state);

void handle_arguments(struct arguments *arguments,
                      struct argp *argp,
                      int argc,
                      char *argv[]);

void prepare_text(struct arguments *arguments, string *text);

struct bench_res {
  size_t text_size;
  double worse;
  double amortised;
  double best;
};

void print_results(vector<bench_res> *results, int print_worst_case);

double speed(struct timeval *t0, struct timeval *t1, size_t text_size, unsigned times);

void error(const char* message, int rc = 1);


#endif
