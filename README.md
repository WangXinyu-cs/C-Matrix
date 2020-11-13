# 1 Introduction
&nbsp;&nbsp;&nbsp;&nbsp;This tiny program is a simple implementation for matrix calculation. I had ever been an employee of Oracle CDC as a JDBC/UCP QA tester for 21 months which, although a very short time, is the most peaceful and happy days during my career life. This little program is my tribute to Oracle whose greatness goes far beyond its products.
# 2 General information
&nbsp;&nbsp;&nbsp;&nbsp;C-Matrix is written in C++ using a few C++ 11 features and it needs a compiler supporting these features, I use gcc 10.2.1 on Fedora 32 to compile it. Also it needs flex and bison to generate a simple scanner and an even more simple parser.<br />
&nbsp;&nbsp;&nbsp;&nbsp;C-Matrix mostly contains a API along with a shared library and an executable program. The former is two head files in the include folder and a so file generated in the bin folder. The latter is a interactive command line tool to run some matrix calculation. Just running make will get these files.<br />
&nbsp;&nbsp;&nbsp;&nbsp;The test folder include a sample program to illustrate how to use the API, covering most of the interfaces.
# 3 Interactive tool
&nbsp;&nbsp;&nbsp;&nbsp;The interactive tool xinyuw generated in the bin folder depends on the previously mentioned so file. Before starting it I always run the env.sh to set the $LD_LIBRARY_PATH.

## 3.1 Vector scalar computation
a=<345,-5465454543,32344,3345><br />
b=<1,2,3,-6,2.7E5><br />
c=a*2; d=b/2<br />
## 3.2 Vector plus/minus<br />
a=<1,2,3,-6,2.7E5><br />
b=<7,99, -100, -5456.35, 735><br />
c=a+b; d = a-b;<br />
## 3.3 Matrix zeros/ones/eye/rand<br />
zeros(2,4)<br />
a=zeros(3, 'int64')<br />
b=ones(3,2,'single')<br />
c=eye(3, 'int16')<br />
d=rand(3,5)<br />
The last parameter is a optional string, it may be single|double|int8|int16|int32|int64|uint8|uint16|uint32|uint64. The default value is “double”.<br />
## 3.4 Matrix diag<br />
a=<10, -20, 30, -40, 50><br />
b=diag(a); c=diag(a,2); d=diag(c, 2)<br />
## 3.5 Matrix blkdiag<br />
a=[1,2,3;-4,-5,-6]; b=rand(3,4); c=eye(2,'int64')<br />
d=blkdiag(c,b*1000,a,'int32')<br />
## 3.6 Matrix cat<br />
a=rand(2,4,'int8'); b=ones(2,4,'int8');<br />
c=cat(1,a,b)<br />
a=rand(3,2,'int8'); b=eye(3,'int8')<br />
c=cat(2,a,b,'int32')<br />
The first parameter of cat is a integer, 1 means 1st-dimension, 2 means 2nd-dimension.<br />
## 3.7 Matrix plus/minus/multiply/inverse/left-divide/right-divide<br />
a=[1,2,3;-4,-5,-6;7,8,9]; b=[-9,-8,-7;3,2,1;-6,-5,-4]<br />
a+b<br />
a-b<br />
a*b<br />
inv(a)<br />
a/b<br />
a\b<br />
## 3.8 Matrix scalar computation<br />
a=rand(3)<br />
a+5; a-5; a*5; a/5<br />
## 3.9 Matrix transpose<br />
a=rand(3,3,'int8')<br />
transpose(a)<br />
## 3.10 Matrix determinant<br />
a=rand(5)<br />
det(a)<br />
This function use a recursion algorithm which takes a O(n!) complexity, tremendous sub-matrix are generated and destroyed during it’s execution. The max scale I ever tried is 13, it seems never return when the size goes up to 14.<br />
## 3.11 quit the program<br />
Both Ctrl-D and quit will terminate the program.
