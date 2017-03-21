/* This file is part of the 'stringi' package for R.
 * Copyright (c) 2013-2017, Marek Gagolewski and other contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "stri_stringi.h"
#include "stri_container_utf16.h"
#include "stri_container_utf8.h"
#include "stri_container_regex.h"


/**
 * Detect if a pattern occurs in a string
 *
 * @param str R character vector
 * @param pattern R character vector containing regular expressions
 * @param omit_na single logical value
 * @param opts_regex list
 * @return character vector
 *
 * @version 0.3-1 (Bartek Tartanus, 2014-07-25)
 *
 * @version 0.3-1 (Marek Gagolewski, 2014-10-17)
 *                using std::vector<int> to avoid mem-leaks
 *
 * @version 0.3-1 (Marek Gagolewski, 2014-11-04)
 *    Issue #112: str_prepare_arg* retvals were not PROTECTed from gc
 *
 * @version 0.4-1 (Marek Gagolewski, 2014-12-04)
 *    FR #122: omit_na arg added
 *
 * @version 1.0-2 (Marek Gagolewski, 2016-01-29)
 *    Issue #214: allow a regex pattern like `.*`  to match an empty string
 *
 * @version 1.0-3 (Marek Gagolewski, 2016-02-03)
 *    FR #216: `negate` arg added
 */
SEXP stri_subset_regex(SEXP str, SEXP pattern, SEXP omit_na, SEXP negate, SEXP opts_regex)
{
   bool negate_1 = stri__prepare_arg_logical_1_notNA(negate, "negate");
   bool omit_na1 = stri__prepare_arg_logical_1_notNA(omit_na, "omit_na");
   PROTECT(str = stri_prepare_arg_string(str, "str"));
   PROTECT(pattern = stri_prepare_arg_string(pattern, "pattern"));
   R_len_t vectorize_length =
      stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));

   uint32_t pattern_flags = StriContainerRegexPattern::getRegexFlags(opts_regex);

   STRI__ERROR_HANDLER_BEGIN(2)
   StriContainerUTF16 str_cont(str, vectorize_length);
   StriContainerRegexPattern pattern_cont(pattern, vectorize_length, pattern_flags);

   // BT: this cannot be done with deque, because pattern is reused so i does not
   // go like 0,1,2...n but 0,pat_len,2*pat_len,1,pat_len+1 and so on
   // MG: agreed
   std::vector<int> which(vectorize_length);
   int result_counter = 0;

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      STRI__CONTINUE_ON_EMPTY_OR_NA_PATTERN(str_cont, pattern_cont,
         {if (omit_na1) which[i] = FALSE; else {which[i] = NA_LOGICAL; result_counter++;} })

      RegexMatcher *matcher = pattern_cont.getMatcher(i); // will be deleted automatically
      matcher->reset(str_cont.get(i));
      which[i] = (int)matcher->find();
      if (negate_1) which[i] = !which[i];
      if (which[i]) result_counter++;
   }

   SEXP ret;
   STRI__PROTECT(ret = stri__subset_by_logical(str_cont, which, result_counter));
   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/**
 * Substitutes vector elements if a pattern occurs in a string
 *
 * @param str character vector
 * @param pattern character vector
 * @param opts_regex list
 * @param value character vector
 * @return character vector
 *
 * @version 1.0-3 (Marek Gagolewski, 2016-02-03)
 *   FR#124
 *
 * @version 1.0-3 (Marek Gagolewski, 2016-02-03)
 *    FR #216: `negate` arg added
 */
SEXP stri_subset_regex_replacement(SEXP str, SEXP pattern, SEXP negate, SEXP opts_regex, SEXP value)
{
   bool negate_1 = stri__prepare_arg_logical_1_notNA(negate, "negate");
   PROTECT(str = stri_prepare_arg_string(str, "str"));
   PROTECT(pattern = stri_prepare_arg_string_1(pattern, "pattern"));
   PROTECT(value = stri_prepare_arg_string(value, "value"));

   int vectorize_length = LENGTH(str);
   int value_length = LENGTH(value);
   if (value_length == 0)
      Rf_error(MSG__REPLACEMENT_ZERO);

   uint32_t pattern_flags = StriContainerRegexPattern::getRegexFlags(opts_regex);
   UText* str_text = NULL; // may potentially be slower, but definitely is more convenient!

   STRI__ERROR_HANDLER_BEGIN(3)
   StriContainerUTF8 str_cont(str, vectorize_length);
   StriContainerUTF8 value_cont(value, value_length);
   StriContainerRegexPattern pattern_cont(pattern, vectorize_length, pattern_flags);

   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length));

   R_len_t k = 0;
   for (R_len_t i = str_cont.vectorize_init();
         i != str_cont.vectorize_end();
         i = str_cont.vectorize_next(i))
   {
      STRI__CONTINUE_ON_EMPTY_OR_NA_PATTERN(str_cont, pattern_cont,
      {SET_STRING_ELT(ret, i, NA_STRING);})

      UErrorCode status = U_ZERO_ERROR;
      RegexMatcher *matcher = pattern_cont.getMatcher(i); // will be deleted automatically
      str_text = utext_openUTF8(str_text, str_cont.get(i).c_str(), str_cont.get(i).length(), &status);
      STRI__CHECKICUSTATUS_THROW(status, {/* do nothing special on err */})
      matcher->reset(str_text);

      bool found = matcher->find();
      if ((found && !negate_1) || (!found && negate_1))
         SET_STRING_ELT(ret, i, value_cont.toR((k++)%value_length));
      else
         SET_STRING_ELT(ret, i, str_cont.toR(i));
   }

   if (str_text) {
      utext_close(str_text);
      str_text = NULL;
   }

   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END({
      if (str_text) {
         utext_close(str_text);
         str_text = NULL;
      }
   })
}
