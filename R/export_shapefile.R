#' Compute all the following patch complexity metric, and export a shapefile
#' with these metrics as patch attributes:
#' - shape index (SHAPE)
#' - fractal dimension (FRAC)
#' - mean detour index (MDI)
#'
#' @param raster_path path to a raster file
#' @param raster_value the raster value of the focal class
#' @param shapefile_path the path of the shapefile to export
#' @param neighborhoodExtractCC the neighborhood definition to extract the
#' patches (4 or 8).
#' @param neighborhoodExploreCC the neighborhood definition to explore the
#' connectivity within the patches.
#'
#' @export
export_shapefile <- function(raster_path, raster_value, shapefile_path, neighborhoodExtractCC = 4, neighborhoodExploreCC = 4, computeIndices=TRUE) {
  assertthat::is.string(raster_path)
  assertthat::is.string(shapefile_path)
  assertthat::assert_that(neighborhoodExtractCC == 4 || neighborhoodExtractCC == 8)
  assertthat::assert_that(neighborhoodExploreCC == 4 || neighborhoodExploreCC == 8)
  assertthat::is.flag(computeIndices)
  rcpp_export_shapefile(raster_path, shapefile_path, raster_value, neighborhoodExtractCC, neighborhoodExploreCC, computeIndices)
}
