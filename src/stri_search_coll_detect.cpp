/* This file is part of the 'stringi' project.
 * Copyright (c) 2013-2025, Marek Gagolewski <https://www.gagolewski.com/>
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
#include "stri_container_usearch.h"
#include <unicode/uregex.h>


/**
 * Detect if a pattern occurs in a string [with collation]
 *
 * @param str character vector
 * @param pattern character vector
 * @param negate single bool
 * @param max_count single int
 * @param opts_collator passed to stri__ucol_open(),
 * if \code{NA}, then \code{stri_detect_fixed_byte} is called
 * @return logical vector
 *
 * @version 0.1-?? (Marek Gagolewski)
 *
 * @version 0.1-?? (Marek Gagolewski)
 *    corrected behavior on empty str/pattern
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-22)
 *    make StriException-friendly, use StriContainerUStringSearch
 *
 * @version 0.2-3 (Marek Gagolewski, 2014-05-08)
 *          new fun: stri_detect_coll (opts_collator == NA not allowed)
 *
 * @version 0.3-1 (Marek Gagolewski, 2014-11-04)
 *    Issue #112: str_prepare_arg* retvals were not PROTECTed from gc
 *
 * @version 1.0-3 (Marek Gagolewski, 2016-02-03)
 *    FR #216: `negate` arg added
 *
 * @version 1.3.1 (Marek Gagolewski, 2019-02-08)
 *    #232: `max_count` arg added
 */
SEXP stri_detect_coll(SEXP str, SEXP pattern, SEXP negate,
                      SEXP max_count, SEXP opts_collator)
{
    bool negate_1 = stri__prepare_arg_logical_1_notNA(negate, "negate");
    int max_count_1 = stri__prepare_arg_integer_1_notNA(max_count, "max_count");
    PROTECT(str = stri__prepare_arg_string(str, "str"));
    PROTECT(pattern = stri__prepare_arg_string(pattern, "pattern"));

    // call stri__ucol_open after prepare_arg:
    // if prepare_arg had failed, we would have a mem leak
    UCollator* collator = NULL;
    collator = stri__ucol_open(opts_collator);

    STRI__ERROR_HANDLER_BEGIN(2)
    R_len_t vectorize_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));
    StriContainerUTF16 str_cont(str, vectorize_length);
    StriContainerUStringSearch pattern_cont(pattern, vectorize_length, collator);  // collator is not owned by pattern_cont

    SEXP ret;
    STRI__PROTECT(ret = Rf_allocVector(LGLSXP, vectorize_length));
    int* ret_tab = LOGICAL(ret);

    for (R_len_t i = pattern_cont.vectorize_init();
            i != pattern_cont.vectorize_end();
            i = pattern_cont.vectorize_next(i))
    {
        if (max_count_1 == 0) {
            ret_tab[i] = NA_LOGICAL;
            continue;
        }

        STRI__CONTINUE_ON_EMPTY_OR_NA_STR_PATTERN(str_cont, pattern_cont,
                ret_tab[i] = NA_LOGICAL,
        {   ret_tab[i] = negate_1;
            if (max_count_1 > 0 && ret_tab[i]) --max_count_1;
        })

        UErrorCode status;
        UStringSearch *matcher = pattern_cont.getMatcher(i, str_cont.get(i));
        usearch_reset(matcher);



        status = U_ZERO_ERROR;
        ret_tab[i] = ((int)usearch_first(matcher, &status) != USEARCH_DONE);  // this is slow! :-(
        //ret_tab[i] = ((int)usearch_search(matcher, 0, NULL, NULL, &status));  // this is slow! :-(
        if (negate_1) ret_tab[i] = !ret_tab[i];
        if (max_count_1 > 0 && ret_tab[i]) --max_count_1;
        STRI__CHECKICUSTATUS_THROW(status, {/* do nothing special on err */})
    }

    if (collator) {
        ucol_close(collator);
        collator=NULL;
    }
    STRI__UNPROTECT_ALL
    return ret;
    STRI__ERROR_HANDLER_END(
        if (collator) ucol_close(collator);
    )
    }
