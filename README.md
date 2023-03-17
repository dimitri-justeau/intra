# intra

The intra R package allows to compute patch complexity metrics from a raster
file. Specifically, it computes the shape index (SHAPE), the fractal dimension
(FRAC), and the mean detour index (MDI).

The main functions are:

`extract_intra_metrics <- function(raster_path, raster_value, neighborhoodExtractCC = 4, neighborhoodExploreCC = 4)`

Which compute these indices and return them as a dataframe, and:

`export_shapefile <- function(raster_path, raster_value, shapefile_path, neighborhoodExtractCC = 4, neighborhoodExploreCC = 4, computeIndices=TRUE)`

Which does the same but export the result as a shapefile, with the metrics as
patch attributes.

This package relies on `Rcpp`, and needs `GDAL/OGR` C++ dev libraries.

This is an experimental package, developed to design new intra-patch connectivity
indices. We would like to suggest their integration in a package such as
`landscapemetrics` in the future, and thus do not aim to publish it to CRAN.
