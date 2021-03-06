/*
 * framework/core/l-display-text.c
 *
 * Copyright(c) 2007-2019 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <framework/core/l-display-text.h>

static const char display_text_lua[] = X(
local M = Class(DisplayObject)

function M:init(font, size, pattern, text)
	self.font = font
	self.pattern = pattern or Pattern.color()
	self.text = Text.new(self.font, self.pattern, text or "")
	self.super:init(0, 0, self.text)
	self:setFontSize(size or 10)
end

function M:setFont(font)
	if font then
		self.font = font
		self.text:setFont(font)
	end
	return self
end

function M:setFontSize(x, y)
	self:setScale(x, y or x)
	return self
end

function M:setPattern(pattern)
	self.pattern = pattern or Pattern.color()
	self.text:setPattern(self.pattern)
	return self
end

function M:setText(text)
	self.text:setText(text or "")
	return self
end

return M
);

int luaopen_display_text(lua_State * L)
{
	if(luaL_loadbuffer(L, display_text_lua, sizeof(display_text_lua) - 1, "DisplayText.lua") == LUA_OK)
		lua_call(L, 0, 1);
	return 1;
}
