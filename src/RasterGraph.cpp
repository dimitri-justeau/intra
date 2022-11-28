//
// Created by djusteau on 18/06/2021.
//

#include <gdal/gdal_priv.h>
#include <gdal/gdal_utils.h>
#include <gdal/gdal_alg.h>
#include <gdal/ogrsf_frmts.h>

#include "RasterGraph.h"

RasterGraph::RasterGraph(const char *raster_path, int class_value, Neighborhood neighborhoodExtractCC,
                         Neighborhood neighborhoodExploreCC) {
    this->class_value = class_value;
    // Load dataset with GDAL
    this->raster_path = raster_path;
    GDALDataset *dataset;
    GDALAllRegister(),
            dataset = (GDALDataset *) GDALOpen(this->raster_path, GA_ReadOnly );
    dataset->GetGeoTransform(transform);
    ogrSpatialReference = dataset->GetSpatialRef()->Clone();
    this->pixel_width = std::abs(transform[1]);
    this->pixel_height = std::abs(transform[5]);
    this->width = dataset->GetRasterBand(1)->GetXSize();
    this->height = dataset->GetRasterBand(1)->GetYSize();
    this->size = width * height;
    // Initialize neighTransform
    this->max_degree = neighborhoodExtractCC;
    this->neighTransform = new int[max_degree];
    initNeighTransform();

    // Build raster graph
    GDALRasterBand  *band = dataset->GetRasterBand(1);
    this->raster_values = (long *) CPLMalloc(sizeof(long) * width * height);
    band->RasterIO(GF_Read, 0, 0, width, height,
                   raster_values, width, height, GDT_CInt32,
                   0, 0);
    GDALClose(dataset);
    // Construct connected components
    this->idx_in_cc = new int[size];
    this->cc_idx = new int[size];
    int* fifo = new int[size];
    int* parent = new int[size]; for (int i = size; i--;) { parent[i] = -1; }
    this->nb_nodes = 0;
    this->nb_cc = 0;
    this->sizeMinCC = 0;
    this->sizeMaxCC = 0;
    for (int i = 0; i < size; i++) {
        if (raster_values[i] != class_value) {
            cc_idx[i] = -1;
            continue;
        }
        nb_nodes++;
        if (parent[i] == -1) {
            // Explore the CC
            std::vector<int> nodesCC(1, i);
            std::vector<int> neighbors(1, neighMask(i));
            int first = 0;
            int last = 0;
            fifo[last++] = i;
            parent[i] = i;
            idx_in_cc[i] = 0;
            cc_idx[i] = nb_cc;
            int sizeCC = 1;
            while (first < last) {
                int current = fifo[first++];
                for (int k = 0; k < max_degree; k++) {
                    int j = neigh(current, k);
                    if (j >= 0 && parent[j] == -1) {
                        parent[j] = current;
                        nodesCC.push_back(j);
                        neighbors.push_back(neighMask(j));
                        fifo[last++] = j;
                        idx_in_cc[j] = sizeCC;
                        cc_idx[j] = nb_cc;
                        sizeCC++;
                    }
                }
            }
            if (sizeMinCC == 0 || sizeMinCC > sizeCC) {
                sizeMinCC = sizeCC;
            }
            if (sizeMaxCC < sizeCC) {
                sizeMaxCC = sizeCC;
            }
            // Store the connected component
            connected_components.push_back(new ConnectedComponent(nodesCC, neighbors, idx_in_cc, width, pixel_width, pixel_height, neighborhoodExploreCC));
            nb_cc++;
        }
    }
    delete[] fifo;
    delete[] parent;
    // Display raster information
    printf("Raster file: %s\n", raster_path);
    printf("\tDimensions: %d x %d\n", width, height);
    printf("\tLandscape area: %f\n", getLandscapeArea());
    printf("\tNb nodes: %d\n", nb_nodes);
    printf("\tNb connected components: %d\n", nb_cc);
    printf("\tSize min cc: %d\n", sizeMinCC);
    printf("\tSize max cc: %d\n", sizeMaxCC);
}

RasterGraph::~RasterGraph() {
    delete[] idx_in_cc;
    delete[] raster_values;
    delete[] neighTransform;
    delete[] cc_idx;
    ogrSpatialReference->DestroySpatialReference(ogrSpatialReference);
    for (int i = 0; i < connected_components.size(); i++) {
      delete connected_components[i];
    }
}

int RasterGraph::leftNeigh(int node) {
    if (node % width != 0 && raster_values[node + neighTransform[LEFT]] == class_value) {
        return node + neighTransform[LEFT];
    }
    return -1;
}

int RasterGraph::topNeigh(int node) {
    if (node >= width && raster_values[node + neighTransform[TOP]] == class_value) {
        return node + neighTransform[TOP];
    }
    return -1;
}

int RasterGraph::rightNeigh(int node) {
    if ((node + 1) % width != 0 && raster_values[node + neighTransform[RIGHT]] == class_value) {
        return node + neighTransform[RIGHT];
    }
    return -1;
}

int RasterGraph::bottomNeigh(int node) {
    if (node < width * (height - 1) && raster_values[node + neighTransform[BOTTOM]] == class_value) {
        return node + neighTransform[BOTTOM];
    }
    return -1;
}

int RasterGraph::topLeftNeigh(int node) {
    if (node >= width && node % width != 0 && raster_values[node + neighTransform[TOP_LEFT]] == class_value) {
        return node + neighTransform[TOP_LEFT];
    }
    return -1;
}

int RasterGraph::topRightNeigh(int node) {
    if (node >= width && (node + 1) % width != 0 && raster_values[node + neighTransform[TOP_RIGHT]] == class_value) {
        return node + neighTransform[TOP_RIGHT];
    }
    return -1;
}

int RasterGraph::bottomRightNeigh(int node) {
    if (node < width * (height - 1) && (node + 1) % width != 0 && raster_values[node + neighTransform[BOTTOM_RIGHT]] == class_value) {
        return node + neighTransform[BOTTOM_RIGHT];
    }
    return -1;
}

int RasterGraph::bottomLeftNeigh(int node) {
    if (node < width * (height - 1) && node % width != 0 && raster_values[node + neighTransform[BOTTOM_LEFT]] == class_value) {
        return node + neighTransform[BOTTOM_LEFT];
    }
    return -1;
}


int RasterGraph::neigh(int node, int direction) {
    switch (direction) {
        case LEFT:
            return leftNeigh(node);
        case TOP:
            return topNeigh(node);
        case RIGHT:
            return rightNeigh(node);
        case BOTTOM:
            return bottomNeigh(node);
        case TOP_LEFT:
            return topLeftNeigh(node);
        case TOP_RIGHT:
            return topRightNeigh(node);
        case BOTTOM_RIGHT:
            return bottomRightNeigh(node);
        case BOTTOM_LEFT:
            return bottomLeftNeigh(node);
        default:
            return -1;
    }
}

int RasterGraph::neighMask(int node) {
    int neighMask = 0;
    if (leftNeigh(node) >= 0) {
        neighMask += 1;
    }
    if (topNeigh(node) >= 0) {
        neighMask += 2;
    }
    if (rightNeigh(node) >= 0) {
        neighMask += 4;
    }
    if (bottomNeigh(node) >= 0) {
        neighMask += 8;
    }
    if (max_degree == 4) {
        return neighMask;
    }
    if (topLeftNeigh(node) >= 0) {
        neighMask += 16;
    }
    if (topRightNeigh(node) >= 0) {
        neighMask += 32;
    }
    if (bottomRightNeigh(node) >= 0) {
        neighMask += 64;
    }
    if (bottomLeftNeigh(node) >= 0) {
        neighMask += 128;
    }
    return neighMask;
}

void RasterGraph::export_connected_component(int cc, char* dest) {
    auto *poSrcDS =
            (GDALDataset *) GDALOpen(raster_path, GA_ReadOnly );
    GDALDataset *poDstDS;
    const char *pszFormat = "GTiff";
    GDALDriver *poDriver;
    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    int destHeight = connected_components[cc]->getHeight();
    int destWidth = connected_components[cc]->getWidth();
    int destSize = destHeight * destWidth;
    int offSetX = connected_components[cc]->getMinX();
    int offSetY = connected_components[cc]->getMinY();
    poDstDS = poDriver->Create(
            dest,
            destWidth,
            destHeight,
            1,
            GDT_Byte,
            nullptr
    );
    double transform[6];
    poSrcDS->GetGeoTransform(transform);
    transform[0] += offSetX * transform[1];
    transform[3] += offSetY * transform[5];
    poDstDS->SetGeoTransform(transform);
    poDstDS->SetProjection(poSrcDS->GetProjectionRef());
    auto * data_to_write = (GByte *) CPLMalloc(sizeof(GByte) * destSize);
    for (int i = 0; i < destSize; i++) {
        data_to_write[i] = 0;
    }
    for (int i = 0; i < connected_components[cc]->getSize(); i++) {
        int x = connected_components[cc]->getXCoordinate(i) - offSetX;
        int y = connected_components[cc]->getYCoordinate(i) - offSetY;
        data_to_write[y * destWidth + x] = 1;
    }
    GDALRasterBand  *band = poDstDS->GetRasterBand(1);
    band->RasterIO(GF_Write, 0, 0, destWidth, destHeight,
                   data_to_write, destWidth, destHeight, GDT_Byte,
                   0, 0);

    char** opt = new char *[1];
    opt[0] = "-scale";
    GDALTranslateOptions* options = GDALTranslateOptionsNew(opt, nullptr);
    GDALTranslate(std::string(dest).append(".png").c_str(), poDstDS, options, nullptr);
    GDALTranslateOptionsFree(options);
    delete[] opt;
    // close the dataset
    GDALClose((GDALDatasetH) poDstDS);
    GDALClose((GDALDatasetH) poSrcDS);
}

void RasterGraph::polygonize(const char *dest, bool computeIndices) {
  // Extract raster data
  GDALDataset *dataset;
  GDALAllRegister(),
  dataset = (GDALDataset *) GDALOpen(this->raster_path, GA_ReadOnly);
  GDALRasterBand  *band = dataset->GetRasterBand(1);
  GDALRasterBand *src_mask = band->GetMaskBand();
  // Prepare output shapefile
  const char *dst_format = "ESRI Shapefile";
  GDALDriver *dst_driver;
  dst_driver = GetGDALDriverManager()->GetDriverByName(dst_format);
  GDALDataset *dst_dataset;
  dst_dataset = dst_driver->Create(dest, 0, 0, 0, GDT_CInt32, NULL);
  const char *src_crs = dataset->GetProjectionRef();
  OGRSpatialReference * dst_crs = new OGRSpatialReference(src_crs);
  OGRLayer *dst_layer = dst_dataset->CreateLayer("layer", dst_crs, wkbPolygon, NULL);
  OGRFeature *dst_feature;
  dst_feature = OGRFeature::CreateFeature(dst_layer->GetLayerDefn());
  // Feature fields
  OGRFieldDefn dst_field("class", OFTInteger);
  dst_layer->CreateField(&dst_field);
  dst_layer->CreateField(new OGRFieldDefn("AREA", OFTReal));
  if (computeIndices) {
    dst_layer->CreateField(new OGRFieldDefn("SHAPE", OFTReal));
    dst_layer->CreateField(new OGRFieldDefn("FRAC", OFTReal));
    dst_layer->CreateField(new OGRFieldDefn("MDI", OFTReal));
    dst_layer->CreateField(new OGRFieldDefn("CWA_SHAPE", OFTReal));
    dst_layer->CreateField(new OGRFieldDefn("CWA_FRAC", OFTReal));
    dst_layer->CreateField(new OGRFieldDefn("CWA_MDI", OFTReal));
  }
  int iPixValField = dst_feature->GetFieldIndex("class");
  char **papszOptions = NULL;
  papszOptions = CSLSetNameValue(papszOptions, "CONNECTED", std::to_string(max_degree).c_str());
  GDALPolygonize(band, src_mask, dst_layer, iPixValField, papszOptions, NULL, NULL);
  GDALClose(dataset);
  for(auto& poFeature: dst_layer) {
    if (poFeature->GetFieldAsInteger("class") != class_value) {
      dst_layer->DeleteFeature(poFeature->GetFID());
    }
  }
  for (int i = 0; i < getNumberOfConnectedComponents(); i++) {
    ConnectedComponent *cc = getConnectedComponent(i);
    int px = cc->getXCoordinate(0);
    int py = cc->getYCoordinate(0);
    double x = px * transform[1] + transform[0] + 0.5 * transform[1];
    double y = py * transform[5] + transform[3] + 0.5 * transform[5];
    OGRPoint* point = new OGRPoint(x, y);
    dst_layer->SetSpatialFilter(point);
    OGRFeature* feature = dst_layer->GetNextFeature();
    if (feature != NULL) {
      feature->SetField("AREA", cc->getArea());
      if (computeIndices) {
        feature->SetField("SHAPE", cc->getShapeIndex());
        feature->SetField("FRAC", cc->getFractalDimension());
        feature->SetField("MDI", cc->meanDetourIndex());
        feature->SetField("CWA_SHAPE", cc->getArea() / cc->getShapeIndex());
        feature->SetField("CWA_FRAC", cc->getArea() / cc->getFractalDimension());
        feature->SetField("CWA_MDI", cc->getArea() * cc->meanDetourIndex());
      }
      dst_layer->SetFeature(feature);
    }
  }
  GDALClose(dst_dataset);
}

int RasterGraph::getSizeOfLargestConnectedComponent() const {
    return sizeMaxCC;
}

int RasterGraph::getSizeOfSmallestConnectedComponent() const {
    return sizeMinCC;
}

int RasterGraph::getNumberOfConnectedComponents() const {
    return nb_cc;
}

int RasterGraph::getNumberOfNodes() const {
    return nb_nodes;
}

int RasterGraph::getRasterWidth() const {
    return width;
}

int RasterGraph::getRasterHeight() const {
    return height;
}

int RasterGraph::getRasterSize() const {
    return size;
}

double RasterGraph::getLandscapeArea() const {
  return size * pixel_height * pixel_width;
}

ConnectedComponent *RasterGraph::getConnectedComponent(int cc_index) {
    return connected_components[cc_index];
}

void RasterGraph::initNeighTransform() {
    switch (max_degree) {
        case FOUR_CONNECTED:
            neighTransform[LEFT] = -1;
            neighTransform[TOP] = -width;
            neighTransform[RIGHT] = 1;
            neighTransform[BOTTOM] = width;
            break;
        case HEIGHT_CONNECTED:
            neighTransform[LEFT] = -1;
            neighTransform[TOP] = -width;
            neighTransform[RIGHT] = 1;
            neighTransform[BOTTOM] = width;
            neighTransform[TOP_LEFT] = - 1 - width;
            neighTransform[TOP_RIGHT] = 1 - width;
            neighTransform[BOTTOM_RIGHT] = 1 + width;
            neighTransform[BOTTOM_LEFT] = width - 1;
            break;
    }
}

int RasterGraph::getPixelIndex(OGRPoint *point) {
    OGRSpatialReference *pRef = point->getSpatialReference();
    OGRCoordinateTransformation *transformation = OGRCreateCoordinateTransformation(pRef, ogrSpatialReference);
    double *x = new double {point->getX()};
    double *y = new double {point->getY()};
    transformation->Transform(1, x, y, NULL, NULL, NULL);
    int Rx = (int)(((x[0] - transform[0]) / transform[1]));
    int Ry = (int)(((y[0] - transform[3]) / transform[5]));
    return Ry * width + Rx;
}

int RasterGraph::getConnectedComponentIndex(int pixel_index) {
    if (pixel_index < 0 || pixel_index >= getRasterSize()) {
        return -1;
    }
    return cc_idx[pixel_index];
}


int RasterGraph::getIndexInConnectedComponent(int raster_pixel_index) {
    return idx_in_cc[raster_pixel_index];
}
