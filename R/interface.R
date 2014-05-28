### Compute the difference between two peak lists.
matched_diff_interface <- function(df){
  stopifnot(c("chromStart", "chromEnd", "track") %in% names(df))
  stopifnot(df$track %in% c(1, 2))
  n <- nrow(df)
  result <-
    .C("matched_diff_interface",
       as.integer(df$chromStart),
       as.integer(df$chromEnd),
       as.integer(df$track),
       count=as.integer(n),
       chromStart=as.integer(df$chromStart),
       chromEnd=as.integer(df$chromEnd),
       PACKAGE="DiffPeaks")
  with(result, {
    diff.df <- data.frame(chromStart, chromEnd)
    diff.df[1:count,]
  })
}
