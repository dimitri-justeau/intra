#' Compute MESH
#'
#' @param intra_metrics dataframe obtained using `extract_intra_metrics`
#' @param landscape_summary dataframe obtained using `landscape_summary`
#'
#' @return The MESH value of the landscape.
#'
#' @export
intra_mesh <- function(intra_metrics, landscape_summary) {
  a <- intra_metrics$area_geo
  a <- a * a
  return(sum(a) / landscape_summary$landscape_area)
}

#' Compute MESH_CWA[SHAPE]
#'
#' @param intra_metrics dataframe obtained using `extract_intra_metrics`
#' @param landscape_summary dataframe obtained using `landscape_summary`
#'
#' @return The MESH_CWA[SHAPE] value of the landscape.
#'
#' @export
intra_mesh_cwa_shape <- function(intra_metrics, landscape_summary) {
  a <- intra_metrics$area_geo / intra_metrics$shape_index
  a <- a * a
  return(sum(a) / landscape_summary$landscape_area)
}
#' Compute MESH_CWA[FRAC]
#'
#' @param intra_metrics dataframe obtained using `extract_intra_metrics`
#' @param landscape_summary dataframe obtained using `landscape_summary`
#'
#' @return The MESH_CWA[FRAC] value of the landscape.
#'
#' @export
intra_mesh_cwa_frac <- function(intra_metrics, landscape_summary) {
  a <- intra_metrics$area_geo / intra_metrics$fractal_dimension
  a <- a * a
  return(sum(a) / landscape_summary$landscape_area)
}

#' Compute MESH_CWA[MDI]
#'
#' @param intra_metrics dataframe obtained using `extract_intra_metrics`
#' @param landscape_summary dataframe obtained using `landscape_summary`
#'
#' @return The MESH_CWA[MDI] value of the landscape.
#'
#' @export
intra_mesh_cwa_mdi <- function(intra_metrics, landscape_summary) {
  a <- intra_metrics$area_geo * intra_metrics$mean_detour_index
  a <- a * a
  return(sum(a) / landscape_summary$landscape_area)
}
