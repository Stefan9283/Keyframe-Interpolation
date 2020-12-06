x = 0;
y = 0;

f0 = 0
f1 = 1


d0 = 0
d1 = 3
dd0 = 1
dd1 = 1



delta = 1,
d = (dd0/2) * delta * delta,
e = d0 * delta,
f = 0,
A = 1 - d - e - f,
B = d1 * delta - 2 * d - e,
C = dd1 * delta * delta - 2 * d,
b = - 15 * A + 7 * B  - C,
a = (B - 3 * A - b) / 2,
c = A - a - b;

x = [f0:0.1:f1];

for i = 1:1:length(x)
   p = (x(i) - f0)/delta;
   y(i) = a * (p * p * p * p * p) + b * (p * p * p * p) + c * (p * p * p) + d * (p * p) + e * (p) + f;

end


plot(x, y)
x = 0;
y = 0;