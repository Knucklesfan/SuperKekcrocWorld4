#pragma once

#include <cute_c2.h>
class block {
	public:
		block(int x, int y, int w, int h, int actas) {
			block::x = x;
			block::y = y;
			width = w;
			height = h;
			block::actas = actas;
			collider.min = c2V(x, y);
			collider.max = c2V(x+w, y+h);
		}
		int x, y, width, height, actas;
		c2AABB collider;
};