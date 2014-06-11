#ifndef APPLY_ALL_H
#define APPLY_ALL_H

define Fn_apply(type, fn, ...) {                                        \
    void *stopper_for_apply = (int[]){0};                               \
    type **list_for_apply = (type*[]){__VA_ARGS__, stopper_for_apply};  \
    for (int i=0; list_for_apply[i] != stopper_for_apply; i++)          \
    fn(list_for_apply[i]);                                              \
}

#endif //APPLY_ALL_H
