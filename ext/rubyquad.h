#include <ruby.h>

VALUE cQuadTree;
void Init_rubyquad();
VALUE quad_tree_init(VALUE self, VALUE north, VALUE south, VALUE east, VALUE west);
VALUE quad_tree_new(VALUE class, VALUE north, VALUE south, VALUE east, VALUE west);

VALUE quad_tree_south_east(VALUE self);
VALUE quad_tree_south_west(VALUE self);
VALUE quad_tree_north_west(VALUE self);
VALUE quad_tree_north_east(VALUE self);

VALUE quad_tree_add_point(VALUE self, VALUE x, VALUE y, VALUE z);
VALUE quad_tree_points_within(VALUE self, VALUE north, VALUE south, VALUE east, VALUE west);


