#include "sano.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  FILE *fd = NULL;
  if (argc == 2) {
    //
    char *file_path = argv[1];
    fd = fopen(file_path, "r");
    if (!fd) {
      perror("Could not open file");
      exit(1);
    }
  }

  int ch;
  initscr();
  keypad(stdscr, true);
  raw();
  noecho();
  mousemask(BUTTON1_PRESSED, NULL);

  Editor ed = init_editor();

  if (fd != NULL) {
    editor_load_from_file(&ed, fd);
  }
  Viewport vp = {0};
  getmaxyx(stdscr, vp.height, vp.width);
  /*  TODO! */
  ed.viewport = &vp;
  editor_update(&ed);

  EventAction action = _WAIT;
  MEVENT event;
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
    case AC_MouseEv:
      // if (getmouse(&event) == OK) {
      //   if (event.bstate & BUTTON1_PRESSED) {
      //     //
      //     ed_movexy(&ed, event.y, event.x);
      //   }
      // }
      break;
    }

    scroll_viewport_if_possible(&vp, ed.cursor_y, ed.cursor_x);
    editor_update(&ed);
    refresh();
  }

  //
  echo();
  endwin();
  // printf("viewport height: %ld\n", vp.v_f_height);
  editor_dump(&ed);

  return 0;
}
