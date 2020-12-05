

f0 = 0;
f1 = 1;
d0 = -100;
d1 = -100;


d = f0;
c = d0;
a = d1 - d0 - 2 * (f1 - c - d);
b = f1 - a - d - c;

x = [0:0.01:1];

for i = 1:1:length(x)
   p = x(i); 
   y(i) = a * (p * p * p) + b * (p * p) + c * (p) + d;
end

plot(x, y)
