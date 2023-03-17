#' Get landscape summary
#'
#' @param raster_path path to a raster file
#' @param raster_value the raster value of the focal class
#' @param neighborhoodExtractCC the neighborhood definition to extract the
#' patches (4 or 8).
#' @param neighborhoodExploreCC the neighborhood definition to explore the
#' connectivity within the patches.
#'
#' @return A data frame containing summary values about a landscape class.
#' nbcc: number of patches
#' maxncc: largest patch size
#' minncc: smallest patch size
#' nb_nodes: the number of habitat pixels
#' landscape_area: the total landscape area in geographical units
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
