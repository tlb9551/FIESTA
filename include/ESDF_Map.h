//
// Created by tommy on 12/17/18.
//

#ifndef ESDF_TOOLS_ESDF_MAP_H
#define ESDF_TOOLS_ESDF_MAP_H

#include <Eigen/Eigen>
#include <iostream>
#include <visualization_msgs/Marker.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include <pcl/kdtree/kdtree_flann.h>
#include <algorithm>
#include <sensor_msgs/PointCloud.h>
//#define HASH_TABLE
#define BLOCK
#define BITWISE

class ESDF_Map {
private:
    struct QueueElement {
        Eigen::Vector3i point;
        double distance;
    };

// region DIRECTION

//    const static int dirNum = 6; // only faces
//    const Eigen::Vector3i dir[dirNum] = {Eigen::Vector3i(-1, 0, 0), Eigen::Vector3i(1, 0, 0),
//                                         Eigen::Vector3i(0, -1, 0), Eigen::Vector3i(0, 1, 0),
//                                         Eigen::Vector3i(0, 0, -1), Eigen::Vector3i(0, 0, 1)};

//    const static int dirNum = 18; // faces & enges
//    const Eigen::Vector3i dir[dirNum] = {Eigen::Vector3i(-1, 0, 0), Eigen::Vector3i(1, 0, 0),
//                                         Eigen::Vector3i(0, -1, 0), Eigen::Vector3i(0, 1, 0),
//                                         Eigen::Vector3i(0, 0, -1), Eigen::Vector3i(0, 0, 1),
//
//                                         Eigen::Vector3i(-1, -1, 0), Eigen::Vector3i(1, 1, 0),
//                                         Eigen::Vector3i(0, -1, -1), Eigen::Vector3i(0, 1, 1),
//                                         Eigen::Vector3i(-1, 0, -1), Eigen::Vector3i(1, 0, 1),
//                                         Eigen::Vector3i(-1, 1, 0), Eigen::Vector3i(1, -1, 0),
//                                         Eigen::Vector3i(0, -1, 1), Eigen::Vector3i(0, 1, -1),
//                                         Eigen::Vector3i(1, 0, -1), Eigen::Vector3i(-1, 0, 1)};

//    const static int dirNum = 26; // faces & edges & vertices
//    const Eigen::Vector3i dir[dirNum] = {Eigen::Vector3i(-1, 0, 0), Eigen::Vector3i(1, 0, 0),
//                                         Eigen::Vector3i(0, -1, 0), Eigen::Vector3i(0, 1, 0),
//                                         Eigen::Vector3i(0, 0, -1), Eigen::Vector3i(0, 0, 1),
//
//                                         Eigen::Vector3i(-1, -1, 0), Eigen::Vector3i(1, 1, 0),
//                                         Eigen::Vector3i(0, -1, -1), Eigen::Vector3i(0, 1, 1),
//                                         Eigen::Vector3i(-1, 0, -1), Eigen::Vector3i(1, 0, 1),
//                                         Eigen::Vector3i(-1, 1, 0), Eigen::Vector3i(1, -1, 0),
//                                         Eigen::Vector3i(0, -1, 1), Eigen::Vector3i(0, 1, -1),
//                                         Eigen::Vector3i(1, 0, -1), Eigen::Vector3i(-1, 0, 1),
//
//                                         Eigen::Vector3i(-1, -1, -1), Eigen::Vector3i(1, 1, 1),
//                                         Eigen::Vector3i(-1, -1, 1), Eigen::Vector3i(1, 1, -1),
//                                         Eigen::Vector3i(-1, 1, -1), Eigen::Vector3i(1, -1, 1),
//                                         Eigen::Vector3i(1, -1, -1), Eigen::Vector3i(-1, 1, 1)};

    const static int dirNum = 24; // faces 2 steps
    const Eigen::Vector3i dir[dirNum] = {Eigen::Vector3i(-1, 0, 0), Eigen::Vector3i(1, 0, 0),
                                         Eigen::Vector3i(0, -1, 0), Eigen::Vector3i(0, 1, 0),
                                         Eigen::Vector3i(0, 0, -1), Eigen::Vector3i(0, 0, 1),

                                         Eigen::Vector3i(-1, -1, 0), Eigen::Vector3i(1, 1, 0),
                                         Eigen::Vector3i(0, -1, -1), Eigen::Vector3i(0, 1, 1),
                                         Eigen::Vector3i(-1, 0, -1), Eigen::Vector3i(1, 0, 1),
                                         Eigen::Vector3i(-1, 1, 0), Eigen::Vector3i(1, -1, 0),
                                         Eigen::Vector3i(0, -1, 1), Eigen::Vector3i(0, 1, -1),
                                         Eigen::Vector3i(1, 0, -1), Eigen::Vector3i(-1, 0, 1),

                                         Eigen::Vector3i(-2, 0, 0), Eigen::Vector3i(2, 0, 0),
                                         Eigen::Vector3i(0, -2, 0), Eigen::Vector3i(0, 2, 0),
                                         Eigen::Vector3i(0, 0, -2), Eigen::Vector3i(0, 0, 2)};

//    const static int dirNum = 32; // faces 2 steps & cornor
//    const Eigen::Vector3i dir[dirNum] = {Eigen::Vector3i(-1, 0, 0), Eigen::Vector3i(1, 0, 0),
//                                         Eigen::Vector3i(0, -1, 0), Eigen::Vector3i(0, 1, 0),
//                                         Eigen::Vector3i(0, 0, -1), Eigen::Vector3i(0, 0, 1),
//
//                                         Eigen::Vector3i(-1, -1, 0), Eigen::Vector3i(1, 1, 0),
//                                         Eigen::Vector3i(0, -1, -1), Eigen::Vector3i(0, 1, 1),
//                                         Eigen::Vector3i(-1, 0, -1), Eigen::Vector3i(1, 0, 1),
//                                         Eigen::Vector3i(-1, 1, 0), Eigen::Vector3i(1, -1, 0),
//                                         Eigen::Vector3i(0, -1, 1), Eigen::Vector3i(0, 1, -1),
//                                         Eigen::Vector3i(1, 0, -1), Eigen::Vector3i(-1, 0, 1),
//
//                                         Eigen::Vector3i(-2, 0, 0), Eigen::Vector3i(2, 0, 0),
//                                         Eigen::Vector3i(0, -2, 0), Eigen::Vector3i(0, 2, 0),
//                                         Eigen::Vector3i(0, 0, -2), Eigen::Vector3i(0, 0, 2),
//
//                                         Eigen::Vector3i(-1, -1, -1), Eigen::Vector3i(1, 1, 1),
//                                         Eigen::Vector3i(-1, -1, 1), Eigen::Vector3i(1, 1, -1),
//                                         Eigen::Vector3i(-1, 1, -1), Eigen::Vector3i(1, -1, 1),
//                                         Eigen::Vector3i(1, -1, -1), Eigen::Vector3i(-1, 1, 1)
//    };

//    const static int dirNum = 62; // faces 3 steps
//    const Eigen::Vector3i dir[dirNum] = {Eigen::Vector3i(-1, 0, 0), Eigen::Vector3i(1, 0, 0),
//                                         Eigen::Vector3i(0, -1, 0), Eigen::Vector3i(0, 1, 0),
//                                         Eigen::Vector3i(0, 0, -1), Eigen::Vector3i(0, 0, 1),
//
//                                         Eigen::Vector3i(-1, -1, 0), Eigen::Vector3i(1, 1, 0),
//                                         Eigen::Vector3i(0, -1, -1), Eigen::Vector3i(0, 1, 1),
//                                         Eigen::Vector3i(-1, 0, -1), Eigen::Vector3i(1, 0, 1),
//                                         Eigen::Vector3i(-1, 1, 0), Eigen::Vector3i(1, -1, 0),
//                                         Eigen::Vector3i(0, -1, 1), Eigen::Vector3i(0, 1, -1),
//                                         Eigen::Vector3i(1, 0, -1), Eigen::Vector3i(-1, 0, 1),
//
//                                         Eigen::Vector3i(-1, -1, -1), Eigen::Vector3i(1, 1, 1),
//                                         Eigen::Vector3i(-1, -1, 1), Eigen::Vector3i(1, 1, -1),
//                                         Eigen::Vector3i(-1, 1, -1), Eigen::Vector3i(1, -1, 1),
//                                         Eigen::Vector3i(1, -1, -1), Eigen::Vector3i(-1, 1, 1),
//
//                                         Eigen::Vector3i(-2, 0, 0), Eigen::Vector3i(2, 0, 0),
//                                         Eigen::Vector3i(0, -2, 0), Eigen::Vector3i(0, 2, 0),
//                                         Eigen::Vector3i(0, 0, -2), Eigen::Vector3i(0, 0, 2),
//
//                                         Eigen::Vector3i(-2, -1, 0), Eigen::Vector3i(2, 1, 0),
//                                         Eigen::Vector3i(0, -2, -1), Eigen::Vector3i(0, 2, 1),
//                                         Eigen::Vector3i(-2, 0, -1), Eigen::Vector3i(2, 0, 1),
//                                         Eigen::Vector3i(-2, 1, 0), Eigen::Vector3i(2, -1, 0),
//                                         Eigen::Vector3i(0, -2, 1), Eigen::Vector3i(0, 2, -1),
//                                         Eigen::Vector3i(2, 0, -1), Eigen::Vector3i(-2, 0, 1),
//
//                                         Eigen::Vector3i(-1, -2, 0), Eigen::Vector3i(1, 2, 0),
//                                         Eigen::Vector3i(0, -1, -2), Eigen::Vector3i(0, 1, 2),
//                                         Eigen::Vector3i(-1, 0, -2), Eigen::Vector3i(1, 0, 2),
//                                         Eigen::Vector3i(-1, 2, 0), Eigen::Vector3i(1, -2, 0),
//                                         Eigen::Vector3i(0, -1, 2), Eigen::Vector3i(0, 1, -2),
//                                         Eigen::Vector3i(1, 0, -2), Eigen::Vector3i(-1, 0, 2),
//
//                                         Eigen::Vector3i(-3, 0, 0), Eigen::Vector3i(3, 0, 0),
//                                         Eigen::Vector3i(0, -3, 0), Eigen::Vector3i(0, 3, 0),
//                                         Eigen::Vector3i(0, 0, -3), Eigen::Vector3i(0, 0, 3)};

// endregion

#ifdef HASH_TABLE

    template<typename T>
    struct matrix_hash : std::unary_function<T, size_t> {
        std::size_t operator()(T const &matrix) const {
            // Note that it is oblivious to the storage order of Eigen matrix (column- or
            // row-major). It will give you the same hash value for two different matrices if they
            // are the transpose of each other in different storage order.
            size_t seed = 0;
            for (size_t i = 0; i < matrix.size(); ++i) {
                auto elem = *(matrix.data() + i);
                seed ^= std::hash<typename T::Scalar>()(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    void increaceCapacity(int &oldSize, int newSize) {
        occupancyBuffer.resize(newSize);
        distanceBuffer.resize(newSize);
        closestObstacle.resize(newSize);
        voxBuffer.resize(newSize);
        tmp1.resize(newSize);
        tmp2.resize(newSize);

        std::fill(distanceBuffer.begin() + oldSize, distanceBuffer.end(), (double) undefined);
//    std::fill(distanceBufferNegative.begin(), distanceBufferNegative.end(), (double) undefined);
        std::fill(occupancyBuffer.begin() + oldSize, occupancyBuffer.end(), 0.5);
        std::fill(closestObstacle.begin() + oldSize, closestObstacle.end(),
                  Eigen::Vector3i(undefined, undefined, undefined));
        std::fill(voxBuffer.begin() + oldSize, voxBuffer.end(), Eigen::Vector3i(undefined, undefined, undefined));
        std::fill(tmp1.begin() + oldSize, tmp1.end(), 0);
        std::fill(tmp2.begin() + oldSize, tmp2.end(), 0);

        head.resize(newSize);
        prev.resize(newSize);
        next.resize(newSize);
        std::fill(head.begin() + oldSize, head.end(), undefined);
        std::fill(prev.begin() + oldSize, prev.end(), undefined);
        std::fill(next.begin() + oldSize, next.end(), undefined);

        printf("\tnewSize:\t%d\n", newSize);
        oldSize = newSize;
    }

#ifdef BLOCK

    int findAndInsert(Eigen::Vector3i hashKey) {
        if (count + blockSize > reserveSize)
            increaceCapacity(reserveSize, reserveSize * 2);
#ifdef BITWISE
        int idxInBlock = ((hashKey.x() & (block - 1)) << blockBit << blockBit)
                         + ((hashKey.y() & (block - 1)) << blockBit)
                         + (hashKey.z() & (block - 1));
        Eigen::Vector3i blockId = hashKey.unaryExpr([&](const int x) { return x >> blockBit; });
#else
        int idxInBlock = (hashKey.x() % block + block) % block * block * block
                         + (hashKey.y() % block + block) % block * block
                         + (hashKey.z() % block + block) % block;
        Eigen::Vector3i blockId = hashKey.unaryExpr([&](const int x) { return x / block - (x % block < 0); });
#endif
        auto tmp = hashTable.find(blockId);

        if (tmp == hashTable.end()) {
//            if (count < reserveSize)
            hashTable.insert(std::make_pair(blockId, count));
//            voxBuffer[count] = blockId;
#ifdef BITWISE
            blockId = blockId.unaryExpr([&](const int x) { return x << blockBit; });
#else
            blockId = blockId * block;
#endif
            for (int i = 0; i < block; i++)
                for (int j = 0; j < block; j++)
                    for (int k = 0; k < block; k++) {
                        voxBuffer[count++] = blockId + Eigen::Vector3i(i, j, k);
                    }
            return count - blockSize + idxInBlock;
        } else {
            return tmp->second + idxInBlock;
        }
    }

#else

    int findAndInsert(Eigen::Vector3i hashKey) {
        if (count >= reserveSize)
            increaceCapacity(reserveSize, reserveSize * 2);
        auto tmp = hashTable.find(hashKey);

        if (tmp == hashTable.end()) {
    //            if (count < reserveSize)
            hashTable.insert(std::make_pair(hashKey, count));
            voxBuffer[count] = hashKey;
            return count++;
        } else {
            return tmp->second;
        }
    }

#endif

    std::unordered_map<Eigen::Vector3i, int, matrix_hash<Eigen::Vector3i>> hashTable;
    int count, reserveSize;
#ifdef BLOCK
    int block, blockSize, blockBit;
#endif
    std::vector<Eigen::Vector3i> voxBuffer;

#endif
// data are saved in vector
    std::vector<double> occupancyBuffer;  // 0 is free, 1 is occupied
    std::vector<double> distanceBuffer;
//    std::vector<double> distanceBufferNegative;

    std::vector<int> tmp1, tmp2;
    std::vector<Eigen::Vector3i> closestObstacle;
    std::vector<int> head, prev, next;


// TODO: replace the Vector3i to a int, i.e. vox -> idx. Space -> Time


    std::queue<QueueElement> insertQueue;
    std::queue<QueueElement> deleteQueue;
    std::queue<QueueElement> updateQueue;
// map property
    Eigen::Vector3d origin;
    int special4undefined;
#ifndef HASH_TABLE
    Eigen::Vector3d map_size;
    Eigen::Vector3d min_range, max_range;  // map range in pos
    Eigen::Vector3i grid_size;             // map range in index
    int grid_size_yz;
#endif

    int inf, undefined;
    double resolution;

    bool exist(int idx);

    bool posInMap(Eigen::Vector3d pos);

    bool voxInMap(Eigen::Vector3i vox);


    void vox2pos(Eigen::Vector3i vox, Eigen::Vector3d &pos);

    int vox2idx(Eigen::Vector3i vox);

    int vox2idx(Eigen::Vector3i vox, int subsampling_factor);

    Eigen::Vector3i idx2vox(int idx);

    double dist(Eigen::Vector3i a, Eigen::Vector3i b);

    void deleteFromList(int link, int idx);

    void insertIntoList(int link, int idx);


public:
#ifdef HASH_TABLE

    ESDF_Map(Eigen::Vector3d
             origin,
             double resolution,
             int reserveSize = 0
    );

#else
    int grid_total_size;

    ESDF_Map(Eigen::Vector3d origin, double resolution, Eigen::Vector3d map_size);

#endif


    ~ESDF_Map() {
    }

    void pos2vox(Eigen::Vector3d pos, Eigen::Vector3i &vox);

    bool updateOccupancy();

    void updateESDF(Eigen::Vector3i observationPoint = Eigen::Vector3i(10000, 10000, 10000));

// occupancy management
    int setOccupancy(Eigen::Vector3d pos, int occ = 1);

    int setOccupancy(Eigen::Vector3i posId, int occ = 1);

    int getOccupancy(Eigen::Vector3d pos);

    int getOccupancy(Eigen::Vector3i posId);

// distance field management
    double getDistance(Eigen::Vector3d pos);

// visualization
    void getPointCloud(sensor_msgs::PointCloud &m);



    void getOccupancyMarker(visualization_msgs::Marker &m, int id, Eigen::Vector4d color);

    void getESDFMarker(std::vector<visualization_msgs::Marker> &markers, int id, Eigen::Vector3d color);

    void getSliceMarker(visualization_msgs::Marker &m, int slice, int id, Eigen::Vector4d color);

    int pos2idx(Eigen::Vector3d pos) {
        Eigen::Vector3i vox;
        pos2vox(pos, vox);
        return vox2idx(vox);
    }

#ifdef DEBUG
// only for test, check whether consistent
    bool check() {
#ifdef HASH_TABLE
        for (int ii = 1; ii < count; ii++) {
            int vox = vox2idx(voxBuffer[ii]);
            if (prev[vox] != undefined && next[prev[vox]] != vox || next[vox] != undefined && prev[next[vox]] != vox) {
                std::cout << voxBuffer[ii](0) << ' ' << voxBuffer[ii](1) << ' ' << voxBuffer[ii](2) << ' ' << vox
                          << std::endl;
                std::cout << prev[vox] << ' ' << next[prev[vox]] << ' ' << next[vox] << ' ' << prev[next[vox]]
                          << std::endl;
                std::cout << vox2idx(closestObstacle[next[prev[vox]]]) << ' ' << vox2idx(closestObstacle[vox])
                          << std::endl;
                std::cout << vox2idx(closestObstacle[prev[vox]]) << ' ' << vox2idx(closestObstacle[next[vox]])
                          << std::endl;

                return false;
            }
            if (prev[vox] == undefined && distanceBuffer[vox] >= 0
                && head[vox2idx(closestObstacle[vox])] != vox)
                return false;
        }

#else

        for (int x = 0; x < grid_size(0); ++x)
            for (int y = 0; y < grid_size(1); ++y)
                for (int z = 0; z < grid_size(2); ++z) {
                    int vox = vox2idx(Eigen::Vector3i(x, y, z));
                    if (prev[vox] != undefined && next[prev[vox]] != vox ||
                        next[vox] != undefined && prev[next[vox]] != vox) {
                        std::cout << x << ' ' << y << ' ' << z << ' ' << vox << std::endl;
                        std::cout << prev[vox] << ' ' << next[prev[vox]] << ' ' << next[vox] << ' ' << prev[next[vox]]
                                  << std::endl;
                        std::cout << vox2idx(closestObstacle[next[prev[vox]]]) << ' ' << vox2idx(closestObstacle[vox])
                                  << std::endl;
                        std::cout << vox2idx(closestObstacle[prev[vox]]) << ' ' << vox2idx(closestObstacle[next[vox]])
                                  << std::endl;

                        return false;
                    }
                    if (prev[vox] == undefined && distanceBuffer[vox] >= 0
                        && head[vox2idx(closestObstacle[vox])] != vox)
                        return false;
                }
#endif
        return true;
    }

// only for test, check whether consistent
    bool checkWithGT() {
#ifdef HASH_TABLE
//        ESDF_Map *esdf_map = new ESDF_Map(Eigen::Vector3d(0, 0, 0), resolution, 10000000);
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        cloud->width = 0;
        cloud->height = 1;
        for (int ii = 1; ii < count; ii++)
            if (exist(ii))
                cloud->width++;
        cloud->points.resize(cloud->width * cloud->height);
        int tot = 0;
        for (int ii = 1; ii < count; ii++)
            if (exist(ii)) {
                cloud->points[tot].x = voxBuffer[ii](0);
                cloud->points[tot].y = voxBuffer[ii](1);
                cloud->points[tot].z = voxBuffer[ii](2);
                tot++;
            }
        pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
        kdtree.setInputCloud(cloud);
        std::vector<int> pointIdxNKNSearch(1);
        std::vector<float> pointNKNSquaredDistance(1);
        int cnt1 = 0, cnt2 = 0;
        double ems1 = 0, ems2 = 0, max1 = 0, max2 = 0;
        int a[30];
        std::fill(a, a + 30, 0);
        for (int ii = 1; ii < count; ii++) {
            if (distanceBuffer[ii] >= 0) {
                kdtree.nearestKSearch(pcl::PointXYZ(voxBuffer[ii](0), voxBuffer[ii](1), voxBuffer[ii](2)), 1,
                                      pointIdxNKNSearch, pointNKNSquaredDistance);
                double tmp = sqrt(pointNKNSquaredDistance[0]) * resolution;
//                if (fabs(distanceBuffer[ii] - tmp) > 1e-3) {
//                    std::cout << voxBuffer[ii](0) << '\t' << voxBuffer[ii](1) << '\t' << voxBuffer[ii](2) << '\n';
//                    std::cout << cloud->points[pointIdxNKNSearch[0]].x << '\t'
//                              << cloud->points[pointIdxNKNSearch[0]].y << '\t'
//                              << cloud->points[pointIdxNKNSearch[0]].z << '\n';
//
//                    std::cout << distanceBuffer[ii] << '\t' << tmp << '\n';
//
//                    return false;
                double error = distanceBuffer[ii] - tmp;
                if (error > 1e-3) {
                    cnt1++;
                    a[(int) (error / 0.1)]++;
                }
                ems1 += distanceBuffer[ii] - tmp;
                ems2 += (distanceBuffer[ii] - tmp) * (distanceBuffer[ii] - tmp);
                cnt2++;
                max1 = std::max(distanceBuffer[ii] - tmp, max1);
//                }
            }
        }

        std::cout << count << std::endl;
        std::cout << ems1 << " / " << cnt1 << " = " << ems1 / cnt1 << std::endl;
        std::cout << ems1 << " / " << cnt2 << " = " << ems1 / cnt2 << std::endl;
        std::cout << ems2 << " / " << cnt1 << " = " << ems2 / cnt1 << std::endl;
        std::cout << ems2 << " / " << cnt2 << " = " << ems2 / cnt2 << std::endl;
        std::cout << "max = " << max1 << std::endl;
        for (int i = 0; i < 30; i++) {
            std::cout << " [ " << i * 0.1 << ", " << i * 0.1 + 0.1 << " ]\t" << a[i] << std::endl;
        }

#else

        for (int x = 0; x < grid_size(0); ++x)
            for (int y = 0; y < grid_size(1); ++y)
                for (int z = 0; z < grid_size(2); ++z) {
                    int vox = vox2idx(Eigen::Vector3i(x, y, z));
                    if (prev[vox] != undefined && next[prev[vox]] != vox ||
                        next[vox] != undefined && prev[next[vox]] != vox) {
                        std::cout << x << ' ' << y << ' ' << z << ' ' << vox << std::endl;
                        std::cout << prev[vox] << ' ' << next[prev[vox]] << ' ' << next[vox] << ' ' << prev[next[vox]]
                                  << std::endl;
                        std::cout << vox2idx(closestObstacle[next[prev[vox]]]) << ' ' << vox2idx(closestObstacle[vox])
                                  << std::endl;
                        std::cout << vox2idx(closestObstacle[prev[vox]]) << ' ' << vox2idx(closestObstacle[next[vox]])
                                  << std::endl;

                        return false;
                    }
                    if (prev[vox] == undefined && distanceBuffer[vox] >= 0
                        && head[vox2idx(closestObstacle[vox])] != vox)
                        return false;
                }
#endif
        return true;
    }
#endif
};


#endif //ESDF_TOOLS_ESDF_MAP_H