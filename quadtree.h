#define MAX_POINTS 20
#define RESOLUTION 0.25
#define RESULTS_SET_DEFAULT_ALLOCATION 1000000 // 1 million needs to be made dynamic, based off number points in system

/** TYPES **/
struct point {
    double x, y, z;
};
typedef struct point Point;

struct point_vector {
    Point *points[MAX_POINTS]; 
    int index;
};
typedef struct point_vector PointVector;

struct quad_tree {
    struct quad_tree *parent, *nW, *nE, *sE, *sW;
    double west, east, south, north;
    int isBroken; 
    PointVector *pointVector;
};
typedef struct quad_tree QuadTree;

struct bounding_box {
    double west, east, south, north, e2, w2;
};
typedef struct bounding_box BoundingBox;

struct results_set {
    Point *points[RESULTS_SET_DEFAULT_ALLOCATION];
    int index;
    int isFull;
};
typedef struct results_set ResultsSet;

/** FUNCTION PROTOTYPES **/
QuadTree* createQuadTree(double west, double east, double south, double north, QuadTree *parent);
Point* createPoint(double x, double y, double z);
PointVector* createPointVector(void);
BoundingBox* createBoundingBox(double west, double east, double south, double north);
ResultsSet* createResultsSet(void);
void insert(QuadTree *tree, Point *point);
void query(ResultsSet *results, QuadTree *tree, BoundingBox *bbox);
void printResults(ResultsSet *results);

