/* templates.c
 * Copyright (C) 2013 Openismus GmbH
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Tristan Van Berkom <tristanvb@openismus.com>
 */
#include <gtk/gtk.h>

static void
test_dialog_basic (void)
{
  GtkWidget *dialog;

  dialog = gtk_dialog_new();
  g_assert (GTK_IS_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

static void
test_dialog_override_property (void)
{
  GtkWidget *dialog;

  dialog = g_object_new (GTK_TYPE_DIALOG,
			 "type-hint", GDK_WINDOW_TYPE_HINT_UTILITY,
			 NULL);
  g_assert (GTK_IS_DIALOG (dialog));
  g_assert (gtk_window_get_type_hint (GTK_WINDOW (dialog)) == GDK_WINDOW_TYPE_HINT_UTILITY);

  gtk_widget_destroy (dialog);
}

static void
test_message_dialog_basic (void)
{
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (NULL, 0,
				   GTK_MESSAGE_INFO,
				   GTK_BUTTONS_CLOSE,
				   "Do it hard !");
  g_assert (GTK_IS_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

int
main (int argc, char **argv)
{
  /* initialize test program */
  gtk_test_init (&argc, &argv);

  /* This environment variable cooperates with gtk_widget_destroy()
   * to assert that all automated compoenents are properly finalized
   * when a given composite widget is destroyed.
   */
  g_assert (g_setenv ("GTK_WIDGET_ASSERT_COMPONENTS", "1", TRUE));

  g_test_add_func ("/Template/GtkDialog/Basic", test_dialog_basic);
  g_test_add_func ("/Template/GtkDialog/OverrideProperty", test_dialog_override_property);
  g_test_add_func ("/Template/GtkMessageDialog/Basic", test_message_dialog_basic);

  return g_test_run();
}