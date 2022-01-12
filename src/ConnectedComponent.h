//
// Created by djusteau on 18/06/2021.
//

#include <vector>
#include "Neighborhood.h"

/**
 * Class representing a connected component of a raster graph. The internal data structure of this class is optimized
 * to compute the mean detour index through the computation of all pairs shortest paths, using a BFS.
 */
class ConnectedComponent {

private:

    // Raster related
    double pixel_height;
    double pixel_width;
    int* coordX;
    int* coordY;
    int minX;
    int maxX;
    int minY;
    int maxY;

    // Indices
    double mean_detour_index;
    int nb_pixel_edges_w;
    int nb_pixel_edges_h;

    // CC related
    int max_degree;
    int size;
    int* edges;
    int* idxInCC;
    int* neighTransform;

public:

    /**
     * Constructor of connected component.
     * @param nodes The nodes of the connected component (original raster indices).
     * @param neighbors The neighTransform of the connected component's nodes, in 4-connected binary representation.
     * @param idxInCC Indices of orginal raster cells to connected component index.
     * @param raster_width The width of the original raster.
     * @param pixel_width The width of a pixel in geographical units.
     * @param pixel_height The height of a pixel in geographical units.
     */
    ConnectedComponent(std::vector<int> &nodes, std::vector<int> &neighbors, int *idxInCC, int raster_width,
                       double pixel_width, double pixel_height, Neighborhood neighborhood);

    ~ConnectedComponent();

    /**
     * @return The size of the connected component.
     */
    int getSize() const;

    /**
     * @param node_idx A node index
     * @return The X coordinate of `node_idx` in the original raster.
     */
    int getXCoordinate(int node_idx) const;

    /**
     * @param node_idx A node index
     * @return The X coordinate of `node_idx` in the original raster.
     */
    int getYCoordinate(int node_idx) const;

    /**
     * Computes the mean detour index from a source node to all other nodes of the connected component.
     * The distance of reference is the manhattan distance in the 4-connected grid.
     * @param source Index of the source node.
     * @return The mean detour index to reach the node `source` from any other node of the connected component.
     */
    double meanDetourIndex(int source) const;

    /**
     * Computes the global mean detour index of the connected component, i.e. the average detour index between any
     * pair of nodes in the connected component.
     * @return The global mean detour index of the connected component.
     */
    double meanDetourIndex();

    /**
     * @param idxSource Index of the source node.
     * @param idxDest Index of the destination node.
     * @return The manhattan distance between the source node and the destination node.
     */
    int manDist(int idxSource, int idxDest) const;

    /**
     * @return The maximum X coordinate of the connected component, in matrix units, according to the raster graph from
     * which it was extracted.
     */
    int getMaxX() const;

    /**
     * @return The maximum Y coordinate of the connected component, in matrix units, according to the raster graph from
     * which it was extracted.
     */
    int getMaxY() const;

    /**
     * @return The minimum X coordinate of the connected component, in matrix units, according to the raster graph from
     * which it was extracted.
     */
    int getMinX() const;

    /**
     * @return The minimum Y coordinate of the connected component, in matrix units, according to the raster graph from
     * which it was extracted.
     */
    int getMinY() const;

    /**
     * @return The width of the connected component (in number of cells).
     */
    int getWidth() const;

    /**
     * @return The height of the connected component (in numbers of cells).
     */
    int getHeight() const;

    /**
     * @return The perimeter of the connected component (in geographical units).
     */
    double getPerimeter() const;

    /**
     * @return The area of the connected component (in geographical units).
     */
    double getArea() const;

    /**
     * @return The shape index of the connected component.
     */
    double getShapeIndex() const;

    /**
     * @return The fractal dimension connected component.
     */
    double getFractalDimension() const;
};
