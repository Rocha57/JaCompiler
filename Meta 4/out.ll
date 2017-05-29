@.strint = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@.strdouble = private unnamed_addr constant [7 x i8] c"%.16E\0A\00"
@.strtrue = private unnamed_addr constant [6 x i8] c"true\0A\00"
@.strfalse = private unnamed_addr constant [7 x i8] c"false\0A\00"

@gcd = global i1 0
@aaa = global double 0.0
define void @gcd2.main(i32 %args.c, i8** %args.v){
	store i1 false, i1* @gcd
	%b = alloca i32
	store i32 3, i32* %b
	%c = alloca double
	%1 = load i32, i32* %b
	%sub2 = sub nsw i32 0, %1	%conv2 = sitofp i32 %sub2 to double
	store double %conv2, double* @aaa
	%2 = load i32, i32* %b
	%call0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.strint, i32 0, i32 0), i32 %2)
	%3 = load i1, i1* @gcd
	%ifcond1 = icmp eq i1 %3, 1
	br i1 %ifcond1 , label %then1 , label %else1
then1:
	%call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.strtrue, i32 0, i32 0))
	br label %ifcont2
else1:
	%call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strfalse, i32 0, i32 0))
	br label %ifcont2
ifcont2:
	%4 = load double, double* @aaa
	%call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strdouble, i32 0, i32 0), double %4)
	%5 = load double, double* %c
	%call4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strdouble, i32 0, i32 0), double %5)
	ret void
}

declare i32 @atoi(i8*)
declare i32 @printf(i8*, ...)
define i32 @main(i32 %argc, i8** %argv){
	%retval = alloca i32
	%argc.addr = alloca i32
	%argv.addr = alloca i8**
	store i32 0, i32* %retval
	store i32 %argc, i32* %argc.addr
	store i8** %argv, i8*** %argv.addr
	%1 = load i32, i32* %argc.addr
	%2 = load i8**, i8*** %argv.addr
	call void @gcd2.main(i32 %1, i8** %2)
	ret i32 0
}
