\name{matched_diff}
\alias{matched_diff}
\title{matched diff}
\description{Compute the difference between peak lists.}
\usage{matched_diff(x, y)}
\arguments{
  \item{x}{
}
  \item{y}{
}
}



\author{Toby Dylan Hocking}




\examples{
some.peaks <- Peaks(c(10, 100, 210, 230), c(20, 200, 220, 300))
more.peaks <- Peaks(c(90, 120, 250, 400), c(110, 240, 300, 500))
peak.diff <- matched_diff(some.peaks, more.peaks)
segs <-
  rbind(data.frame(some.peaks, what="one sample"),
        data.frame(more.peaks, what="another sample"),
        data.frame(peak.diff, what="difference"))
library(ggplot2)
ggplot()+
  geom_segment(aes(chromStart, what, xend=chromEnd, yend=what), data=segs)
}
