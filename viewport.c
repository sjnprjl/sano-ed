#include "sano.h"

#define _width(viewport) (viewport->x + viewport->width)
#define _height(viewport) (viewport->y + viewport->height)
#define _is_inside_viewport_x(viewport, x) (x >= vp->x && x <= _width(viewport))
#define _is_inside_viewport_y(viewport, y)                                     \
  (y >= vp->y && y <= _height(viewport))

int is_inside_viewport(Viewport *vp, u_int64_t y, u_int64_t x) {
  return _is_inside_viewport_y(vp, y) && _is_inside_viewport_x(vp, x);
}

void scroll_viewport_if_possible(Viewport *vp, u_int64_t y, u_int64_t x) {
  if (is_inside_viewport(vp, y, x))
    return;

  int64_t delta_x = 0;
  int64_t delta_y = 0;

  if (y < vp->y) {
    delta_y = (int64_t)(y - vp->y);
  } else if (y > _height(vp)) {
    delta_y = (int64_t)(y - _height(vp));
  }

  if (x < vp->x) {
    delta_x = (int64_t)(x - vp->x);
  } else if (x > vp->width) {
    delta_x = (int64_t)(x - _width(vp));
  }

  vp->x += delta_x;
  vp->y += delta_y;
}
