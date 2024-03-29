% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/random.R
\name{stri_rand_lipsum}
\alias{stri_rand_lipsum}
\title{A Lorem Ipsum Generator}
\usage{
stri_rand_lipsum(n_paragraphs, start_lipsum = TRUE, nparagraphs = n_paragraphs)
}
\arguments{
\item{n_paragraphs}{single integer, number of paragraphs to generate}

\item{start_lipsum}{single logical value; should the resulting
text start with \emph{Lorem ipsum dolor sit amet}?}

\item{nparagraphs}{[DEPRECATED] alias of \code{n_paragraphs}}
}
\value{
Returns a character vector of length \code{n_paragraphs}.
}
\description{
Generates (pseudo)random \emph{lorem ipsum} text consisting
of a given number of text paragraphs.
}
\details{
\emph{Lorem ipsum} is a dummy text often used as a source
of data for string processing and displaying/lay-outing exercises.

The current implementation is very simple:
words are selected randomly from a Zipf distribution
(based on a set of ca. 190 predefined Latin words).
The number of words per sentence and sentences per paragraph
follows a discretized, truncated normal distribution.
No Markov chain modeling, just i.i.d. word selection.
}
\examples{
cat(sapply(
   stri_wrap(stri_rand_lipsum(10), 80, simplify=FALSE),
   stri_flatten, collapse='\n'), sep='\n\n')
cat(stri_rand_lipsum(10), sep='\n\n')

}
\seealso{
The official online manual of \pkg{stringi} at \url{https://stringi.gagolewski.com/}

Gagolewski M., \pkg{stringi}: Fast and portable character string processing in R, \emph{Journal of Statistical Software} 103(2), 2022, 1-59, \doi{10.18637/jss.v103.i02}

Other random: 
\code{\link{stri_rand_shuffle}()},
\code{\link{stri_rand_strings}()}
}
\concept{random}
\author{
\href{https://www.gagolewski.com/}{Marek Gagolewski} and other contributors
}
