fileID = fopen('leddata.c','w+');
s = "uint8_t direction[245] = {";
fprintf(fileID, "%s", s);

for i=1:245
    fprintf(fileID,"%f, ", mics(i));
    
    if(rem(i,50) == 0)
        fprintf(fileID,"\n");
    end
end
s = "};";
fprintf(fileID, "%s", s);
fprintf(fileID,"\n\n\n");
s = "uint8_t height[245] = {";
fprintf(fileID, "%s", s);

for i=1:245
    fprintf(fileID,"%f, ", heights(i));
    
    if(rem(i,50) == 0)
        fprintf(fileID,"\n");
    end
end
s = "};";
fprintf(fileID, "%s", s);
fclose(fileID);