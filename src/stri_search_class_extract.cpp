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
#include "stri_container_utf8.h"
#include "stri_container_charclass.h"
#include "stri_container_logical.h"
#include <deque>
#include <utility>
using namespace std;


/**
 * Extract first or last occurences of a character class in each string
 *
 * @param str character vector
 * @param pattern character vector
 * @return character vector
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-08)
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-15)
 *          Use StrContainerCharClass
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-16)
 *          make StriException-friendly
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-04-03)
 *          detects invalid UTF-8 byte stream
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-04-05)
 *          StriContainerCharClass now relies on UnicodeSet
 */
SEXP stri__extract_firstlast_charclass(SEXP str, SEXP pattern, bool first)
{
   str = stri_prepare_arg_string(str, "str");
   pattern = stri_prepare_arg_string(pattern, "pattern");
   R_len_t vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 str_cont(str, vectorize_length);
   StriContainerCharClass pattern_cont(pattern, vectorize_length);

   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length));

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      SET_STRING_ELT(ret, i, NA_STRING);

      if (str_cont.isNA(i) || pattern_cont.isNA(i))
         continue;

      const UnicodeSet* pattern_cur = &pattern_cont.get(i);
      R_len_t     str_cur_n = str_cont.get(i).length();
      const char* str_cur_s = str_cont.get(i).c_str();
      R_len_t j, jlast;
      UChar32 chr;

      if (first) {
         for (jlast=j=0; j<str_cur_n; ) {
            U8_NEXT(str_cur_s, j, str_cur_n, chr);
            if (chr < 0) // invalid utf-8 sequence
               throw StriException(MSG__INVALID_UTF8);
            if (pattern_cur->contains(chr)) {
               SET_STRING_ELT(ret, i,
                  Rf_mkCharLenCE(str_cur_s+jlast, j-jlast, CE_UTF8));
               break; // that's enough for first
            }
            jlast = j;
         }
      }
      else {
         for (jlast=j=str_cur_n; j>0; ) {
            U8_PREV(str_cur_s, 0, j, chr); // go backwards
            if (chr < 0) // invalid utf-8 sequence
               throw StriException(MSG__INVALID_UTF8);
            if (pattern_cur->contains(chr)) {
               SET_STRING_ELT(ret, i,
                  Rf_mkCharLenCE(str_cur_s+j, jlast-j, CE_UTF8));
               break; // that's enough for last
            }
            jlast = j;
         }
      }
   }

   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}


/**
 * Extract first occurence of a character class in each string
 *
 * @param str character vector
 * @param pattern character vector
 * @return character vector
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-08)
 */
SEXP stri_extract_first_charclass(SEXP str, SEXP pattern)
{
   return stri__extract_firstlast_charclass(str, pattern, true);
}


/**
 * Extract last occurence of a character class in each string
 *
 * @param str character vector
 * @param pattern character vector
 * @return character vector
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-08)
 */
SEXP stri_extract_last_charclass(SEXP str, SEXP pattern)
{
   return stri__extract_firstlast_charclass(str, pattern, false);
}


/**
 * Extract all occurences of a character class in each string
 *
 * @param str character vector
 * @param pattern character vector
 * @return list of character vectors
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-08)
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-15)
 *          Use StrContainerCharClass
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-16)
 *          make StriException-friendly
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-04-03)
 *          detects invalid UTF-8 byte stream
 *
 * @version 0.2-1 (Marek Gagolewski, 2014-04-05)
 *          StriContainerCharClass now relies on UnicodeSet
 */
SEXP stri_extract_all_charclass(SEXP str, SEXP pattern, SEXP merge)
{
   str = stri_prepare_arg_string(str, "str");
   pattern = stri_prepare_arg_string(pattern, "pattern");
   merge = stri_prepare_arg_logical(merge, "merge");
   R_len_t vectorize_length = stri__recycling_rule(true, 3, LENGTH(str), LENGTH(pattern), LENGTH(merge));

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 str_cont(str, vectorize_length);
   StriContainerCharClass pattern_cont(pattern, vectorize_length);
   StriContainerLogical merge_cont(merge, vectorize_length);

   SEXP notfound; // this vector will be set iff not found or NA
   STRI__PROTECT(notfound = stri__vector_NA_strings(1));

   SEXP ret;
   STRI__PROTECT(ret = Rf_allocVector(VECSXP, vectorize_length));

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      if (pattern_cont.isNA(i) || str_cont.isNA(i) || merge_cont.isNA(i)) {
         SET_VECTOR_ELT(ret, i, notfound);
         continue;
      }

      bool merge_cur = merge_cont.get(i);
      const UnicodeSet* pattern_cur = &pattern_cont.get(i);
      R_len_t     str_cur_n = str_cont.get(i).length();
      const char* str_cur_s = str_cont.get(i).c_str();
      R_len_t j, jlast;
      UChar32 chr;
      deque< pair<R_len_t, R_len_t> > occurences; // codepoint based-indices

      for (jlast=j=0; j<str_cur_n; ) {
         U8_NEXT(str_cur_s, j, str_cur_n, chr);
         if (chr < 0) // invalid utf-8 sequence
            throw StriException(MSG__INVALID_UTF8);
         if (pattern_cur->contains(chr)) {
            occurences.push_back(pair<R_len_t, R_len_t>(jlast, j));
         }
         jlast = j;
      }

      R_len_t noccurences = (R_len_t)occurences.size();
      if (noccurences == 0)
         SET_VECTOR_ELT(ret, i, notfound);
      else if (merge_cur && noccurences > 1) {
         // do merge
         deque< pair<R_len_t, R_len_t> > occurences2;
         deque< pair<R_len_t, R_len_t> >::iterator iter = occurences.begin();
         occurences2.push_back(*iter);
         for (++iter; iter != occurences.end(); ++iter) {
            pair<R_len_t, R_len_t> curoccur = *iter;
            if (occurences2.back().second == curoccur.first) { // continue seq
               occurences2.back().second = curoccur.second;  // change `end`
            }
            else { // new seq
               occurences2.push_back(curoccur);
            }
         }

         // create resulting matrix from occurences2
         R_len_t noccurences2 = (R_len_t)occurences2.size();
         SEXP cur_res;
         STRI__PROTECT(cur_res = Rf_allocVector(STRSXP, noccurences2));
         iter = occurences2.begin();
         for (R_len_t f = 0; iter != occurences2.end(); ++iter, ++f) {
            pair<R_len_t, R_len_t> curo = *iter;
            SET_STRING_ELT(cur_res, f,
               Rf_mkCharLenCE(str_cur_s+curo.first, curo.second-curo.first, CE_UTF8));
         }
         SET_VECTOR_ELT(ret, i, cur_res);
         STRI__UNPROTECT(1)
      }
      else {
         // do not merge
         SEXP cur_res;
         STRI__PROTECT(cur_res = Rf_allocVector(STRSXP, noccurences));
         deque< pair<R_len_t, R_len_t> >::iterator iter = occurences.begin();
         for (R_len_t f = 0; iter != occurences.end(); ++iter, ++f) {
            pair<R_len_t, R_len_t> curo = *iter;
            SET_STRING_ELT(cur_res, f,
               Rf_mkCharLenCE(str_cur_s+curo.first, curo.second-curo.first, CE_UTF8));
         }
         SET_VECTOR_ELT(ret, i, cur_res);
         STRI__UNPROTECT(1)
      }
   }

   STRI__UNPROTECT_ALL
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}
