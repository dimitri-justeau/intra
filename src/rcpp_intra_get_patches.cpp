#include <Rcpp.h>
using namespace Rcpp;

#include "RasterGraph.h"

// [[Rcpp::export]]
DataFrame rcpp_get_patches(const char* raster_path, int class_value, int neighborhoodExtractCC, int neighborhoodExploreCC) {
    Neighborhood neighExtract = (Neighborhood)neighborhoodExtractCC;
    Neighborhood neighExplore = (Neighborhood)neighborhoodExploreCC;
    RasterGraph *raster_graph = new RasterGraph(raster_path, class_value, neighExtract, neighExplore);
    int nbcc = raster_graph->getNumberOfConnectedComponents();
    NumericVector size_pixel(nbcc);
    NumericVector area_geo(nbcc);
    NumericVector perimeter_geo(nbcc);
    NumericVector shape_index(nbcc);
    NumericVector fractal_dimension(nbcc);
    NumericVector mean_detour_index(nbcc);
    for (int i = 0; i < nbcc; i++) {
        size_pixel[i] = raster_graph->getConnectedComponent(i)->getSize();
        area_geo[i] = raster_graph->getConnectedComponent(i)->getArea();
        perimeter_geo[i] = raster_graph->getConnectedComponent(i)->getPerimeter();
        shape_index[i] = raster_graph->getConnectedComponent(i)->getShapeIndex();
        fractal_dimension[i] = raster_graph->getConnectedComponent(i)->getFractalDimension();
        mean_detour_index[i] = raster_graph->getConnectedComponent(i)->meanDetourIndex();
    }
    DataFrame df = DataFrame::create(
        Named("size_px") = size_pixel,
        Named("area_geo") = area_geo,
        Named("perimeter_geo") = perimeter_geo,
        Named("shape_index") = shape_index,
        Named("fractal_dimension") = fractal_dimension,
        Named("mean_detour_index") = mean_detour_index
    );
    delete raster_graph;
    return df;
}
