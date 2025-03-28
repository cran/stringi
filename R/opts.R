# kate: default-dictionary en_US

## This file is part of the 'stringi' package for R.
## Copyright (c) 2013-2025, Marek Gagolewski <https://www.gagolewski.com/>
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
##
## 1. Redistributions of source code must retain the above copyright notice,
## this list of conditions and the following disclaimer.
##
## 2. Redistributions in binary form must reproduce the above copyright notice,
## this list of conditions and the following disclaimer in the documentation
## and/or other materials provided with the distribution.
##
## 3. Neither the name of the copyright holder nor the names of its
## contributors may be used to endorse or promote products derived from
## this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
## 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
## BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
## HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
## SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
## OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
## WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
## OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
## EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#' @title
#' Generate a List with Collator Settings
#'
#' @description
#' A convenience function to tune the \pkg{ICU} Collator's behavior,
#' e.g., in \code{\link{stri_compare}}, \code{\link{stri_order}},
#' \code{\link{stri_unique}}, \code{\link{stri_duplicated}},
#' as well as \code{\link{stri_detect_coll}}
#' and other \link{stringi-search-coll} functions.
#'
#'
#' @details
#' \pkg{ICU}'s \emph{collator} performs a locale-aware, natural-language
#' alike string comparison.
#' This is a more reliable way of establishing relationships between
#' strings than the one provided by base \R, and definitely
#' one that is more complex and appropriate than ordinary bytewise
#' comparison.
#'
#'
#' @param locale single string, \code{NULL} or
#' \code{''} for default locale
#' @param strength single integer in \{1,2,3,4\}, which defines collation strength;
#' \code{1} for the most permissive collation rules, \code{4} for the strictest
#' ones
#' @param alternate_shifted single logical value; \code{FALSE}
#'  treats all the code points with non-ignorable primary weights in the same way,
#'  \code{TRUE} causes code points with primary weights that are equal or below
#'  the variable top value to be ignored on primary level and moved to the quaternary level
#' @param french single logical value; used in Canadian French;
#' \code{TRUE} results in secondary weights being considered backwards
#' @param uppercase_first single logical value; \code{NA}
#' orders upper and lower case letters in accordance to their tertiary weights,
#' \code{TRUE} forces upper case letters to sort before lower case letters,
#' \code{FALSE} does the opposite
#' @param case_level single logical value;
#' controls whether an extra case level (positioned before the third level) is generated or not
#' @param normalization
#' single logical value; if \code{TRUE},
#' then incremental check is performed to see whether the input data is in
#' the FCD form. If the data is not in the FCD form, incremental NFD
#' normalization is performed
#' @param normalisation alias of \code{normalization}
#' @param numeric single logical value;
#' when turned on, this attribute generates a collation key for
#' the numeric value of substrings of digits;
#' this is a way to get '100' to sort AFTER '2';
#' note that negative or non-integer numbers will not be ordered properly
#'
#' @return
#' Returns a named list object; missing settings are left with default values.
#'
#' @export
#' @family locale_sensitive
#' @family search_coll
#'
#'
#' @rdname stri_opts_collator
#'
#' @references
#' \emph{Collation} -- ICU User Guide,
#' \url{https://unicode-org.github.io/icu/userguide/collation/}
#'
#' \emph{ICU Collation Service Architecture} -- ICU User Guide,
#' \url{https://unicode-org.github.io/icu/userguide/collation/architecture.html}
#'
#' \emph{\code{icu::Collator} Class Reference} -- ICU4C API Documentation,
#' \url{https://unicode-org.github.io/icu-docs/apidoc/dev/icu4c/classicu_1_1Collator.html}
#'
#' @examples
#' stri_cmp('number100', 'number2')
#' stri_cmp('number100', 'number2', opts_collator=stri_opts_collator(numeric=TRUE))
#' stri_cmp('number100', 'number2', numeric=TRUE) # equivalent
#' stri_cmp('above mentioned', 'above-mentioned')
#' stri_cmp('above mentioned', 'above-mentioned', alternate_shifted=TRUE)
stri_opts_collator <- function(
    locale = NULL, strength = 3L, alternate_shifted = FALSE,
    french = FALSE, uppercase_first = NA, case_level = FALSE, normalization = FALSE,
    normalisation = normalization, numeric = FALSE
) {
    opts <- list()
    if (!missing(locale))
        opts["locale"] <- locale
    if (!missing(strength))
        opts["strength"] <- strength
    if (!missing(alternate_shifted))
        opts["alternate_shifted"] <- alternate_shifted
    if (!missing(french))
        opts["french"] <- french
    if (!missing(uppercase_first))
        opts["uppercase_first"] <- uppercase_first
    if (!missing(case_level))
        opts["case_level"] <- case_level
    if (!missing(numeric))
        opts["numeric"] <- numeric

    if (!missing(normalization))
        opts["normalization"] <- normalization
    else if (!missing(normalisation))
        opts["normalization"] <- normalisation

    opts
}


#' @rdname stri_opts_collator
#' @export
stri_coll <- stri_opts_collator


#' @title
#' Generate a List with Regex Matcher Settings
#'
#' @description
#' A convenience function to tune the \pkg{ICU} regular expressions
#' matcher's behavior, e.g., in \code{\link{stri_count_regex}}
#' and other \link{stringi-search-regex} functions.
#'
#' @details
#' Note that some regex settings may be changed using ICU regex flags
#' inside regexes. For example, \code{'(?i)pattern'} performs
#' a case-insensitive match of a given pattern,
#' see the \pkg{ICU} User Guide entry on Regular Expressions
#' in the References section or \link{stringi-search-regex}.
#'
#' @param case_insensitive logical; enables case insensitive matching [regex flag \code{(?i)}]
#' @param comments logical; allows white space and comments within patterns [regex flag \code{(?x)}]
#' @param dotall logical;  if set, `\code{.}` matches line terminators,
#'  otherwise matching of `\code{.}`  stops at a line end [regex flag \code{(?s)}]
#' @param dot_all alias of \code{dotall}
#' @param literal logical; if set, treat the entire pattern as a literal string:
#' metacharacters or escape sequences in the input sequence will be given no special meaning;
#' note that in most cases you would rather use the \link{stringi-search-fixed}
#' facilities in this case
#' @param multiline logical; controls the behavior of `\code{$}` and `\code{^}`.
#' If set, recognize line terminators within a string, otherwise,
#'  match only at start and end of input string [regex flag \code{(?m)}]
#' @param multi_line alias of \code{multiline}
#' @param unix_lines logical; Unix-only line endings;
#' when enabled, only \code{U+000a} is recognized as a
#' line ending by `\code{.}`, `\code{$}`, and `\code{^}`.
#' @param uword logical; Unicode word boundaries;
#' if set, uses the Unicode TR 29 definition of word boundaries;
#' warning: Unicode word boundaries are quite different from traditional
#' regex word boundaries. [regex flag \code{(?w)}]
#' See \url{https://unicode.org/reports/tr29/#Word_Boundaries}
#' @param error_on_unknown_escapes logical;
#' whether to generate an error on unrecognized backslash escapes;
#' if set, fail with an error on patterns that contain backslash-escaped ASCII
#' letters without a known special meaning;
#' otherwise, these escaped letters represent themselves
#' @param time_limit integer; processing time limit, in ~milliseconds (but not precisely so,
#' depends on the CPU speed), for match operations;
#' setting a limit is desirable if poorly written regexes are expected on input;
#' 0 for no limit
#' @param stack_limit integer; maximal size, in bytes, of the heap storage available
#' for the match backtracking stack; setting a limit is desirable if poorly
#' written regexes are expected on input; 0 for no limit
#'
#' @return
#' Returns a named list object; missing settings are left with default values.
#'
#' @export
#' @family search_regex
#'
#' @references
#' \emph{\code{enum URegexpFlag}: Constants for Regular Expression Match Modes}
#' -- ICU4C API Documentation,
#' \url{https://unicode-org.github.io/icu-docs/apidoc/dev/icu4c/uregex_8h.html}
#'
#' \emph{Regular Expressions} -- ICU User Guide,
#' \url{https://unicode-org.github.io/icu/userguide/strings/regexp.html}
#'
#' @examples
#' stri_detect_regex('ala', 'ALA') # case-sensitive by default
#' stri_detect_regex('ala', 'ALA', opts_regex=stri_opts_regex(case_insensitive=TRUE))
#' stri_detect_regex('ala', 'ALA', case_insensitive=TRUE) # equivalent
#' stri_detect_regex('ala', '(?i)ALA') # equivalent
stri_opts_regex <- function(
    case_insensitive, comments,
    dotall, dot_all = dotall,
    literal,
    multiline, multi_line = multiline,
    unix_lines, uword, error_on_unknown_escapes,
    time_limit = 0L, stack_limit = 0L
) {
    opts <- list()
    if (!missing(case_insensitive))
        opts["case_insensitive"] <- case_insensitive
    if (!missing(comments))
        opts["comments"] <- comments
    if (!missing(literal))
        opts["literal"] <- literal
    if (!missing(unix_lines))
        opts["unix_lines"] <- unix_lines
    if (!missing(uword))
        opts["uword"] <- uword
    if (!missing(error_on_unknown_escapes))
        opts["error_on_unknown_escapes"] <- error_on_unknown_escapes

    if (!missing(stack_limit))
        opts["stack_limit"] <- stack_limit
    if (!missing(time_limit))
        opts["time_limit"] <- time_limit

    if (!missing(dotall))
        opts["dotall"] <- dotall
    else if (!missing(dot_all))
        opts["dotall"] <- dot_all

    if (!missing(multiline))
        opts["multiline"] <- multiline
    else if (!missing(multi_line))
        opts["multiline"] <- multi_line

    opts
}


#' @title
#' Generate a List with BreakIterator Settings
#'
#' @description
#' A convenience function to tune the \pkg{ICU} \code{BreakIterator}'s behavior
#' in some text boundary analysis functions, see
#' \link{stringi-search-boundaries}.
#'
#' @details
#' The \code{skip_*} family of settings may be used to prevent performing
#' any special actions on particular types of text boundaries, e.g.,
#' in case of the \code{\link{stri_locate_all_boundaries}} and
#' \code{\link{stri_split_boundaries}} functions.
#'
#' Note that custom break iterator rules (advanced users only)
#' should be specified as a single string.
#' For a detailed description of the syntax of RBBI rules, please refer
#' to the ICU User Guide on Boundary Analysis.
#'
#' @param type single string; either the break iterator type, one of \code{character},
#' \code{line_break}, \code{sentence}, \code{word},
#' or a custom set of ICU break iteration rules;
#' see \link{stringi-search-boundaries}
#' @param locale single string, \code{NULL} or \code{''} for default locale
#' @param skip_word_none logical; perform no action for 'words' that
#' do not fit into any other categories
#' @param skip_word_number logical; perform no action for words that
#' appear to be numbers
#' @param skip_word_letter logical; perform no action for words that
#' contain letters, excluding hiragana, katakana, or ideographic characters
#' @param skip_word_kana logical; perform no action for words
#' containing kana characters
#' @param skip_word_ideo logical; perform no action for words
#' containing ideographic characters
#' @param skip_line_soft logical; perform no action for soft line breaks,
#' i.e., positions where a line break is acceptable but not required
#' @param skip_line_hard logical; perform no action for hard,
#' or mandatory line breaks
#' @param skip_sentence_term logical; perform no action for sentences
#' ending with a sentence terminator ('\code{.}', '\code{,}', '\code{?}',
#' '\code{!}'), possibly followed by a hard separator
#' (\code{CR}, \code{LF}, \code{PS}, etc.)
#' @param skip_sentence_sep logical; perform no action for sentences
#' that do not contain an ending sentence terminator, but are ended
#' by a hard separator or end of input
#'
#' @return
#' Returns a named list object.
#' Omitted \code{skip_*} values act as they have been set to \code{FALSE}.
#'
#' @export
#' @family text_boundaries
#'
#' @references
#' \emph{\code{ubrk.h} File Reference} -- ICU4C API Documentation,
#' \url{https://unicode-org.github.io/icu-docs/apidoc/dev/icu4c/ubrk_8h.html}
#'
#' \emph{Boundary Analysis} -- ICU User Guide,
#' \url{https://unicode-org.github.io/icu/userguide/boundaryanalysis/}
stri_opts_brkiter <- function(
    type, locale, skip_word_none, skip_word_number,
    skip_word_letter, skip_word_kana, skip_word_ideo, skip_line_soft,
    skip_line_hard, skip_sentence_term, skip_sentence_sep
) {
    opts <- list()
    if (!missing(type))
        opts["type"] <- type
    if (!missing(locale))
        opts["locale"] <- locale
    if (!missing(skip_word_none))
        opts["skip_word_none"] <- skip_word_none
    if (!missing(skip_word_number))
        opts["skip_word_number"] <- skip_word_number
    if (!missing(skip_word_letter))
        opts["skip_word_letter"] <- skip_word_letter
    if (!missing(skip_word_kana))
        opts["skip_word_kana"] <- skip_word_kana
    if (!missing(skip_word_ideo))
        opts["skip_word_ideo"] <- skip_word_ideo
    if (!missing(skip_line_soft))
        opts["skip_line_soft"] <- skip_line_soft
    if (!missing(skip_line_hard))
        opts["skip_line_hard"] <- skip_line_hard
    if (!missing(skip_sentence_term))
        opts["skip_sentence_term"] <- skip_sentence_term
    if (!missing(skip_sentence_sep))
        opts["skip_sentence_sep"] <- skip_sentence_sep
    opts
}


#' @title
#' Generate a List with Fixed Pattern Search Engine's Settings
#'
#' @description
#' A convenience function used to tune up the behavior of \code{stri_*_fixed}
#' functions, see \link{stringi-search-fixed}.
#'
#' @details
#' Case-insensitive matching uses a simple, single-code point case mapping
#' (via ICU's \code{u_toupper()} function).
#' Full case mappings should be used whenever possible because they produce
#' better results by working on whole strings. They also take into account
#' the string context and the language, see \link{stringi-search-coll}.
#'
#' Searching for overlapping pattern matches is available in
#' \code{\link{stri_extract_all_fixed}}, \code{\link{stri_locate_all_fixed}},
#' and \code{\link{stri_count_fixed}} functions.
#'
#' @param case_insensitive logical; enable simple case insensitive matching
#' @param overlap logical; enable overlapping matches' detection
#'
#' @return
#' Returns a named list object.
#'
#' @export
#' @family search_fixed
#'
#' @references
#' \emph{C/POSIX Migration} -- ICU User Guide,
#' \url{https://unicode-org.github.io/icu/userguide/icu/posix.html}
#'
#' @examples
#' stri_detect_fixed('ala', 'ALA') # case-sensitive by default
#' stri_detect_fixed('ala', 'ALA', opts_fixed=stri_opts_fixed(case_insensitive=TRUE))
#' stri_detect_fixed('ala', 'ALA', case_insensitive=TRUE) # equivalent
stri_opts_fixed <- function(case_insensitive = FALSE, overlap = FALSE)
{
    opts <- list()
    if (!missing(case_insensitive))
        opts["case_insensitive"] <- case_insensitive
    if (!missing(overlap))
        opts["overlap"] <- overlap
    opts
}
