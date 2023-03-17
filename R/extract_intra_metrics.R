#' Compute and return as a data frame all the following patch complexity metric:
#' - shape index (SHAPE)
#' - fractal dimension (FRAC)
#' - mean detour index (MDI)
#'
#' @param raster_path path to a raster file
#' @param raster_value the raster value of the focal class
#' @param neighborhoodExtractCC the neighborhood definition to extract the
#' patches (4 or 8).
#' @param neighborhoodExploreCC the neighborhood definition to explore the
#' connectivity within the patches.
#'
#' @return A dataframe containing the intra-patch complexity metrics for all
#' patches of the landscape.
#'
#' @export
extract_intra_metrics <- function(raster_path, raster_value, neighborhoodExtractCC = 4, neighborhoodExploreCC = 4) {
    assertthat::is.string(raster_path)
    assertthat::assert_that(neighborhoodExtractCC == 4 || neighborhoodExtractCC == 8);
    assertthat::assert_that(neighborhoodExploreCC == 4 || neighborhoodExploreCC == 8);
    return(rcpp_get_patches(raster_path, raster_value, neighborhoodExtractCC, neighborhoodExploreCC))
}
