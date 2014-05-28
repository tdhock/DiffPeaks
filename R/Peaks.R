### Compute the difference between peak lists.
matched_diff <- function(x, y){
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
  matched_diff_interface(df)
}

### Make a data.frame representing a list of peaks on 1 chromosome.
Peaks <- function(chromStart, chromEnd){
  stopifnot(chromStart < chromEnd)
  data.frame(chromStart=as.integer(chromStart),
             chromEnd=as.integer(chromEnd))
}
