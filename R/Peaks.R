### Compute the difference between peak lists.
matched_diff <- structure(function(x, y){
  for(df in list(x, y)){
    stopifnot(is.data.frame(df))
    for(col.name in c("chromStart", "chromEnd")){
      stopifnot(col.name %in% names(df))
      stopifnot(is.integer(df[, col.name]))
    }
  }
  if(nrow(x)==0)return(y)
  if(nrow(y)==0)return(x)
  df <-
    rbind(data.frame(x, track=1L),
          data.frame(y, track=2L))
  df <- df[order(df$chromStart), ]
  diff.df <- matched_diff_interface(df)
  diff.df[order(diff.df$chromStart),]
}, ex=function(){
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
})

### Make a data.frame representing a list of peaks on 1 chromosome.
Peaks <- function(chromStart, chromEnd){
  stopifnot(chromStart < chromEnd)
  data.frame(chromStart=as.integer(chromStart),
             chromEnd=as.integer(chromEnd))
}
