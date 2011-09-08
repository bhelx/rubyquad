#include <ruby.h>
#include "quadtree.h"

VALUE quad_tree_init(VALUE self, VALUE north, VALUE south, VALUE east, VALUE west);
VALUE quad_tree_add_point(VALUE self, VALUE x, VALUE y, VALUE z);

VALUE quad_tree_new(VALUE class, VALUE north, VALUE south, VALUE east, VALUE west);
void Init_rubyquad();

static VALUE cQuadTree;

VALUE quad_tree_init(VALUE self, VALUE north, VALUE south, VALUE east, VALUE west) {

  rb_iv_set(self, "@north", north);
  rb_iv_set(self, "@south", south);
  rb_iv_set(self, "@east", east);
  rb_iv_set(self, "@west", west);

  return self;
}

VALUE quad_tree_add_point(VALUE self, VALUE x, VALUE y, VALUE z) {

  QuadTree *tree;
  Data_Get_Struct(self, QuadTree, tree);

  Point *point = createPoint(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z));
  insert(tree, point);

  free(point);

  return self;
}

VALUE quad_tree_south_east(VALUE self) {
  QuadTree *tree;
  Data_Get_Struct(self, QuadTree, tree);
  if (!tree->sE)
    return Qnil;

  return Data_Wrap_Struct(cQuadTree, 0, -1, tree->sE);

}

VALUE quad_tree_south_west(VALUE self) {
  QuadTree *tree;
  Data_Get_Struct(self, QuadTree, tree);
  if (!tree->sW)
    return Qnil;

  return Data_Wrap_Struct(cQuadTree, 0, -1, tree->sW);
}

VALUE quad_tree_north_east(VALUE self) {
  QuadTree *tree;
  Data_Get_Struct(self, QuadTree, tree);
  if (!tree->nE)
    return Qnil;

  return Data_Wrap_Struct(cQuadTree, 0, -1, tree->nE);
}

VALUE quad_tree_north_west(VALUE self) {
  QuadTree *tree;
  Data_Get_Struct(self, QuadTree, tree);
  if (!tree->nW)
    return Qnil;

  return Data_Wrap_Struct(cQuadTree, 0, -1, tree->nW);
}


VALUE quad_tree_new(VALUE class, VALUE north, VALUE south, VALUE east, VALUE west) {

  QuadTree *tree = createQuadTree(NUM2DBL(north), NUM2DBL(south), NUM2DBL(east), NUM2DBL(west), NULL);

  VALUE argv[4];
  argv[0] = north;
  argv[1] = south;
  argv[2] = east;
  argv[3] = west;


  VALUE tdata = Data_Wrap_Struct(class, 0, -1, tree);
  rb_obj_call_init(tdata, 4, argv);

  return tdata;
}

void Init_rubyquad() {
  cQuadTree = rb_define_class("QuadTree", rb_cObject);
  rb_define_singleton_method(cQuadTree, "new", quad_tree_new, 4);
  rb_define_method(cQuadTree, "initialize", quad_tree_init, 4);
  rb_define_method(cQuadTree, "insert", quad_tree_add_point, 3);
  rb_define_method(cQuadTree, "south_east", quad_tree_south_east, 0);
  rb_define_method(cQuadTree, "south_west", quad_tree_south_west, 0);
  rb_define_method(cQuadTree, "north_east", quad_tree_north_east, 0);
  rb_define_method(cQuadTree, "north_west", quad_tree_north_west, 0);

}


