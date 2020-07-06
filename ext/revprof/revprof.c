/* vim: set ts=8 sw=4 sts=4 et : */

#include <stdio.h>

#include <ruby.h>

void Init_revprof();
VALUE foo();

void
Init_revprof() {
  /* class Revprof */
  VALUE mRevprof = rb_define_module("Revprof");
  VALUE cProfiler = rb_define_class_under(mRevprof, "Profiler", rb_cObject);

  /* def foo */
  rb_define_method(cProfiler, "foo", foo, 0);
}

VALUE
foo() {
  printf("Hello world from C!\n");
  return Qnil;
}
