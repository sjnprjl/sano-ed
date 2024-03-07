#ifndef SANO_H
#define SANO_H
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#define INIT_GAP_BUF_SIZE 256
#define internal static

#define FREE(ptr)                                                              \
  do {                                                                         \
    if (ptr)                                                                   \
      free(ptr);                                                               \
  } while (0)

typedef enum EventAction {
  _WAIT = 1,
  AC_Quit,
  AC_NewLine,
  AC_CharAdd,
  AC_MoveLeft,
  AC_MoveRight,
  AC_BackSpace,
  AC_MoveUp,
  AC_MoveDown,
  AC_MouseEv,
} EventAction;

typedef struct _viewport_t {
  u_int64_t x;
  u_int64_t y;
  u_int64_t width;
  u_int64_t height;
} Viewport;

typedef struct _GapBuffer {
  char *data;

  unsigned int start;
  unsigned int end;

  unsigned long length;

} GapBuffer;

typedef struct Node {
  GapBuffer *data;

  struct Node *prev;
  struct Node *next;
} Line;

typedef struct _Editor {
  Line *line_head;
  Line *current_line;

  unsigned int line_length;
  unsigned int cursor_x;
  unsigned int cursor_y; /* current line  */

  Viewport *viewport;
} Editor;

/* ==============
 * | Gap Buffer |
 * ==============
 * */
void try_gap_shift(GapBuffer *gbuf, unsigned int at);
char *get_buf_as_str(GapBuffer *buf);

/*
 * =================================
 * |          Line                 |
 * =================================
 *
 * */

Line *create_new_line();
Line *add_line_after(Line *current);
Line *add_line_before(Line *current);
Line *add_line_after_crfc(Line *current, unsigned int);

void line_delete_char(Line *, unsigned int at);
void insert_char_into_line_at(Line *line, unsigned int cursor_pos, char ch);

unsigned int ln_get_len(Line *ln);

/*
 * =================================
 * |         Editor                |
 * =================================
 *
 * */
Editor init_editor();

void editor_insert_char(Editor *ed, char ch);
void editor_add_new_line(Editor *ed);
void editor_move_left(Editor *ed);
void editor_move_right(Editor *ed);
void editor_move_down(Editor *ed);
void editor_move_up(Editor *ed);
void editor_update(Editor *ed);
void editor_delete_char(Editor *ed);
void editor_load_from_file(Editor *ed, FILE *file);
void ed_movexy(Editor *ed, unsigned int y, unsigned int x);
void editor_dump(Editor *ed);

/*
 * ===========
 * | Viewport|
 * ==========
 * */

int is_inside_viewport(Viewport *vp, u_int64_t y, u_int64_t x);

void scroll_viewport_if_possible(Viewport *vp, u_int64_t y, u_int64_t x);

/*
 * ===========
 * | Action |
 * ==========
 * */

EventAction handle_key_event(int key);
#endif // !SANO_H
