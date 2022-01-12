//
// Created by djusteau on 18/06/2021.
//

#include <complex>
#include <c++/9/algorithm>
#include "ConnectedComponent.h"

ConnectedComponent::ConnectedComponent(std::vector<int> &nodes, std::vector<int> &neighbors, int *idxInCC,
                                       int raster_width, double pixel_width, double pixel_height, Neighborhood neighborhood) {
    this->size = nodes.size();
    this->idxInCC = idxInCC;
    this->pixel_height = pixel_height;
    this->pixel_width = pixel_width;
    this->max_degree = neighborhood;
    this->neighTransform = new int[max_degree];

    switch (max_degree) {
        case FOUR_CONNECTED:
            neighTransform[LEFT] = -1;
            neighTransform[TOP] = -raster_width;
            neighTransform[RIGHT] = 1;
            neighTransform[BOTTOM] = raster_width;
            break;
        case HEIGHT_CONNECTED:
            neighTransform[LEFT] = -1;
            neighTransform[TOP] = -raster_width;
            neighTransform[RIGHT] = 1;
            neighTransform[BOTTOM] = raster_width;
            neighTransform[TOP_LEFT] = - 1 - raster_width;
            neighTransform[TOP_RIGHT] = 1 - raster_width;
            neighTransform[BOTTOM_RIGHT] = 1 + raster_width;
            neighTransform[BOTTOM_LEFT] = raster_width - 1;
            break;
    }

    this->edges = new int[size * (max_degree + 1)];
    this->coordX = new int[size];
    this->coordY = new int[size];
    this->minX = -1;
    this->maxX = 0;
    this->minY = -1;
    this->maxY = 0;
    this->nb_pixel_edges_w = 0;
    this->nb_pixel_edges_h = 0;
    this->mean_detour_index = -1;
    // DESCRIPTION OF THE DATA STRUCTURE:
    //
    for (int i = 0; i < size; i++) {
        edges[(max_degree + 1) * i] = nodes[i];
        int deg = 0;
        for (int k = 0; k < max_degree; k++) {
            if (neighbors[i] & (1 << k)) {
                deg++;
                edges[(max_degree + 1) * i + deg] = idxInCC[nodes[i] + neighTransform[k]];
            } else {
                switch (k) {
                    case LEFT:
                        nb_pixel_edges_h++;
                        break;
                    case TOP:
                        nb_pixel_edges_w++;
                        break;
                    case RIGHT:
                        nb_pixel_edges_h++;
                        break;
                    case BOTTOM:
                        nb_pixel_edges_w++;
                }
            }
        }
        for (int k = deg; k < max_degree; k++) {
            edges[(max_degree + 1) * i + k + 1] = -1;
        }
        coordX[i] = nodes[i] % raster_width;
        coordY[i] = nodes[i] / raster_width;
        if (minX == -1 || coordX[i] < minX) {
            minX = coordX[i];
        }
        if (minY == -1 || coordY[i] < minY) {
            minY = coordY[i];
        }
        if (maxX < coordX[i]) {
            maxX = coordX[i];
        }
        if (maxY < coordY[i]) {
            maxY = coordY[i];
        }
    }
}

ConnectedComponent::~ConnectedComponent() {
    delete[] edges;
    delete[] coordX;
    delete[] coordY;
    delete[] neighTransform;
}

double ConnectedComponent::meanDetourIndex() {
    if (this->mean_detour_index != -1) {
        return this->mean_detour_index;
    }
    if (size == 1) {
        this->mean_detour_index = 1.0;
        return 1.0;
    }
    double mdi = 0;

    #pragma omp parallel for reduction(+: mdi)
    for (int source = 0; source < size; source++) {
        mdi += meanDetourIndex(source);
    }
    double sizeD = size;
    this->mean_detour_index = mdi / (sizeD * (sizeD - 1));
    return mean_detour_index;
}

double ConnectedComponent::meanDetourIndex(int source) const {

    double dmdi = 0;

    bool* visited = new bool[size]; for (int i = size; i--;) { visited[i] = false; }
    int* queue = new int[size];
    int* dist = new int[size];

    int front = 0;
    int rear = 0;
    int current;

    visited[source] = true;
    queue[front] = source;
    rear++;
    dist[source] = 0;

    while(front < rear) {
        current = queue[front++];
        for (int k = 0; k < max_degree; k++) {
            int i = edges[(max_degree + 1) * current + k + 1];
            if (i < 0) {
                break;
            }
            if (!visited[i]) {
                dist[i] = (dist[current] + 1);
                queue[rear++] = i;
                visited[i] = true;
                int m = manDist(source, i);
                dmdi += 1.0 * m / dist[i];
            }
        }
    }

    delete[] dist;
    delete[] queue;
    delete[] visited;

    return dmdi;
}

#pragma omp declare simd
int ConnectedComponent::manDist(int idxSource, int idxDest) const {
    int ax = coordX[idxSource];
    int ay = coordY[idxSource];
    int bx = coordX[idxDest];
    int by = coordY[idxDest];
    int dx = std::abs(ax - bx);
    int dy = std::abs(ay - by);
    switch (max_degree) {
        case 4:
            return dx + dy;
        case 8:
            return std::min(dx, dy) + std::abs(dx - dy);
    }
}

int ConnectedComponent::getMaxX() const {
    return maxX;
}

int ConnectedComponent::getMaxY() const {
    return maxY;
}

int ConnectedComponent::getMinX() const {
    return minX;
}

int ConnectedComponent::getMinY() const {
    return minY;
}

int ConnectedComponent::getWidth() const {
    return maxX - minX + 1;
}

int ConnectedComponent::getHeight() const {
    return maxY - minY + 1;
}

double ConnectedComponent::getPerimeter() const {
    return nb_pixel_edges_w * pixel_width + nb_pixel_edges_h * pixel_height;
}

double ConnectedComponent::getArea() const {
    return size * (pixel_width * pixel_height);
}

double ConnectedComponent::getShapeIndex() const {
    double n = std::floor(std::sqrt(getArea()));
    double m = size - (n * n);
    if (m == 0) {
        return getPerimeter() / (4 * n);
    }
    if ((n * n) < size && size <= n * (n + 1)) {
        return getPerimeter() / (4 * n + 2);
    }
    return getPerimeter() / (4 * n + 4);
}

int ConnectedComponent::getSize() const {
    return size;
}

int ConnectedComponent::getXCoordinate(int node_idx) const {
    return coordX[node_idx];
}

int ConnectedComponent::getYCoordinate(int node_idx) const {
    return coordY[node_idx];
}

double ConnectedComponent::getFractalDimension() const {
    return 2 * std::log(0.25 * getPerimeter()) / std::log(getArea());
}
