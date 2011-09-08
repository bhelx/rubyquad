#include <stdlib.h>
#include <stdio.h>
#include <ruby.h>
#include "quadtree.h"

#define RUN_TESTS 0

static int pointWithinBox(BoundingBox *bbox, Point*p);
static void addPointToNode(QuadTree *node, Point *p);
static int intersects(BoundingBox *bbox, QuadTree *tree);
static QuadTree* getSubNode(QuadTree *tree, Point *point);
static void breakTree(QuadTree *tree); 

QuadTree* createQuadTree(double north, double south, double east, double west, QuadTree *parent) {
  QuadTree *tree = ALLOC(QuadTree);
  //QuadTree *tree = (QuadTree *) malloc(sizeof(QuadTree));

  tree->west = west;
  tree->east = east;
  tree->south = south;
  tree->north = north;

  tree->nW = NULL;
  tree->sW = NULL;
  tree->sE = NULL;
  tree->nE = NULL;

  tree->parent = parent;
  tree->pointVector = createPointVector();
  tree->isBroken = 0;
  return tree;
}

Point* createPoint(double x, double y, double z) {
  Point *p = ALLOC(Point);
  //Point *p = (Point *) malloc(sizeof(Point));
  p->x = x;
  p->y = y;
  p->z = z;
  return p;
}

PointVector* createPointVector() {
  PointVector *pv = ALLOC(PointVector);
  //PointVector *pv = (PointVector *) malloc(sizeof(PointVector));
  pv->index = 0;
  return pv;
}

BoundingBox* createBoundingBox(double west, double east, double south, double north) {
  BoundingBox *bbox = ALLOC(BoundingBox);
  //BoundingBox *bbox = (BoundingBox *) malloc(sizeof(BoundingBox));
  bbox->west = west;
  bbox->east = east;
  bbox->north = north;
  bbox->south = south;
  return bbox;
}

ResultsSet* createResultsSet() {
  ResultsSet *results = ALLOC(ResultsSet);
  //ResultsSet *results = (ResultsSet *) malloc(sizeof(ResultsSet));
  results->index = 0;
  results->isFull = 0;
  return results;
}

void insert(QuadTree *tree, Point *point) {
  if (tree->isBroken) {
    insert(getSubNode(tree, point), point);
  } else {
    if (tree->pointVector->index >= MAX_POINTS) {
      breakTree(tree);
      insert(tree, point); //call again w/ this point
    } else {
      addPointToNode(tree, point);
    }
  }
}


void query(ResultsSet *results, QuadTree *tree, BoundingBox *bbox) {
  if (tree->isBroken) {
    if (tree->sW && intersects(bbox, tree->sW)) {
      query(results, tree->sW, bbox);
    }
    if (tree->sE && intersects(bbox, tree->sE)) {
      query(results, tree->sE, bbox);
    }
    if (tree->nW && intersects(bbox, tree->nW)) {
      query(results, tree->nW, bbox);
    }
    if (tree->nE && intersects(bbox, tree->nE)) {
      query(results, tree->nE, bbox);
    }
  } else {//add point vector to results set, assume we have enough space for now
    int i;
    for (i = 0; i < tree->pointVector->index; i++) {
      Point *p = tree->pointVector->points[i];
      if (pointWithinBox(bbox, p)) {
        results->points[results->index] = p;
        results->index++;
      }
    }
  }
}

/**
 * Final brute force check on points
 */
static int pointWithinBox(BoundingBox *bbox, Point *point) {
  return (point->x > bbox->west) && (point->x < bbox->east) && (point->y > bbox->south) && (point->y < bbox->north);
}

void printResults(ResultsSet *results) {
  int i;
  for (i = 0; i < results->index; i++) {
      Point *p = results->points[i];
      printf("Point: x=%f    y=%f    z=%f\n", p->x, p->y, p->z);
  }
}


/**
 * Makes deep copy of Point p
 */
static void addPointToNode(QuadTree *tree, Point *p) {
  Point *new = createPoint(p->x, p->y, p->z);
  PointVector *pv = tree->pointVector;
  pv->points[pv->index] = new;
  pv->index++;
}

/**
 * Clever intersect okmij
 */
static int intersects(BoundingBox *bbox, QuadTree *tree) {
  double d1 = bbox->east - bbox->west;
  double d2 = tree->west - bbox->east;
  double d3 = tree->east - tree->west;
  double d4 = bbox->west - tree->east;
  if (d1 < 0)
    d1 += 360;
  if (d2 < 0)
    d2 += 360;
  if (d3 < 0)
    d3 += 360;
  if (d4 < 0)
    d4 += 360;
  return ((d1 + d2 + d3 + d4) != 360);
}

static void breakTree(QuadTree *tree) {
  tree->isBroken = 1; //declare the node 'broken'
  int i;
  for (i = 0; i < MAX_POINTS; i++) {
    insert(tree, tree->pointVector->points[i]);
  }
  free(tree->pointVector);
}

static QuadTree* getSubNode(QuadTree *tree, Point *point) {
  double x = point->x;
  double y = point->y;

  double xMid = tree->west + ((tree->east - tree->west)*0.5);
  double yMid = tree->south + ((tree->north - tree->south)*0.5);
  if (x < xMid) {//left half
    if (y < yMid) {//southwest
      if (!tree->sW)
        tree->sW = createQuadTree(yMid, tree->south, xMid, tree->west, tree); //create
      return tree->sW;
    } else {//northwest
      if (!tree->nW)
        tree->nW = createQuadTree(tree->north, yMid, xMid, tree->west, tree);
      return tree->nW;
    }
  } else {//right half
    if (y < yMid) {//southeast
      if (!tree->sE)
        tree->sE = createQuadTree(yMid, tree->south, tree->east, xMid, tree);
      return tree->sE;
    } else {//northeast
      if (!tree->nE)
        tree->nE = createQuadTree(tree->north, yMid, tree->east, xMid, tree);
      return tree->nE;
    }
  }
  printf("DERP WTF! look at getSubNode() in quadtree.c \n");
  return NULL; //should never happen right?
}

#if RUN_TESTS == 1
int main() {
    QuadTree *root;
    root = createQuadTree(0.0, 450.0, 0.0, 200.0, NULL);
    printf("root->east = %f\n", root->east);

    Point *p1 = createPoint(50.0, 90.0, 100.0);
    printf("p1->x = %f\n", p1->x);

    addPointToNode(root, p1);
    printf("added\n");
    printf("root->pointVector->index = %d\n", root->pointVector->index);
    printf("root->pointVector->points[0]->x = %f\n", root->pointVector->points[0]->x);

    ResultsSet *results = createResultsSet();

    BoundingBox *bbox = createBoundingBox(30.0, 60.0, 30.0, 60.0);
    printf("bbox->west = %f\n", bbox->west);

    printf("intersects should be 1: %d\n", intersects(bbox, root));

    BoundingBox *bbox2 = createBoundingBox(-10, 30, 30.0, 60.0);
    printf("bbox2->west = %f\n", bbox2->west);

    printf("intersects should be 1: %d\n", intersects(bbox2, root));

    BoundingBox *bbox3 = createBoundingBox(-10, -5, -10, -5);
    printf("bbox3->west = %f\n", bbox3->west);

    printf("intersects should be 0: %d\n", intersects(bbox3, root));

    root->pointVector = createPointVector(); //new pointvector
    insert(root, p1);

    printf("pointx %f\n", root->pointVector->points[0]->x);

    int i;
    for (i = 0; i < 25; i++) {
        Point *p = createPoint(i*4, i*6, i*10);
        insert(root, p);
    }

    printf("nw is null? %d\n", root->nW == NULL);
    printf("sw is null? %d\n", root->sW == NULL);
    printf("se is null? %d\n", root->sE == NULL);
    printf("ne is null? %d\n", root->nE == NULL);

    printf("se point vector index: %d\n", root->nW->pointVector->index);
    printf("ne point vector index: %d\n", root->sW->pointVector->index);
    printf("root pv is null %d\n", root->pointVector == NULL);


    return 0;
}
#endif



