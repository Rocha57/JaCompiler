@.strint = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@.strdouble = private unnamed_addr constant [7 x i8] c"%.16E\0A\00"

define void @zero.main(i32 %args.c, i8** %args.v){
	%a = alloca i32
	%b = alloca i32
	store i32 3, i32* %a
	%1 = load i32, i32* %a
	store i32 %1, i32* %b
	%2 = load i32, i32* %a
	%3 = load i32, i32* %b
	%4 = add i32 %2, %3
	%call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.strint, i32 0, i32 0), i32 %4)
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
	call void @zero.main(i32 %1, i8** %2)
	ret i32 0
}
