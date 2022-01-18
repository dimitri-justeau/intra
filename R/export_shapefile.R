# Copyright (c) 2021, Dimitri Justeau-Allaire
#
# Institut Agronomique neo-Caledonien (IAC), 98800 Noumea, New Caledonia
# AMAP, Univ Montpellier, CIRAD, CNRS, INRA, IRD, Montpellier, France

#' Get landscape summary
#'
#' @param
#'
#' @details
#' TODO.
#'
#' @examples
#' \dontrun{TODO}
#'
#' @export
export_shapefile <- function(raster_path, raster_value, shapefile_path, neighborhoodExtractCC = 4, neighborhoodExploreCC = 4) {
  assertthat::is.string(raster_path)
  assertthat::is.string(shapefile_path)
  assertthat::assert_that(neighborhoodExtractCC == 4 || neighborhoodExtractCC == 8);
  assertthat::assert_that(neighborhoodExploreCC == 4 || neighborhoodExploreCC == 8);
  rcpp_export_shapefile(raster_path, shapefile_path, raster_value, neighborhoodExtractCC, neighborhoodExploreCC)
}
