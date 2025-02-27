/****************************************************************************
 * apps/graphics/pdcurs34/pdcurses/pdc_touch.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Adapted from the original public domain pdcurses by Gregory Nutt
 ****************************************************************************/

/* Name: touch
 *
 * Synopsis:
 *       int touchwin(WINDOW *win);
 *       int touchline(WINDOW *win, int start, int count);
 *       int untouchwin(WINDOW *win);
 *       int wtouchln(WINDOW *win, int y, int n, int changed);
 *       bool is_linetouched(WINDOW *win, int line);
 *       bool is_wintouched(WINDOW *win);
 *
 * Description:
 *       touchwin() and touchline() throw away all information about
 *       which parts of the window have been touched, pretending that the
 *       entire window has been drawn on.  This is sometimes necessary
 *       when using overlapping windows, since a change to one window
 *       will affect the other window, but the records of which lines
 *       have been changed in the other window will not reflect the
 *       change.
 *
 *       untouchwin() marks all lines in the window as unchanged since
 *       the last call to wrefresh().
 *
 *       wtouchln() makes n lines in the window, starting at line y, look
 *       as if they have (changed == 1) or have not (changed == 0) been
 *       changed since the last call to wrefresh().
 *
 *       is_linetouched() returns true if the specified line in the
 *       specified window has been changed since the last call to
 *       wrefresh().
 *
 *       is_wintouched() returns true if the specified window
 *       has been changed since the last call to wrefresh().
 *
 * Return Value:
 *       All functions return OK on success and ERR on error except
 *       is_wintouched() and is_linetouched().
 *
 * Portability                                X/Open    BSD    SYS V
 *       touchwin                                Y       Y       Y
 *       touchline                               Y       -      3.0
 *       untouchwin                              Y       -      4.0
 *       wtouchln                                Y       Y       Y
 *       is_linetouched                          Y       -      4.0
 *       is_wintouched                           Y       -      4.0
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "curspriv.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int touchwin(WINDOW *win)
{
  int i;

  PDC_LOG(("touchwin() - called: Win=%x\n", win));

  if (!win)
    {
      return ERR;
    }

  for (i = 0; i < win->_maxy; i++)
    {
      win->_firstch[i] = 0;
      win->_lastch[i]  = win->_maxx - 1;
    }

  return OK;
}

int touchline(WINDOW *win, int start, int count)
{
  int i;

  PDC_LOG(("touchline() - called: win=%p start %d count %d\n",
           win, start, count));

  if (!win || start > win->_maxy || start + count > win->_maxy)
    {
      return ERR;
    }

  for (i = start; i < start + count; i++)
    {
      win->_firstch[i] = 0;
      win->_lastch[i]  = win->_maxx - 1;
    }

  return OK;
}

int untouchwin(WINDOW *win)
{
  int i;

  PDC_LOG(("untouchwin() - called: win=%p", win));

  if (!win)
    {
      return ERR;
    }

  for (i = 0; i < win->_maxy; i++)
    {
      win->_firstch[i] = _NO_CHANGE;
      win->_lastch[i]  = _NO_CHANGE;
    }

  return OK;
}

int wtouchln(WINDOW *win, int y, int n, int changed)
{
  int i;

  PDC_LOG(("wtouchln() - called: win=%p y=%d n=%d changed=%d\n",
           win, y, n, changed));

  if (!win || y > win->_maxy || y + n > win->_maxy)
    {
      return ERR;
    }

  for (i = y; i < y + n; i++)
    {
      if (changed)
        {
          win->_firstch[i] = 0;
          win->_lastch[i]  = win->_maxx - 1;
        }
      else
        {
          win->_firstch[i] = _NO_CHANGE;
          win->_lastch[i]  = _NO_CHANGE;
        }
    }

  return OK;
}

bool is_linetouched(WINDOW *win, int line)
{
  PDC_LOG(("is_linetouched() - called: win=%p line=%d\n", win, line));

  if (!win || line > win->_maxy || line < 0)
    {
      return false;
    }

  return (win->_firstch[line] != _NO_CHANGE) ? true : false;
}

bool is_wintouched(WINDOW *win)
{
  int i;

  PDC_LOG(("is_wintouched() - called: win=%p\n", win));

  if (win)
    {
      for (i = 0; i < win->_maxy; i++)
        {
          if (win->_firstch[i] != _NO_CHANGE)
            {
              return true;
            }
        }
    }

  return false;
}
