#include "sano.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

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
  Line *new_line = add_line_after_crfc(ed->current_line, ed->cursor_x);
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
  } else if (key == KEY_UP) {
    return AC_MoveUp;
  } else if (key == KEY_DOWN) {
    return AC_MoveDown;
  } else if (key == KEY_MOUSE) {
    return AC_MouseEv;
  }

  return AC_CharAdd;
}

void ed_cursor_clamp(Editor *ed) {
  const unsigned int len = ln_get_len(ed->current_line);
  if (ed->cursor_x > len) {
    ed->cursor_x = len;
  }
}

void editor_move_left(Editor *ed) {
  if (ed->cursor_x != 0) {
    ed->cursor_x--;
  }
  //
}

void editor_move_up(Editor *ed) {
  if (ed->cursor_y != 0 && ed->current_line->prev) {
    ed->cursor_y--;
    ed->current_line = ed->current_line->prev;
    ed_cursor_clamp(ed);
  }
}

void editor_move_down(Editor *ed) {
  if (ed->current_line->next) {
    ed->cursor_y++;
    ed->current_line = ed->current_line->next;
    ed_cursor_clamp(ed);
  }
}

void ed_movexy(Editor *ed, unsigned int y, unsigned int x) {

  unsigned int delta = y < ed->cursor_y ? ed->cursor_y - y : y - ed->cursor_y;
  if (y < ed->cursor_y) {
    while (delta-- && ed->current_line) {
      ed->current_line = ed->current_line->prev;
    }
    ed->cursor_y = y;
  } else if (y > ed->cursor_y) {
    while (delta-- && ed->current_line) {
      ed->current_line = ed->current_line->next;
    }
    ed->cursor_y = y;
  }
  ed->cursor_x = x;
  ed_cursor_clamp(ed);
  move(ed->cursor_y, ed->cursor_x);
}

void editor_move_right(Editor *ed) {
  ed->cursor_x++;
  ed_cursor_clamp(ed);
  //
}

void editor_update(Editor *ed) {
  unsigned int x = ed->cursor_x;
  unsigned int y = ed->cursor_y;
  unsigned int ty = ed->viewport->y;

  if (ed->viewport->y > 0) {
    y -= ed->viewport->y;
  }
  if (ed->viewport->x > 0) {
    x -= ed->viewport->x;
  }

  Line *line = ed->line_head;

  /*  */
  while (ty-- && line) {
    line = line->next;
  }

  unsigned int _y = 0;
  while (line && _y < ed->viewport->height) {
    move(_y++, 0);
    clrtoeol();
    char *str = get_buf_as_str(line->data);
    printw("%s", str);
    free(str);
    line = line->next;
  }
  move(y, x);
}
void editor_delete_char(Editor *ed) {
  if (ed->cursor_x != 0) {
    line_delete_char(ed->current_line, ed->cursor_x);
    ed->cursor_x--;
  }
  //
}

void editor_load_from_file(Editor *ed, FILE *file) {
  char ch;
  while ((ch = getc(file)) != EOF) {
    if (ch == '\n') {
      add_line_after(ed->current_line);
      editor_move_down(ed);
      ed->cursor_x = 0;
      //
    } else {
      //
      insert_char_into_line_at(ed->current_line, ed->cursor_x++, ch);
    }
    //
  }
  /* reset to top  */
  ed->cursor_x = 0;
  ed->cursor_y = 0;
  ed->current_line = ed->line_head;
}

void editor_dump(Editor *ed) {
  //
  Line *line = ed->line_head;

  while (line) {
    char *str = get_buf_as_str(line->data);
    printf("%s\n", str);
    free(str);
    line = line->next;
  }
}
