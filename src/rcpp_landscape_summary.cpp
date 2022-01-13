#include <Rcpp.h>
using namespace Rcpp;

#include "RasterGraph.h"

// [[Rcpp::export]]
DataFrame rcpp_landscape_summary(const char* raster_path, int class_value, int neighborhoodExtractCC, int neighborhoodExploreCC) {
    Neighborhood neighExtract = (Neighborhood)neighborhoodExtractCC;
    Neighborhood neighExplore = (Neighborhood)neighborhoodExploreCC;
    RasterGraph *raster_graph = new RasterGraph(raster_path, class_value, neighExtract, neighExplore);
    NumericVector nbcc = {raster_graph->getNumberOfConnectedComponents()};
    NumericVector width = {raster_graph->getRasterWidth()};
    NumericVector height = {raster_graph->getRasterHeight()};
    NumericVector size = {raster_graph->getRasterSize()};
    NumericVector nb_nodes = {raster_graph->getNumberOfNodes()};
    NumericVector landscape_area = {raster_graph->getLandscapeArea()};
    NumericVector maxncc = {raster_graph->getSizeOfLargestConnectedComponent()};
    NumericVector minncc = {raster_graph->getSizeOfSmallestConnectedComponent()};
    DataFrame df = DataFrame::create(
      Named("nbcc") = nbcc,
      Named("width") = width,
      Named("height") = height,
      Named("size") = size,
      Named("nb_nodes") = nb_nodes,
      Named("landscape_area") = landscape_area,
      Named("maxncc") = maxncc,
      Named("minncc") = minncc
    );
    delete raster_graph;
    return df;
}
