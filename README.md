# Trace_File_Generator
<strong>Generate Trace File with format : W [TAB] PSN</strong>
<br>
<strong>< Build Option ></strong>
<br>
- Modify this predefined macro as you want

| Macro | For What |
|:---:|:---|
| RAND_MODE | Generate trace file with random sequence (0 : Not Use, 1 : Use) |
| OVERWRITE_COUNT | Create the same write operation as the current OVERWRITE_COUNT macro for each PSN (Default : 0) |
<br>

<strong> < Example of a generated trace file > </strong>
1) RAND_MODE 0 with OVERWRITE_COUNT 0<br>
W 0<br>
W	1<br>
W	2<br>
W	3<br>
W	4<br>
2) RAND_MODE i with OVERWRITE_COUNT 1<br>
W 1<br>
W	0<br>
W	4<br>
W	3<br>
W	2<br>
W 4<br>
W	1<br>
W	2<br>
W	3<br>
W	0<br>
  
