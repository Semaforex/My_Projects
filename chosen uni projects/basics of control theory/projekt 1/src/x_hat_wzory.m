syms xh1 xh2 xh3 xh4 l1 l2 l3 l4 u y

xh = [xh1; xh2; xh3; xh4];
L = [l1; l2; l3; l4];

xh_dot = A * xh + B * u + L * (y - C * xh);