#include "sano.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GapBuffer *create_new_gap_buf(unsigned long size) {
  GapBuffer *gap_buf = malloc(sizeof(GapBuffer));

  char *buf = malloc(sizeof(char) * size);

  if (buf == NULL) {
    // TODO!
    printf("Could not allocate buffer while creating gap buffer\n");
    exit(1);
  }

  gap_buf->data = buf;

  gap_buf->start = 0;
  gap_buf->end = size;
  gap_buf->length = size;

  return gap_buf;
}

int get_buffer_total_filled_len(GapBuffer *buf) {
  int gap = buf->end - buf->start;
  return buf->length - gap;
}

char get_buf_next_char(GapBuffer *buf, int index) {
  int gap = buf->end - buf->start;
  if (index < 0)
    return '\0';
  if (index < (signed)buf->start)
    return buf->data[index];

  /* TODO!: this might break? */
  if ((unsigned int long)(index + gap) > buf->length)
    return '\0';
  return buf->data[index + gap];
}

void iter_buffer(GapBuffer *buf, void (*proc)(char, int)) {
  int index = 0;

  char ch;
  while ((ch = get_buf_next_char(buf, index))) {
    proc(ch, index++);
  }
}

void iter_lines(Line *head, void (*proc)(char ch, int y, int x)) {
  int y = 0;
  int x = 0;
  while (head) {
    int index = 0;
    char ch;
    while ((ch = get_buf_next_char(head->data, index++))) {
      proc(ch, y, x++);
    }
    head = head->next;
    y++;
    x = 0;
  }
}

void try_gap_shift(GapBuffer *gbuf, unsigned int at) {
  const int gap = (signed)(gbuf->end - gbuf->start);
  if (gap <= 0)
    return;
  if (at < gbuf->start && (signed)at >= 0) {
    const unsigned int delta = gbuf->start - at;
    memcpy(&gbuf->data[gbuf->end - delta], &gbuf->data[at], delta);
    gbuf->start -= delta;
    gbuf->end -= delta;
  } else if (at > gbuf->start && at + gap - 1 < gbuf->length) {
    const int unsigned pos = at + gap - 1;

    /*  think!: pos is always going to be greater than gbuf->end?  */
    const int delta = (signed int)(pos - gbuf->end) + 1; //?;
    memcpy(&gbuf->data[gbuf->start], &gbuf->data[gbuf->end], delta);
    gbuf->start += delta;
    gbuf->end += delta;
  }

  //
}

void grow_gap_buffer(GapBuffer *gbuf) {

  const int old_size = gbuf->length;
  const int new_size = (old_size * 2) * sizeof(char);

  char *b = realloc(gbuf->data, new_size);

  if (b == NULL) {
    printf("Could not reallocate. Do not have sufficient RAM\n");
    exit(1);
  }
  gbuf->data = b;

  int delta = gbuf->length - gbuf->end;
  memcpy(&gbuf->data[new_size - delta], &gbuf->data[gbuf->end], delta);

  gbuf->length = new_size;
  gbuf->end = gbuf->length - delta;
}

void insert_into_gap_buffer_at(GapBuffer *gbuf, unsigned int at, char ch) {

  if ((signed int)(gbuf->end - gbuf->start) <= 0) {
    grow_gap_buffer(gbuf);
  }

  try_gap_shift(gbuf, at);
  gbuf->data[gbuf->start++] = ch;

  //
}

void insert_char_into_line_at(Line *line, unsigned int cursor_pos, char ch) {
  insert_into_gap_buffer_at(line->data, cursor_pos, ch);
  //
}

void free_gap_buffer(GapBuffer *gap_buf) {
  FREE(gap_buf->data);
  FREE(gap_buf);
}

Line *create_new_line() {
  Line *line = malloc(sizeof(Line));
  if (!line) {
    /* TODO!: graceful exit */
    printf("Could not allocate a new line\n");
    exit(1);
  }
  line->data = create_new_gap_buf(INIT_GAP_BUF_SIZE);

  return line;
}

void free_only_this_line(Line *this) {
  free_gap_buffer(this->data);
  FREE(this);
}

void line_delete_char(Line *this, unsigned int at) {
  /* subtracting start will act as the character is deleted. */
  if (at != 0) {
    try_gap_shift(this->data, at);
    if (this->data->start != 0)
      this->data->start--;
  }
}

static inline void relate_two_adj_nodes(Line *behind, Line *next_to_behind) {

  if (!behind || !next_to_behind)
    return;

  next_to_behind->prev = behind;
  behind->next = next_to_behind;
}

void add_line_prev_to(Line *current) {
  Line *old_prev = current->prev;
  Line *to_insert = create_new_line();

  relate_two_adj_nodes(to_insert, current);
  /*  if it was in the middle */
  if (old_prev)
    relate_two_adj_nodes(old_prev, to_insert);
}

Line *add_line_after_to(Line *current) {
  Line *old_next = current->next;
  Line *to_insert = create_new_line();

  relate_two_adj_nodes(current, to_insert);

  if (old_next) {
    relate_two_adj_nodes(to_insert, old_next);
  }
  return to_insert;
}

void delete_line(Line *this) {
  Line *prev_line = this->prev;
  Line *next_line = this->next;
  relate_two_adj_nodes(prev_line, next_line);
  free_only_this_line(this);
}
