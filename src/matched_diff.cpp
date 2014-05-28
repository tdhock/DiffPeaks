 #include <cstdio>
// #include <limits>
//#include <iostream>
#include "matched_diff.h"

int
matched_diff
(int *peak_chromStart,
 int *peak_chromEnd,
 int *peak_track,
 int peak_count,
 int *diff_chromStart,
 int *diff_chromEnd){
  int peak_i = 0, diff_i = 0, status;
  PeakMatches peak_matches;
  Peak *peak;
  while(peak_i < peak_count) {
    peak_matches.init();
    // Add peaks and matches until there are no more overlapping
    // peaks.
    while(peak_matches.get_chromEnd() == 0 || 
	  (peak_i < peak_count && 
	   peak_chromStart[peak_i] < peak_matches.get_chromEnd())){
      peak = new Peak(peak_chromStart[peak_i], 
		      peak_chromEnd[peak_i],
		      peak_track[peak_i]);
      status = peak_matches.add_peak(peak);
      peak_i++;
      if(status < 0){
	return status;
      }
    }
    peak_matches.remove_matching();
    while(peak_matches.has_peaks()){
      peak = peak_matches.pop_peak();
      diff_chromStart[diff_i] = peak->get_chromStart();
      diff_chromEnd[diff_i] = peak->get_chromEnd();
      delete peak;
      diff_i++;
    }
    //printf("overlap done diff_i=%d peak_i=%d\n", diff_i, peak_i);
  }
  
  // 249250621 for hg19:chr1.
  // 248956422 for hg38:chr1.
  // 2147483647 returned by std::numeric_limits<int>::max().
  // 2000000000 == 2e9L in R (integer implemented as long).
  // So this code should work fine.

  // int imin = std::numeric_limits<int>::min(); // minimum value
  // int imax = std::numeric_limits<int>::max();
  // printf("%d %d\n", imin, imax);
   
    return diff_i;
}
