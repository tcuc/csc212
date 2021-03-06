#include "KDTree.h"
#include <math.h>
#include <iostream>

KDNode::KDNode(double lat, double lon, const char *desc) {
    left = NULL;
    right = NULL;
    depth = 0;
    description = desc;
    latitude = lat;
    longitude = lon;
}

KDNode::~KDNode() {
}

// returns the distance (in miles) beween this node and a given lat and lon
double KDNode::distance(double lat, double lon) {
    double param = M_PI / 180.0; // required for conversion from degrees to radians
    double rad = 3956.0;  // radius of earth in miles
    double d_lat = (lat - latitude) * param;
    double d_lon = (lon - longitude) * param;
    double dist = sin(d_lat/2) * sin(d_lat/2) + cos(latitude*param) * cos(lat*param) * sin(d_lon/2) * sin(d_lon/2);
    dist = 2.0 * atan2(sqrt(dist), sqrt(1-dist));
    return rad * dist;
}

KDTree::KDTree() {
    root = NULL;
    size = 0;
}

KDTree::~KDTree() {
    destroy(root);
}

void KDTree::destroy(KDNode *p) {
    if(!p){
        return;
    }
    destroy(p->left);
    destroy(p->right);
    delete(p);
    return;
}


void KDTree::insert(double lat, double lon, const char *desc) {
    KDNode* p=new KDNode(lat,lon,desc);
    ++size;
    if(insert(p,root)){
        p->depth=0;
        root=p;  
    }
}
bool KDTree::insert(KDNode* p, KDNode* r){
    if(!r){
        return true;
    }
    p->depth++;
    if((r->depth%2)){
        if( p->longitude <= r->longitude){
            if(insert(p,r->left)){
                r->left=p;
            }
        }
        else{
            if(insert(p,r->right)){
                r->right=p;
            }
        }
    }
    else{
        if( p->latitude <= r->latitude){
            if(insert(p,r->left)){
                r->left=p;
            }
        }
        else{
            if(insert(p,r->right)){
                r->right=p;
            }
        }
    }
    return false;
}

unsigned int KDTree::printNeighbors(double lat, double lon, double rad, const char *filter){
    std::cout << "var markers = [\n";
    std::cout << "\t[\"" << "CENTER" << "\", " << lat << ", " << lon << "],\n";
    unsigned int counterOfResults = printNeighbors(lat,lon,rad,filter,root);
    std::cout << "];\n";
    return counterOfResults;
}

unsigned int KDTree::printNeighbors(double lat, double lon, double rad, const char *filter, KDNode* c){
    if(!c){
        return 0;
    }
    unsigned int counterOfResults=0;
    if(c->distance(lat,lon)<=rad){
        if(c->description.find(filter) != std::string::npos) {
            std::cout << "\t[\"" << c->description << "\", " << c->latitude << ", " << c->longitude << "],\n";
            ++counterOfResults;
        }
        counterOfResults += printNeighbors(lat,lon,rad,filter,c->right);
        counterOfResults += printNeighbors(lat,lon,rad,filter,c->left);
    }

    else{
        if(c->depth%2){
            if((c->longitude) <= (lon) ){
                //std::cout << "lon seach right, depth: "<< c->depth << std::endl;
                counterOfResults += printNeighbors(lat,lon,rad,filter,c->right);
            }
            else{
                //std::cout << "lon seach left, depth: "<< c->depth << std::endl;
                counterOfResults += printNeighbors(lat,lon,rad,filter,c->left);
            }
        }

        else{
            if((c->latitude) <= (lat) ){
                //std::cout << "lat seach right, depth: "<< c->depth << std::endl;
                counterOfResults += printNeighbors(lat,lon,rad,filter,c->right);
            }
            else{
                //std::cout << "lat seach left, depth: "<< c->depth << std::endl;
                counterOfResults += printNeighbors(lat,lon,rad,filter,c->left);
            }
        }
    }
    return counterOfResults;
}


unsigned int KDTree::getSize(){
    return size;
}

void KDTree::printleft(){
    printleft(root);
}
void KDTree::printleft(KDNode* p){
    if(!p){
        return;
    }
    std::cout << p->description << "  - "  << p->latitude << ","<< p->longitude << " depth:"<< p->depth << std::endl;
    printleft(p->left);
}