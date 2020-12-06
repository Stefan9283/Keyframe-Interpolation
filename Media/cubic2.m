

f0 = 8;
f1 = 10;

x = [f0:0.01:f1];

for i = 1:1:length(x)
   p = x(i); 
   y(i) =(p * p * p);
end

plot(x, y)
