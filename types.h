#pragma once
typedef unsigned long long ulong64;
typedef void* vptr;
typedef unsigned int uint32;
typedef const char* str;
typedef char* strbuf;
typedef unsigned char byte;

enum class InputType { KeyDown, KeyUp, MouseDown, MouseUp };

struct Color {
	byte r, g, b, a;
	Color(uint32 rgb, byte a = 255) : r((rgb & 0xFF0000) >> 16), g((rgb & 0x00FF00) >> 8), b(rgb & 0x0000FF), a(a) {}
	Color(byte r, byte g, byte b, byte a = 255) : r(r), g(g), b(b), a(a) {}
};