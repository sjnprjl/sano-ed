#include "sano.h"
#include <ncurses.h>
#include <stdio.h>

int main(int argc, char **argv) {

  (void)argc;
  (void)argv;

  Editor ed = init_editor();

  int ch;
  initscr();
  keypad(stdscr, true);
  raw();
  noecho();

  EventAction action = _WAIT;
  while (action != AC_Quit && (ch = getch())) {
    action = handle_key_event(ch);
    switch (action) {
    case AC_Quit:
      break;
    case AC_CharAdd:
      editor_insert_char(&ed, ch);
      break;

    case AC_NewLine:
      editor_add_new_line(&ed);
      break;

    case AC_MoveLeft:
      editor_move_left(&ed);
      break;
    case AC_MoveRight:
      editor_move_right(&ed);
      break;
    case AC_BackSpace:
      editor_delete_char(&ed);
      break;
    case AC_MoveUp:
      editor_move_up(&ed);
      break;
    case AC_MoveDown:
      editor_move_down(&ed);
      break;
    case _WAIT:
      break;
    }

    editor_update(&ed);
    refresh();
  }

  printf("%s", ed.current_line->data->data);

  echo();
  endwin();

  return 0;
}
