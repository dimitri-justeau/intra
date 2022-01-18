// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "intra_types.h"
#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// rcpp_export_shapefile
void rcpp_export_shapefile(const char* raster_path, const char* dest, int class_value, int neighborhoodExtractCC, int neighborhoodExploreCC);
RcppExport SEXP _intra_rcpp_export_shapefile(SEXP raster_pathSEXP, SEXP destSEXP, SEXP class_valueSEXP, SEXP neighborhoodExtractCCSEXP, SEXP neighborhoodExploreCCSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const char* >::type raster_path(raster_pathSEXP);
    Rcpp::traits::input_parameter< const char* >::type dest(destSEXP);
    Rcpp::traits::input_parameter< int >::type class_value(class_valueSEXP);
    Rcpp::traits::input_parameter< int >::type neighborhoodExtractCC(neighborhoodExtractCCSEXP);
    Rcpp::traits::input_parameter< int >::type neighborhoodExploreCC(neighborhoodExploreCCSEXP);
    rcpp_export_shapefile(raster_path, dest, class_value, neighborhoodExtractCC, neighborhoodExploreCC);
    return R_NilValue;
END_RCPP
}
// rcpp_get_patches
DataFrame rcpp_get_patches(const char* raster_path, int class_value, int neighborhoodExtractCC, int neighborhoodExploreCC);
RcppExport SEXP _intra_rcpp_get_patches(SEXP raster_pathSEXP, SEXP class_valueSEXP, SEXP neighborhoodExtractCCSEXP, SEXP neighborhoodExploreCCSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const char* >::type raster_path(raster_pathSEXP);
    Rcpp::traits::input_parameter< int >::type class_value(class_valueSEXP);
    Rcpp::traits::input_parameter< int >::type neighborhoodExtractCC(neighborhoodExtractCCSEXP);
    Rcpp::traits::input_parameter< int >::type neighborhoodExploreCC(neighborhoodExploreCCSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_get_patches(raster_path, class_value, neighborhoodExtractCC, neighborhoodExploreCC));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_landscape_summary
DataFrame rcpp_landscape_summary(const char* raster_path, int class_value, int neighborhoodExtractCC, int neighborhoodExploreCC);
RcppExport SEXP _intra_rcpp_landscape_summary(SEXP raster_pathSEXP, SEXP class_valueSEXP, SEXP neighborhoodExtractCCSEXP, SEXP neighborhoodExploreCCSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const char* >::type raster_path(raster_pathSEXP);
    Rcpp::traits::input_parameter< int >::type class_value(class_valueSEXP);
    Rcpp::traits::input_parameter< int >::type neighborhoodExtractCC(neighborhoodExtractCCSEXP);
    Rcpp::traits::input_parameter< int >::type neighborhoodExploreCC(neighborhoodExploreCCSEXP);
    rcpp_result_gen = Rcpp::wrap(rcpp_landscape_summary(raster_path, class_value, neighborhoodExtractCC, neighborhoodExploreCC));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_intra_rcpp_export_shapefile", (DL_FUNC) &_intra_rcpp_export_shapefile, 5},
    {"_intra_rcpp_get_patches", (DL_FUNC) &_intra_rcpp_get_patches, 4},
    {"_intra_rcpp_landscape_summary", (DL_FUNC) &_intra_rcpp_landscape_summary, 4},
    {NULL, NULL, 0}
};

RcppExport void R_init_intra(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
