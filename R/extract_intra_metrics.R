# Copyright (c) 2021, Dimitri Justeau-Allaire
#
# Institut Agronomique neo-Caledonien (IAC), 98800 Noumea, New Caledonia
# AMAP, Univ Montpellier, CIRAD, CNRS, INRA, IRD, Montpellier, France

#' Compute all intra-patch connectivity metric.
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
extract_intra_metrics <- function(raster_path, raster_value, neighborhoodExtractCC = 4, neighborhoodExploreCC = 4) {
    assertthat::is.string(raster_path)
    assertthat::assert_that(neighborhoodExtractCC == 4 || neighborhoodExtractCC == 8);
    assertthat::assert_that(neighborhoodExploreCC == 4 || neighborhoodExploreCC == 8);
    return(rcpp_get_patches(raster_path, raster_value, neighborhoodExtractCC, neighborhoodExploreCC))
}
