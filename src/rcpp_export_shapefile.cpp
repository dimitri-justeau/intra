#include <Rcpp.h>
using namespace Rcpp;

#include "RasterGraph.h"

// [[Rcpp::export]]
void rcpp_export_shapefile(const char* raster_path, const char* dest, int class_value, int neighborhoodExtractCC, int neighborhoodExploreCC, bool computeIndices) {
  Neighborhood neighExtract = (Neighborhood)neighborhoodExtractCC;
  Neighborhood neighExplore = (Neighborhood)neighborhoodExploreCC;
  RasterGraph *raster_graph = new RasterGraph(raster_path, class_value, neighExtract, neighExplore);
  raster_graph->polygonize(dest, computeIndices);
}
