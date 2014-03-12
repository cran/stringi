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




/**
 * Extract first occurence of a fixed pattern in each string
 *
 * @param str character vector
 * @param pattern character vector
 * @param collator_opts list
 * @param firs logical - search for the first or the last occurence?
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-24)
 */
SEXP stri__extract_firstlast_fixed(SEXP str, SEXP pattern, SEXP collator_opts, bool first)
{
   str = stri_prepare_arg_string(str, "str");
   pattern = stri_prepare_arg_string(pattern, "pattern");

   // call stri__ucol_open after prepare_arg:
   // if prepare_arg had failed, we would have a mem leak
   UCollator* collator = stri__ucol_open(collator_opts);
   if (!collator)
      return stri__extract_firstlast_fixed_byte(str, pattern, first);

   STRI__ERROR_HANDLER_BEGIN
   R_len_t vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));
   StriContainerUTF16 str_cont(str, vectorize_length, false); // writable
   StriContainerUStringSearch pattern_cont(pattern, vectorize_length, collator);  // collator is not owned by pattern_cont

   SEXP ret;
   PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length));

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      STRI__CONTINUE_ON_EMPTY_OR_NA_STR_PATTERN(str_cont, pattern_cont,
         SET_STRING_ELT(ret, i, NA_STRING);, SET_STRING_ELT(ret, i, NA_STRING);)

      UStringSearch *matcher = pattern_cont.getMatcher(i, str_cont.get(i));
      usearch_reset(matcher);

      int start;
      if (first) {
         UErrorCode status = U_ZERO_ERROR;
         start = (int)usearch_first(matcher, &status);
         if (U_FAILURE(status)) throw StriException(status);
      } else {
         UErrorCode status = U_ZERO_ERROR;
         start = (int)usearch_last(matcher, &status);
         if (U_FAILURE(status)) throw StriException(status);
      }

      if (start == USEARCH_DONE) {
         SET_STRING_ELT(ret, i, NA_STRING);
         continue;
      }

      str_cont.getWritable(i).setTo(str_cont.get(i), (int32_t) start, (int32_t) usearch_getMatchedLength(matcher));
      SET_STRING_ELT(ret, i, str_cont.toR(i));
   }

   if (collator) { ucol_close(collator); collator=NULL; }
   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END(
      if (collator) ucol_close(collator);
   )
}


/**
 * Extract first occurence of a fixed pattern in each string
 *
 * @param str character vector
 * @param pattern character vector
 * @param collator_opts list
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-24)
 */
SEXP stri_extract_first_fixed(SEXP str, SEXP pattern, SEXP collator_opts)
{
   return stri__extract_firstlast_fixed(str, pattern, collator_opts, true);
}



/**
 * Extract last occurence of a fixed pattern in each string
 *
 * @param str character vector
 * @param pattern character vector
 * @param collator_opts list
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-24)
 */
SEXP stri_extract_last_fixed(SEXP str, SEXP pattern, SEXP collator_opts)
{
   return stri__extract_firstlast_fixed(str, pattern, collator_opts, false);
}




/**
 * Extract all occurences of a fixed pattern in each string
 *
 * @param str character vector
 * @param pattern character vector
 * @param collator_opts list
 * @return list of character vectors
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-24)
 */
SEXP stri_extract_all_fixed(SEXP str, SEXP pattern, SEXP collator_opts)
{
   str = stri_prepare_arg_string(str, "str");
   pattern = stri_prepare_arg_string(pattern, "pattern");

   // call stri__ucol_open after prepare_arg:
   // if prepare_arg had failed, we would have a mem leak
   UCollator* collator = stri__ucol_open(collator_opts);
   if (!collator)
      return stri__extract_all_fixed_byte(str, pattern);

   STRI__ERROR_HANDLER_BEGIN
   R_len_t vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));
   StriContainerUTF16 str_cont(str, vectorize_length);
   StriContainerUStringSearch pattern_cont(pattern, vectorize_length, collator);  // collator is not owned by pattern_cont

   SEXP ret;
   PROTECT(ret = Rf_allocVector(VECSXP, vectorize_length));

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      STRI__CONTINUE_ON_EMPTY_OR_NA_STR_PATTERN(str_cont, pattern_cont,
         SET_VECTOR_ELT(ret, i, stri__vector_NA_strings(1));,
         SET_VECTOR_ELT(ret, i, stri__vector_NA_strings(1));)

     UStringSearch *matcher = pattern_cont.getMatcher(i, str_cont.get(i));
      usearch_reset(matcher);

      UErrorCode status = U_ZERO_ERROR;
      int start = (int)usearch_first(matcher, &status);
      if (U_FAILURE(status)) throw StriException(status);

      if (start == USEARCH_DONE) {
         SET_VECTOR_ELT(ret, i, stri__vector_NA_strings(1));
         continue;
      }

      deque<R_len_t_x2> occurences;
      while (start != USEARCH_DONE) {
         occurences.push_back(R_len_t_x2(start, usearch_getMatchedLength(matcher)));
         start = usearch_next(matcher, &status);
         if (U_FAILURE(status)) throw StriException(status);
      }

      R_len_t noccurences = (R_len_t)occurences.size();
      StriContainerUTF16 out_cont(noccurences);
      deque<R_len_t_x2>::iterator iter = occurences.begin();
      for (R_len_t j = 0; iter != occurences.end(); ++iter, ++j) {
         R_len_t_x2 match = *iter;
         out_cont.getWritable(j).setTo(str_cont.get(i), match.v1, match.v2);
      }

      SET_VECTOR_ELT(ret, i, out_cont.toR());
   }

   if (collator) { ucol_close(collator); collator=NULL; }
   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END(
      if (collator) ucol_close(collator);
   )
}





/**
 * Extract first or last occurences of pattern in a string [with collation] - THIS IS DUMB! :)
 *
 * @param str character vector
 * @param pattern character vector
 * @param first looking for first or last match? [WHATEVER]
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-24)
 */
SEXP stri__extract_firstlast_fixed_byte(SEXP str, SEXP pattern, bool)
{
   str = stri_prepare_arg_string(str, "str");
   pattern = stri_prepare_arg_string(pattern, "pattern");

   STRI__ERROR_HANDLER_BEGIN
   int vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));
   StriContainerUTF8 str_cont(str, vectorize_length);
   StriContainerByteSearch pattern_cont(pattern, vectorize_length);

   SEXP ret;
   PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length));

   for (R_len_t i = pattern_cont.vectorize_init();
      i != pattern_cont.vectorize_end();
      i = pattern_cont.vectorize_next(i))
   {
      STRI__CONTINUE_ON_EMPTY_OR_NA_STR_PATTERN(str_cont, pattern_cont,
         SET_STRING_ELT(ret, i, NA_STRING);, SET_STRING_ELT(ret, i, NA_STRING);)

      pattern_cont.setupMatcher(i, str_cont.get(i).c_str(), str_cont.get(i).length());

      int start = pattern_cont.findFirst(); // whatever

      if (start == USEARCH_DONE)
         SET_STRING_ELT(ret, i, NA_STRING);
      else
         SET_STRING_ELT(ret, i, pattern_cont.toR(i));
   }

   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END( ;/* do nothing special on error */ )
}



/**
 * Extract all occurences of pattern in a string [with collation] - THIS IS DUMB! :)
 *
 * @param str character vector
 * @param pattern character vector
 * @param first looking for first or last match? [WHATEVER]
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-24)
 */
SEXP stri__extract_all_fixed_byte(SEXP str, SEXP pattern)
{
   str = stri_prepare_arg_string(str, "str");
   pattern = stri_prepare_arg_string(pattern, "pattern");

   STRI__ERROR_HANDLER_BEGIN
   int vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));
   StriContainerUTF8 str_cont(str, vectorize_length);
   StriContainerByteSearch pattern_cont(pattern, vectorize_length);

   SEXP ret;
   PROTECT(ret = Rf_allocVector(VECSXP, vectorize_length));

   for (R_len_t i = pattern_cont.vectorize_init();
      i != pattern_cont.vectorize_end();
      i = pattern_cont.vectorize_next(i))
   {
      STRI__CONTINUE_ON_EMPTY_OR_NA_STR_PATTERN(str_cont, pattern_cont,
         SET_VECTOR_ELT(ret, i, stri__vector_NA_strings(1));, SET_VECTOR_ELT(ret, i, stri__vector_NA_strings(1));)

      pattern_cont.setupMatcher(i, str_cont.get(i).c_str(), str_cont.get(i).length());

      int count = 0;
      while (pattern_cont.findNext() != USEARCH_DONE)
         ++count;

      if (count == 0)
         SET_VECTOR_ELT(ret, i, stri__vector_NA_strings(1));
      else {
         SEXP ans, match;
         PROTECT(ans = Rf_allocVector(STRSXP, count));
         PROTECT(match = pattern_cont.toR(i));
         for (R_len_t j=0; j<count; ++j)
            SET_STRING_ELT(ans, j, match);
         UNPROTECT(2);
         SET_VECTOR_ELT(ret, i, ans);
      }
   }

   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END( ;/* do nothing special on error */ )
}
