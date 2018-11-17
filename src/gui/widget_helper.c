﻿/*
 * widget.c -- GUI widget helper APIs.
 *
 * Copyright (c) 2018, Liu chao <lc-soft@live.cn> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/gui/widget.h>

void Widget_SetPadding(LCUI_Widget w, float top, float right,
		       float bottom, float left)
{
	Widget_SetStyle(w, key_padding_top, top, px);
	Widget_SetStyle(w, key_padding_right, right, px);
	Widget_SetStyle(w, key_padding_bottom, bottom, px);
	Widget_SetStyle(w, key_padding_left, left, px);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_SetMargin(LCUI_Widget w, float top, float right,
		      float bottom, float left)
{
	Widget_SetStyle(w, key_margin_top, top, px);
	Widget_SetStyle(w, key_margin_right, right, px);
	Widget_SetStyle(w, key_margin_bottom, bottom, px);
	Widget_SetStyle(w, key_margin_left, left, px);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_SetBorder(LCUI_Widget w, float width, int style, LCUI_Color clr)
{
	Widget_SetStyle(w, key_border_top_color, clr, color);
	Widget_SetStyle(w, key_border_right_color, clr, color);
	Widget_SetStyle(w, key_border_bottom_color, clr, color);
	Widget_SetStyle(w, key_border_left_color, clr, color);
	Widget_SetStyle(w, key_border_top_width, width, px);
	Widget_SetStyle(w, key_border_right_width, width, px);
	Widget_SetStyle(w, key_border_bottom_width, width, px);
	Widget_SetStyle(w, key_border_left_width, width, px);
	Widget_SetStyle(w, key_border_top_style, style, style);
	Widget_SetStyle(w, key_border_right_style, style, style);
	Widget_SetStyle(w, key_border_bottom_style, style, style);
	Widget_SetStyle(w, key_border_left_style, style, style);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_SetBoxShadow(LCUI_Widget w, float x, float y,
			 float blur, LCUI_Color color)
{
	Widget_SetStyle(w, key_box_shadow_x, x, px);
	Widget_SetStyle(w, key_box_shadow_y, y, px);
	Widget_SetStyle(w, key_box_shadow_blur, blur, px);
	Widget_SetStyle(w, key_box_shadow_color, color, color);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_Move(LCUI_Widget w, float left, float top)
{
	Widget_SetStyle(w, key_top, top, px);
	Widget_SetStyle(w, key_left, left, px);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_Resize(LCUI_Widget w, float width, float height)
{
	Widget_SetStyle(w, key_width, width, px);
	Widget_SetStyle(w, key_height, height, px);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_ResizeWithSurface(LCUI_Widget w, float width, float height)
{
	Widget_Resize(w, width, height);
	Widget_AddTask(w, LCUI_WTASK_RESIZE_WITH_SURFACE);
}

LCUI_Style Widget_GetStyle(LCUI_Widget w, int key)
{
	assert(key >= 0 && key < w->custom_style->length);
	return &w->custom_style->sheet[key];
}

LCUI_Style Widget_GetInheritedStyle(LCUI_Widget w, int key)
{
	assert(key >= 0 && key < w->inherited_style->length);
	return &w->inherited_style->sheet[key];
}

void Widget_SetVisibility(LCUI_Widget w, const char *value)
{
	LCUI_Style s = Widget_GetStyle(w, key_visibility);
	if (s->is_valid && s->type == LCUI_STYPE_STRING) {
		free(s->val_string);
		s->val_string = NULL;
	}
	Widget_SetStyle(w, key_visibility, strdup2(value), string);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_SetVisible(LCUI_Widget w)
{
	Widget_SetVisibility(w, "visible");
}

void Widget_SetHidden(LCUI_Widget w)
{
	Widget_SetVisibility(w, "hidden");
}

void Widget_Show(LCUI_Widget w)
{
	LCUI_Style s = Widget_GetStyle(w, key_display);

	if (s->is_valid && s->type == LCUI_STYPE_STYLE &&
	    s->val_style == SV_NONE) {
		Widget_UnsetStyle(w, key_display);
	} else if (!w->computed_style.visible) {
		s = Widget_GetInheritedStyle(w, key_display);
		if (s->is_valid && s->type == LCUI_STYPE_STYLE &&
		    s->val_style != SV_NONE) {
			Widget_SetStyle(w, key_display, s->val_style, style);
		} else {
			Widget_SetStyle(w, key_display, SV_BLOCK, style);
		}
	}
	Widget_UpdateStyle(w, FALSE);
}

void Widget_Hide(LCUI_Widget w)
{
	Widget_SetStyle(w, key_display, SV_NONE, style);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_SetPosition(LCUI_Widget w, LCUI_StyleValue position)
{
	Widget_SetStyle(w, key_position, position, style);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_SetOpacity(LCUI_Widget w, float opacity)
{
	Widget_SetStyle(w, key_opacity, opacity, scale);
	Widget_UpdateStyle(w, FALSE);
}

void Widget_SetBoxSizing(LCUI_Widget w, LCUI_StyleValue sizing)
{
	Widget_SetStyle(w, key_box_sizing, sizing, style);
	Widget_UpdateStyle(w, FALSE);
}
