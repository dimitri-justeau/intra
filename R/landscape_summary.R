# Copyright (c) 2021, Dimitri Justeau-Allaire
#
# Institut Agronomique neo-Caledonien (IAC), 98800 Noumea, New Caledonia
# AMAP, Univ Montpellier, CIRAD, CNRS, INRA, IRD, Montpellier, France

#' Get landscape summary
#'
#' @param
#'
#' @return A dataframe
#'
#' @details
#' TODO.
#'
#' @examples
#' \dontrun{TODO}
#'
#' @export
landscape_summary <- function(raster_path, raster_value, neighborhoodExtractCC = 4, neighborhoodExploreCC = 4) {
  assertthat::is.string(raster_path)
  assertthat::assert_that(neighborhoodExtractCC == 4 || neighborhoodExtractCC == 8);
  assertthat::assert_that(neighborhoodExploreCC == 4 || neighborhoodExploreCC == 8);
  return(rcpp_landscape_summary(raster_path, raster_value, neighborhoodExtractCC, neighborhoodExploreCC))
}
