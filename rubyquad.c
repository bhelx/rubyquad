#include <ruby.h>
#include "quadtree.h"

static VALUE cQuadTree;

static VALUE quad_tree_init(VALUE self, VALUE north, VALUE south, VALUE east, VALUE west) {

  //rb_define_attr(self, "north", 1, 0);
  rb_iv_set(self, "@north", north);

  //rb_define_attr(self, "south", 1, 0);
  rb_iv_set(self, "@south", south);

  //rb_define_attr(self, "east", 1, 0);
  rb_iv_set(self, "@east", east);

  //rb_define_attr(self, "west", 1, 0);
  rb_iv_set(self, "@west", west);

  return self;
}

static VALUE quad_tree_add_point(VALUE self, VALUE x, VALUE y, VALUE z) {
 
  QuadTree *tree;
  Data_Get_Struct(self, QuadTree, tree);

  Point *point = createPoint(NUM2DBL(x), NUM2DBL(y), NUM2DBL(z));
  insert(tree, point);

  return self;
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

}


