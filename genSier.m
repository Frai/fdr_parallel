function out = genSier(a, b, c, n)
    if n == 0
        out.xvals = [a(1), b(1), c(1)];
        out.yvals = [a(2), b(2), c(2)];
    else
        out1 = genSier(a, (a+b)/2, (a+c)/2, n-1);
        out2 = genSier(b, (a+b)/2, (b+c)/2, n-1);
        out3 = genSier(c, (a+c)/2, (b+c)/2, n-1);
        out = [out1, out2, out3];
    end
end