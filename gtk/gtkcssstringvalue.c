/* GTK - The GIMP Toolkit
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "gtkcssstringvalueprivate.h"

#include <string.h>

struct _GtkCssValue {
  GTK_CSS_VALUE_BASE
  char *string;
};

static void
gtk_css_value_string_free (GtkCssValue *value)
{
  g_slice_free (GtkCssValue, value);
}

static gboolean
gtk_css_value_string_equal (const GtkCssValue *value1,
                            const GtkCssValue *value2)
{
  return g_strcmp0 (value1->string, value2->string) == 0;
}

static GtkCssValue *
gtk_css_value_string_transition (GtkCssValue *start,
                                 GtkCssValue *end,
                                 double       progress)
{
  return NULL;
}

static void
gtk_css_value_string_print (const GtkCssValue *value,
                            GString           *str)
{
  char *string = value->string;
  gsize len;

  g_string_append_c (str, '"');

  do {
    len = strcspn (string, "\"\n\r\f");
    g_string_append_len (str, string, len);
    string += len;
    switch (*string)
      {
      case '\0':
        break;
      case '\n':
        g_string_append (str, "\\A ");
        break;
      case '\r':
        g_string_append (str, "\\D ");
        break;
      case '\f':
        g_string_append (str, "\\C ");
        break;
      case '\"':
        g_string_append (str, "\\\"");
        break;
      case '\\':
        g_string_append (str, "\\\\");
        break;
      default:
        g_assert_not_reached ();
        break;
      }
  } while (*string);

  g_string_append_c (str, '"');
}

static const GtkCssValueClass GTK_CSS_VALUE_STRING = {
  gtk_css_value_string_free,
  gtk_css_value_string_equal,
  gtk_css_value_string_transition,
  gtk_css_value_string_print
};

GtkCssValue *
_gtk_css_string_value_new (const char *string)
{
  return _gtk_css_string_value_new_take (g_strdup (string));
}

GtkCssValue *
_gtk_css_string_value_new_take (char *string)
{
  GtkCssValue *result;

  result = _gtk_css_value_new (GtkCssValue, &GTK_CSS_VALUE_STRING);
  result->string = string;

  return result;
}

GtkCssValue *
_gtk_css_string_value_parse (GtkCssParser *parser)
{
  char *s;

  g_return_val_if_fail (parser != NULL, NULL);

  s = _gtk_css_parser_read_string (parser);
  if (s == NULL)
    return NULL;
  
  return _gtk_css_string_value_new_take (s);
}

const char *
_gtk_css_string_value_get (const GtkCssValue *value)
{
  g_return_val_if_fail (value != NULL, NULL);
  g_return_val_if_fail (value->class == &GTK_CSS_VALUE_STRING, NULL);

  return value->string;
}
