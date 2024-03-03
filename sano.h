#ifndef SANO_H
#define SANO_H
#ifndef INIT_GAP_BUF_SIZE
#define INIT_GAP_BUF_SIZE 256
#endif

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
} EventAction;

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

  /* current line  */
  unsigned int cursor_y;

} Editor;

/* func def*/

Line *create_new_line();
Editor init_editor();

void insert_char_into_line_at(Line *line, unsigned int cursor_pos, char ch);
EventAction handle_key_event(int key);
void editor_insert_char(Editor *ed, char ch);

/* create new line and return pointer to it. */
Line *add_line_after_to(Line *current);

void editor_add_new_line(Editor *ed);
void editor_move_left(Editor *ed);
void editor_move_right(Editor *ed);
void iter_lines(Line *line, void (*proc)(char, int, int));
void editor_update(Editor *ed);
#endif // !SANO_H
