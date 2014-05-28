context("Examples")

expect_peaks_diff <- function(peaks1, peaks2, diff.chromStart, diff.chromEnd){
  gen.list <-
    list(matched_diff(peaks1, peaks2),
         matched_diff(peaks2, peaks1))
  for(generated in gen.list){
    expect_identical(generated$chromStart, diff.chromStart)
    expect_identical(generated$chromEnd, diff.chromEnd)
  }
}

test_that("two peaks, no overlap, two differences", {
  expect_peaks_diff(Peaks(1, 5),
                    Peaks(7, 10),
                    c(1L, 7L),
                    c(5L, 10L))
})

test_that("two differences and two matches", {
  expect_peaks_diff(Peaks(c(1, 11, 25), c(4, 20, 30)+1),
                    Peaks(c(6, 11, 21), c(10, 14, 30)+1),
                    c(1L, 6L),
                    c(5L, 11L))
})

test_that("two peaks inside 1 peak -- pick the bigger one", {
  expect_peaks_diff(Peaks(1L, 11L),
                    Peaks(c(2L, 7L), c(5L, 9L)),
                    7L,
                    9L)
})

test_that("one peak completely overlapped, other partially", {
  expect_peaks_diff(Peaks(c(1L, 22L), c(21L, 25L)),
                    Peaks(10L, 25L),
                    22L,
                    25L)
})

test_that("several overlaps", {
  expect_peaks_diff(Peaks(c(10, 100, 210, 230), c(20, 200, 220, 300)),
                    Peaks(c(90, 120, 250, 400), c(110, 240, 300, 500)),
                    c(10L, 90L, 210L, 400L),
                    c(20L, 110L, 220L, 500L))
})
