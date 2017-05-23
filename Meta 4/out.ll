@.strint = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@.strdouble = private unnamed_addr constant [7 x i8] c"%.16E\0A\00"

define void @Echo.main(i32 %args.c, i8** %args.v){
	%x = alloca i32
	%y = alloca i32
	%args.c.addr = alloca i32
	%args.v.addr = alloca i8**
	store i32 %args.c, i32* %args.c.addr
	store i8** %args.v, i8*** %args.v.addr
	%1 = load i8**, i8*** %args.v.addr
	%arrayid1 = getelementptr inbounds i8*, i8** %1, i64 1
	%2 = load i8*, i8** %arrayid1
	%3 = call i32 @atoi(i8* %2)
	store i32 %3, i32* %x
	%4 = load i8**, i8*** %args.v.addr
	%arrayid2 = getelementptr inbounds i8*, i8** %4, i64 2
	%5 = load i8*, i8** %arrayid2
	%6 = call i32 @atoi(i8* %5)
	store i32 %6, i32* %y
	%7 = load i32, i32* %x
	%8 = load i32, i32* %y
	%9 = add i32 %7, %8
	%call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.strint, i32 0, i32 0), i32 %9)
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
	call void @Echo.main(i32 %1, i8** %2)
	ret i32 0
}
