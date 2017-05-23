@.strint = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@.strdouble = private unnamed_addr constant [7 x i8] c"%.16E\0A\00"
define void @doubleclass.main(i32 %args.c, i8** %args.v){
%call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strdouble, i32 0, i32 0), double 4.5)
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
	call void @doubleclass.main(i32 %1, i8** %2)
	ret i32 0
}
