//
// Created by djusteau on 18/06/2021.
//

#include <ogr_spatialref.h>
#include <ogr_geometry.h>

#include "ConnectedComponent.h"

/**
 * Class that reads a binary (0 - 1) raster and converts it into a spatial graph (currently only 4-connected
 * neighTransform is supported). Connected components are extracted into a data structure optimized for computation
 * of the mean detour index, which involves computing all pairs shortest paths.
 */
class RasterGraph {

private:

    // Raster related
    const char *raster_path;
    int width;
    int height;
    int size;
    int class_value;
    long* raster_values;
    double transform[6];
    OGRSpatialReference* ogrSpatialReference;
    double pixel_width;
    double pixel_height;

    // Graph related
    int nb_nodes;
    int nb_cc;
    int max_degree;
    int sizeMaxCC;
    int sizeMinCC;
    int* idx_in_cc;
    int* cc_idx;
    std::vector<ConnectedComponent*> connected_components;
    int* neighTransform;

    void initNeighTransform();

public:

    /**
     * Constructs a raster graph from a binary raster path.
     * @param raster_path Path to a binary raster.
     * @param class_value The raster value of the focal class
     * @param The neighborhood to use for connected components extraction (FOUR_CONNECTED or HEIGHT_CONNECTED).
     * @param The neighborhood to use for connected components exploration (FOUR_CONNECTED or HEIGHT_CONNECTED).
     */
    explicit RasterGraph(const char *raster_path, int class_value, Neighborhood neighborhoodExtractCC,
                         Neighborhood neighborhoodExploreCC);

    ~RasterGraph();

    /**
     * Given a `node` and a `direction` (among `LEFT`, `TOP`, `RIGHT`, `BOTTOM`), return the neighbor of `node` in
     * the corresponding direction `direction`. If there is no such neighbor in the raster graph, return -1.
     * @param node A node in the raster graph.
     * @param direction A direction among (among `LEFT`, `TOP`, `RIGHT`, `BOTTOM`)
     * @return The neighbor of `node` in the direction `direction` if it exists in the raster graph, otherwise -1.
     */
    int neigh(int node, int direction);

    /**
     * Given a node in the raster graph, return an integer representing the neighTransform of this node in binary
     * representation. Values of LEFT, TOP, RIGHT BOTTOM correspond to the bit position. e.g. 6 <=> 0110 means that
     * the node has top (TOP = 1) and a right (RIGHT = 2) neighTransform.
     * @param node A node in the raster graph.
     * @return The neighTransform of `node` as a binary mask.
     */
    int neighMask(int node);

    /**
     * Return the left neighbor of `node` if it exists, otherwise -1.
     * @param node A node in the raster graph.
     * @return The left neighbor of `node` if it exists, otherwise -1.
     */
    int leftNeigh(int node);

    /**
     * Return the top neighbor of `node` if it exists, otherwise -1.
     * @param node A node in the raster graph.
     * @return The top neighbor of `node` if it exists, otherwise -1.
     */
    int topNeigh(int node);

    /**
     * Return the right neighbor of `node` if it exists, otherwise -1.
     * @param node A node in the raster graph.
     * @return The right neighbor of `node` if it exists, otherwise -1.
     */
    int rightNeigh(int node);

    /**
     * Return the bottom neighbor of `node` if it exists, otherwise -1.
     * @param node A node in the raster graph.
     * @return The bottom neighbor of `node` if it exists, otherwise -1.
     */
    int bottomNeigh(int node);

    int topLeftNeigh(int node);
    int topRightNeigh(int node);
    int bottomRightNeigh(int node);
    int bottomLeftNeigh(int node);

    /**
     * Extract and export a connected component into a a tif file.
     * @param cc The index of the connected component to export.
     * @param dest The destination file.
     */
    void export_connected_component(int cc, char* dest);

    /**
     * Uses GDAL's polygonize algorithm to export the raster to a shapefile,
     * with connected component intra metrics as feature attributes.
     * @param dest The destination file
     */
    void polygonize(const char* dest);

    /**
     * @return The width of input raster.
     */
    int getRasterWidth() const;

    /**
     * @return The height of input raster.
     */
    int getRasterHeight() const;

    /**
     * @return The size of input raster.
     */
    int getRasterSize() const;

    /**
     * @return The area of the landscape (in geographical units).
     */
    double getLandscapeArea() const;

    /**
     * @return The number of nodes in the raster graph.
     */
    int getNumberOfNodes() const;

    /**
     * @return The number of connected components in the raster graph.
     */
    int getNumberOfConnectedComponents() const;

    /**
     * @return The size of the largest connected component in the raster graph.
     */
    int getSizeOfLargestConnectedComponent() const;

    /**
    * @return The size of the smalles connected component in the raster graph.
    */
    int getSizeOfSmallestConnectedComponent() const;

    /**
     * @param cc_index The index of the connected component to retrieve.
     * @return A pointer to the connected component at index `cc_index`.
     */
    ConnectedComponent* getConnectedComponent(int cc_index);

    /**
     * @param point A Point (OGRPoint)
     * @return The index of the pixel where `point` is located.
     */
    int getPixelIndex(OGRPoint* point);

    /**
     * @param pixel_index A pixel index
     * @return The index of the connected component containing this pixel. If the pixel is not located
     *          in a connected component, return -1.
     */
    int getConnectedComponentIndex(int pixel_index);

    /**
     * @param raster_pixel_index Index from this CC's raster
     * @return The index of raster_pixel_index within this connected component.
     */
    int getIndexInConnectedComponent(int raster_pixel_index);

};
