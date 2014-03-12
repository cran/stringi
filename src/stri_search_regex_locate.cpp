/* This file is part of the 'stringi' package for R.
 * Copyright (c) 2013-2014, Marek Gagolewski and Bartek Tartanus
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




/** Locate all occurences of a regex pattern
 *
 * @param str character vector
 * @param pattern character vector
 * @param opts_regex list
 * @return list of integer matrices (2 columns)
 *
 * @version 0.1 (Bartek Tartanus)
 * @version 0.2 (Marek Gagolewski) - StriContainerUTF16+deque usage
 * @version 0.3 (Marek Gagolewski, 2013-06-19) use StriContainerRegexPattern + opts_regex
 */
SEXP stri_locate_all_regex(SEXP str, SEXP pattern, SEXP opts_regex)
{
   str = stri_prepare_arg_string(str, "str"); // prepare string argument
   pattern = stri_prepare_arg_string(pattern, "pattern"); // prepare string argument
   R_len_t vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));

   uint32_t pattern_flags = StriContainerRegexPattern::getRegexFlags(opts_regex);

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF16 str_cont(str, vectorize_length);
   StriContainerRegexPattern pattern_cont(pattern, vectorize_length, pattern_flags);

   SEXP ret;
   PROTECT(ret = Rf_allocVector(VECSXP, vectorize_length));

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      STRI__CONTINUE_ON_EMPTY_OR_NA_STR_PATTERN(str_cont, pattern_cont,
         SET_VECTOR_ELT(ret, i, stri__matrix_NA_INTEGER(1, 2));,
         SET_VECTOR_ELT(ret, i, stri__matrix_NA_INTEGER(1, 2));)

      RegexMatcher *matcher = pattern_cont.getMatcher(i); // will be deleted automatically
      matcher->reset(str_cont.get(i));
      int found = (int)matcher->find();
      if (!found) {
         SET_VECTOR_ELT(ret, i, stri__matrix_NA_INTEGER(1, 2));
         continue;
      }

      deque<R_len_t_x2> occurences;
      do {
         UErrorCode status = U_ZERO_ERROR;
         int start = (int)matcher->start(status);
         int end  =  (int)matcher->end(status);
         if (U_FAILURE(status)) throw StriException(status);

         occurences.push_back(R_len_t_x2(start, end));
         found = (int)matcher->find();
      } while (found);

      R_len_t noccurences = (R_len_t)occurences.size();
      SEXP ans;
      PROTECT(ans = Rf_allocMatrix(INTSXP, noccurences, 2));
      int* ans_tab = INTEGER(ans);
      deque<R_len_t_x2>::iterator iter = occurences.begin();
      for (R_len_t j = 0; iter != occurences.end(); ++iter, ++j) {
         R_len_t_x2 match = *iter;
         ans_tab[j]             = match.v1;
         ans_tab[j+noccurences] = match.v2;
      }

      // Adjust UChar index -> UChar32 index (1-2 byte UTF16 to 1 byte UTF32-code points)
      str_cont.UChar16_to_UChar32_index(i, ans_tab,
            ans_tab+noccurences, noccurences,
            1, // 0-based index -> 1-based
            0  // end returns position of next character after match
      );
      SET_VECTOR_ELT(ret, i, ans);
      UNPROTECT(1);
   }

   stri__locate_set_dimnames_list(ret);
   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}



/** Locate first occurence of a regex pattern
 *
 * @param str character vector
 * @param pattern character vector
 * @param opts_regex list
 * @param firs logical - search for the first or the last occurence?
 * @return list of integer matrices (2 columns)
 *
 * @version 0.1 (Bartek Tartanus)
 * @version 0.2 (Marek Gagolewski) - StriContainerUTF16
 * @version 0.3 (Marek Gagolewski, 2013-06-19) use StriContainerRegexPattern + opts_regex
 */
SEXP stri__locate_firstlast_regex(SEXP str, SEXP pattern, SEXP opts_regex, bool first)
{
   str = stri_prepare_arg_string(str, "str"); // prepare string argument
   pattern = stri_prepare_arg_string(pattern, "pattern"); // prepare string argument
   R_len_t vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));

   uint32_t pattern_flags = StriContainerRegexPattern::getRegexFlags(opts_regex);

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF16 str_cont(str, vectorize_length);
   StriContainerRegexPattern pattern_cont(pattern, vectorize_length, pattern_flags);

   SEXP ret;
   PROTECT(ret = Rf_allocMatrix(INTSXP, vectorize_length, 2));
   int* ret_tab = INTEGER(ret);

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      ret_tab[i]                  = NA_INTEGER;
      ret_tab[i+vectorize_length] = NA_INTEGER;
      STRI__CONTINUE_ON_EMPTY_OR_NA_STR_PATTERN(str_cont, pattern_cont, ;/*nothing*/, ;/*nothing*/)

      RegexMatcher *matcher = pattern_cont.getMatcher(i); // will be deleted automatically
      matcher->reset(str_cont.get(i));

      if ((int)matcher->find()) { //find first matches
         UErrorCode status = U_ZERO_ERROR;
         ret_tab[i] = (int)matcher->start(status);
         if (U_FAILURE(status)) throw StriException(status);
         ret_tab[i+vectorize_length] = (int)matcher->end(status);
         if (U_FAILURE(status)) throw StriException(status);
      }
      else
         continue; // no match

      if (!first) { // continue searching
         while ((int)matcher->find()) {
            UErrorCode status = U_ZERO_ERROR;
            ret_tab[i]                  = (int)matcher->start(status);
            if (U_FAILURE(status)) throw StriException(status);
            ret_tab[i+vectorize_length] = (int)matcher->end(status);
            if (U_FAILURE(status)) throw StriException(status);
         }
      }

      // Adjust UChar index -> UChar32 index (1-2 byte UTF16 to 1 byte UTF32-code points)
      str_cont.UChar16_to_UChar32_index(i,
            ret_tab+i, ret_tab+i+vectorize_length, 1,
            1, // 0-based index -> 1-based
            0  // end returns position of next character after match
      );
   }

   stri__locate_set_dimnames_matrix(ret);
   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/** Locate first or last occurence of a regex pattern
 *
 * @param str character vector
 * @param pattern character vector
 * @param opts_regex list
 * @return list of integer matrices (2 columns)
 *
 * @version 0.1 (Bartek Tartanus)
 * @version 0.2 (Marek Gagolewski) - StriContainerUTF16
 * @version 0.3 (Marek Gagolewski, 2013-06-19) use StriContainerRegexPattern + opts_regex
 */
SEXP stri_locate_first_regex(SEXP str, SEXP pattern, SEXP opts_regex)
{
   return stri__locate_firstlast_regex(str, pattern, opts_regex, true);
}




/** Locate first occurence of a regex pattern
 * @param str character vector
 * @param pattern character vector
 * @param opts_regex list
 * @return list of integer matrices (2 columns)
 * @version 0.1 (Bartlomiej Tartanus, 2013-06-10) - StriContainerUTF16
 * @version 0.2 (Marek Gagolewski, 2013-06-19) use StriContainerRegexPattern + opts_regex
 */
SEXP stri_locate_last_regex(SEXP str, SEXP pattern, SEXP opts_regex)
{
   return stri__locate_firstlast_regex(str, pattern, opts_regex, false);
}
