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
#' Query Default Settings for \pkg{stringi}
#'
#' @description
#' Gives the current default settings used by the \pkg{ICU} library.
#'
#' @param short logical; whether or not the results should be given
#' in a concise form; defaults to \code{TRUE}
#'
#' @return If \code{short} is \code{TRUE}, then a single string providing
#' information on the default character encoding, locale, and Unicode
#' as well as \pkg{ICU} version is returned.
#'
#' Otherwise, a list with the following components is returned:
#' \itemize{
#' \item \code{Unicode.version} -- version of Unicode supported
#' by the \pkg{ICU} library;
#' \item \code{ICU.version} -- \pkg{ICU} library version used;
#' \item \code{Locale} -- contains information on default locale,
#' as returned by \code{\link{stri_locale_info}};
#' \item \code{Charset.internal} -- fixed at \code{c('UTF-8', 'UTF-16')};
#' \item \code{Charset.native} -- information on the default encoding,
#' as returned by \code{\link{stri_enc_info}};
#' \item \code{ICU.system} -- logical; \code{TRUE} indicates that
#' the system \pkg{ICU} libs are used, otherwise \pkg{ICU} was built together
#' with \pkg{stringi};
#' \item \code{ICU.UTF8} -- logical; \code{TRUE} if the internal
#' \code{U_CHARSET_IS_UTF8} flag is defined and set.
#' }
#'
#' @export
#' @family locale
#' @family encoding
stri_info <- function(short = FALSE)
{
    stopifnot(is.logical(short), length(short) == 1)

    info <- .Call(C_stri_info)
    #loclist <- stri_locale_list()
    locale <- info$Locale$Name
    charset <- info$Charset.native$Name.friendly

    if (charset != "UTF-8") {
        if (!identical(info$Charset.native$ASCII.subset, TRUE))
            warning(stri_paste("Your native character encoding is not a superset of US-ASCII. ",
                "Consider switching to UTF-8."))
        else if (!identical(info$Charset.native$Unicode.1to1, TRUE))
            warning(stri_paste("Your native character encoding does not map to Unicode properly. ",
                "Consider switching to UTF-8."))
    }

    if (!short)
        return(info) else {
        return(sprintf("stringi_%s (%s.%s; ICU4C %s [%s%s]; Unicode %s)", as.character(packageVersion("stringi")),
            locale, charset, info$ICU.version, if (info$ICU.system) "system" else "bundle",
            if (info$ICU.UTF8) "#U_CHARSET_IS_UTF8" else "", info$Unicode.version))
    }
}
