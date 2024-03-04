#include "sano.h"
#include <ncurses.h>

Editor init_editor() {

  Editor ed = (Editor){
      .line_head = create_new_line(),
      .cursor_x = 0,
      .cursor_y = 0,
      .line_length = 1,
  };

  ed.current_line = ed.line_head;

  return ed;
}

void editor_insert_char(Editor *ed, char ch) {
  insert_char_into_line_at(ed->current_line, ed->cursor_x++, ch);
  //
}

void editor_add_new_line(Editor *ed) {
  Line *new_line = add_line_after_to(ed->current_line);
  ed->current_line = new_line;
  ed->cursor_y++;
  ed->cursor_x = 0;

  //
}

EventAction handle_key_event(int key) {
  if (key == 'q')
    return AC_Quit;
  else if (key == KEY_LEFT) {
    return AC_MoveLeft;
  } else if (key == KEY_RIGHT) {
    return AC_MoveRight;
  } else if (key == '\n') {
    return AC_NewLine;
  } else if (key == KEY_BACKSPACE) {
    return AC_BackSpace;
  }

  return AC_CharAdd;
}
void editor_move_left(Editor *ed) {
  if (ed->cursor_x != 0) {
    ed->cursor_x--;
  }
  //
}

void editor_move_up(Editor *ed) {
  if (ed->cursor_y != 0) {
    ed->cursor_y--;
    ed->current_line = ed->current_line->prev;
  }
}

void editor_move_right(Editor *ed) {
  ed->cursor_x++;
  //
}

void updater(char ch, int y, int x) {
  mvaddch(y, x, ch);

  //
}

void editor_update(Editor *ed) {
  clear();
  iter_lines(ed->line_head, updater);
  move(ed->cursor_y, ed->cursor_x);

  //
}
void editor_delete_char(Editor *ed) {
  if (ed->cursor_x != 0) {
    line_delete_char(ed->current_line, ed->cursor_x);
    ed->cursor_x--;
  }
  //
}
