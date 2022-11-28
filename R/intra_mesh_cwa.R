# Copyright (c) 2021, Dimitri Justeau-Allaire
#
# Institut Agronomique neo-Caledonien (IAC), 98800 Noumea, New Caledonia
# AMAP, Univ Montpellier, CIRAD, CNRS, INRA, IRD, Montpellier, France

#' Compute MESH
#'
#' @param intra_metrics dataframe obtained using `extract_intra_metrics`
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
intra_mesh <- function(intra_metrics, landscape_summary) {
  a <- intra_metrics$area_geo
  a <- a * a
  return(sum(a) / landscape_summary$landscape_area)
}

#' Compute MESH_CWA[SHAPE]
#'
#' @param intra_metrics dataframe obtained using `extract_intra_metrics`
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
intra_mesh_cwa_shape <- function(intra_metrics, landscape_summary) {
  a <- intra_metrics$area_geo / intra_metrics$shape_index
  a <- a * a
  return(sum(a) / landscape_summary$landscape_area)
}
#' Compute MESH_CWA[FRAC]
#'
#' @param intra_metrics dataframe obtained using `extract_intra_metrics`
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
intra_mesh_cwa_frac <- function(intra_metrics, landscape_summary) {
  a <- intra_metrics$area_geo / intra_metrics$fractal_dimension
  a <- a * a
  return(sum(a) / landscape_summary$landscape_area)
}

#' Compute MESH_CWA[MDI]
#'
#' @param intra_metrics dataframe obtained using `extract_intra_metrics`
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
intra_mesh_cwa_mdi <- function(intra_metrics, landscape_summary) {
  a <- intra_metrics$area_geo * intra_metrics$mean_detour_index
  a <- a * a
  return(sum(a) / landscape_summary$landscape_area)
}
