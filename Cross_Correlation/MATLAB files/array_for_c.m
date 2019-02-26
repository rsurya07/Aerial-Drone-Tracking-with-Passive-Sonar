clc
clear
x = audioread('l.wav');
y = audioread('r.wav');
samples = 512;

for i=100001:100000+samples
    x1(i-100000) = x(i);
    y1(i-100000) = y(i);
end
x1=x1';
y1=y1';



fileID = fopen('data.c','w+');
s = "#include <stdlib.h>";
fprintf(fileID, "%s\n", s);
s = '#include "arm_math.h"';
fprintf(fileID, "%s\n\n\n",s);
s = "float testInputA_f32[512] = {";
fprintf(fileID, "%s", s);

for i=1:samples
    fprintf(fileID,"%f, ", x1(i));
    
    if(rem(i,100) == 0)
        fprintf(fileID,"\n");
    end
end
s = "};";
fprintf(fileID, "%s", s);
fprintf(fileID,"\n\n\n");
s = "float testInputB_f32[512] = {";
fprintf(fileID, "%s", s);

for i=1:samples
    fprintf(fileID,"%f, ", y1(i));
    
    if(rem(i,100) == 0)
        fprintf(fileID,"\n");
    end
end
s = "};";
fprintf(fileID, "%s", s);
fclose(fileID);
