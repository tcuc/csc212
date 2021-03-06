#ifndef __kdtree__
#define __kdtree__
#include <string>

class KDNode {
    private:
        double latitude;
        double longitude;
        int depth;
        std::string description;
        KDNode *left;
        KDNode *right;

        // this function returns the distance in miles from lat, lon
        // to this object
        double distance(double lat, double lon);

    public:
        KDNode(double lat, double lon, const char *desc);
        ~KDNode();

    friend class KDTree;
};

class KDTree {
    private:
        unsigned int size;
        KDNode *root;

        void destroy(KDNode *p);

    public:
        KDTree();
        ~KDTree();

        unsigned int getSize();
        void insert(double lat, double lon, const char *desc);
        bool insert(KDNode* p, KDNode* r);
        unsigned int printNeighbors(double lat, double lon, double rad, const char *filter);
        unsigned int printNeighbors(double lat, double lon, double rad, const char *filter, KDNode* c);
        unsigned int printNeighbors(KDNode* p,KDNode* r,double rad);
        void printleft();
        void printleft(KDNode* p);
};

#endif
